/* $begin shellmain */
#include "myshell.h"
#include<errno.h>
#define MAXARGS   128


struct job_t jobs[MAXJOBS]; // 프로세스 리스트. 모든 프로세스의 정보를 기록함.
int next_jid = 1;  // 다음에 부여할 job id.



int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    initjobs(); /* 쉘 시작 시 장부 초기화 */

    /* 시그널 핸들러 등록 */
    Signal(SIGINT,  sigint_handler);
    Signal(SIGTSTP, sigtstp_handler);
    Signal(SIGCHLD, sigchld_handler);
    
    while (1) {

        // 만약 아무것도 입력 안하기 엔터키 눌러도, TM 상태인 job 있으면 처리해주기.
        reap_terminated_jobs(jobs);

        /* Read */
        printf("CSE4100-SP-P2> ");
        fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* 사용자가 입력한 문장을 분석하고 어떻게 실행할지 결정 */
void eval(char *cmdline) 
{   

    // 프로세스가 실행되고, jobs에 저장되기 전에 바로 끝나버리는 문제.

    char *cmds[MAXARGS]; // 덩어리들을 담을 새로운 배열
    int num_cmds;        // 명령어의 개수
    sigset_t mask_all, mask_one, prev_one;

    // [추가할 코드] 문자열이 잘리기 전에 원본을 보관할 배열 생성 및 복사
    char original_cmdline[MAXLINE];
    strcpy(original_cmdline, cmdline);

    // 시그널 집합 준비
    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);

    // 1. 일단 파이프를 기준으로 명령어들을 자르기
    num_cmds = parse_pipe(cmdline, cmds);

    // 2. 파이프가 없는 경우, 즉 명령어가 단일 명령어 하나인 경우.
    if (num_cmds == 1) {

        char *argv[MAXARGS];
        char buf[MAXLINE];
        int bg;
        pid_t pid;
        
        strcpy(buf, cmdline);
        bg = parseline(buf, argv); 
        if (argv[0] == NULL)  return;

        if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run

            // 자식이 생성되기 전에 SIGCHLD를 차단 -> jobs에 프로세스를 추가하기도 전에 프로세스가 끝나서 오류나는 것을 방지
            Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);

            // 1. Fork()를 호출하여 자식 프로세스 생성
            pid = Fork();

            // 2. 자식 프로세스일 경우
            if (pid == 0) { 

                // 새로운 프로세스 그룹 생성 및 시그널 차단 해제
                Setpgid(0, 0); // 새로운 프로세스 그룹 생성
                Sigprocmask(SIG_SETMASK, &prev_one, NULL);

                // execvp() 실행
                if (execvp(argv[0], argv) < 0) {
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                }
            }

            // 3. 부모 프로세스일 경우

            // jobs 리스트를 수정하는 동안 모든 시그널 차단
            Sigprocmask(SIG_BLOCK, &mask_all, NULL);
            addjob(pid, (bg ? BG : FG), original_cmdline);
            Sigprocmask(SIG_SETMASK, &prev_one, NULL); // 다시 SIGCHLD만 차단된 상태 혹은 이전 상태로 복구
            
            // 포그라운드 경우
            if (!bg){
                int status;

                waitfg(pid);
            }

            // 백그라운드 경우
            else
                printf("[%d] (%d) %s", pid2jid(pid), (int)pid, cmdline);
        }
    }

    // 3. 파이프가 있는 경우
    else {
        int fd[2];
        int fd_in = 0; // 이전 파이프의 출구 (처음엔 표준 입력 0)
        pid_t pids[MAXARGS]; // 자식들의 PID를 보관할 배열
        char *argv[MAXARGS];

        // 마지막 명령어(cmds[num_cmds-1])를 임시로 파싱하여 bg 여부를 알아냅니다.
        char *temp_argv[MAXARGS];
        int bg = parseline(cmds[num_cmds - 1], temp_argv);

        // 파이프 전체를 하나의 작업으로 관리하기 위해 시그널 차단
        Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);

        for (int i = 0; i < num_cmds; i++) {

            // 마지막 명령어가 아니면 새로운 파이프 생성
            if (i < num_cmds - 1) {
                if (pipe(fd) < 0) { // 파이프 새로 만들기. fd[0], fd[1]에 번호 붙음(3 4, 5 6, ...)
                    perror("pipe");
                    return;
                }
            }

            // 자식 프로세스
            if ((pids[i] = Fork()) == 0) {

                // 첫 번째 자식을 그룹 리더로, 나머지는 그 그룹에 합류
                Setpgid(0, (i == 0) ? 0 : pids[0]);
                Sigprocmask(SIG_SETMASK, &prev_one, NULL);

                // 1. 입력을 fd_in으로 설정 -> 이전 파이프의 출구에서 입력 받음
                if (fd_in != 0) {
                    dup2(fd_in, STDIN_FILENO); // 프로세스가 입력받는 곳을, STDIN_FILENO가 fd_in 으로부터 받도록 지정
                    close(fd_in);
                }

                // 2. 출력을 fd[1]로 설정 -> 현재 파이프의 입구로 출력 (마지막 명령어 제외)
                if (i < num_cmds - 1) {
                    dup2(fd[1], STDOUT_FILENO); // 프로세스가 출력하는 곳을, STDOUT_FILENO 파이프의 출력부 fd[1]로 출력을 하라고 지정.
                    close(fd[0]);
                    close(fd[1]);
                }

                // 명령어 실행

                parseline(cmds[i], argv);

                // [수정 포인트 추가] 내장 명령어인지 확인하고, 맞으면 실행 후 자식 종료
                if (builtin_command(argv)) {
                    exit(0); 
                }

                if (execvp(argv[0], argv) < 0) {
                    fprintf(stderr, "%s: Command not found.\n", argv[0]);
                    fflush(stderr); // 메시지를 즉시 화면으로 밀어내기
                    exit(1);
                }
            }

            // 부모 프로세스
            else {
                if (fd_in != 0) close(fd_in);

                if (i < num_cmds - 1) {
                    close(fd[1]);
                    fd_in = fd[0]; // 다음 프로세스가 이전 파이프의 출력부 fd[0]으로부터 입력받을 수 있도록 fd_in 기록
                }
            }
        }

        /* [로직 추가] 파이프 그룹의 첫 번째 프로세스를 jobs에 등록 */
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        addjob(pids[0], (bg ? BG : FG), original_cmdline);
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);

        // 포그라운드 실행이면 waitfg를 통해 모든 자식들이 다 종료될 때까지 기다리기
        if (!bg) {

            // jobs에 등록된 대표 프로세스 처리 기다리기
            waitfg(pids[0]);

            reap_terminated_jobs(jobs);

            // 나머지 자식들도 모두 종료될 때까지 대기
            // 이렇게 나머지 자식들도 기다려 준 후에 CSE4100 ~ 룰 출력해줘야함.
            for (int i = 1; i < num_cmds; i++) {
                int status;
                /* sigchld_handler가 먼저 가로채서(reap) 에러가 날 수 있으나, 
                   결과적으로 대기 효과를 주어 프롬프트가 먼저 뜨는 것을 방지합니다. */
                waitpid(pids[i], &status, WUNTRACED);
            }

        } else {
            printf("[%d] (%d) %s", pid2jid(pids[0]), (int)pids[0], cmdline);
        }
    }

    return;
}

/* 첫 번째 명령어(argv[0])를 확인해서, 리눅스 외부 프로그램이 아닌 셸 내부에서 직접 기능을 수행해야 하는 명령어인지 검사하고 실행 */
int builtin_command(char **argv) 
{

    if (!strcmp(argv[0], "exit"))
        exit(0);

    if (!strcmp(argv[0], "cd")) {

        // 인자가 없는 경우 (그냥 'cd'만 쳤을 때) -> 홈 디렉터리로 이동 
        if (argv[1] == NULL) {
            chdir(getenv("HOME"));
        } 

        // 인자가 있는 경우 ('cd abcd' 등) -> 해당 디렉터리로 이동 [cite: 45]
        else {
            if (chdir(argv[1]) < 0) { // chdir이 실패하면 -1 반환
                printf("%s: No such file or directory\n", argv[1]);
            }
        }
        return 1; // 내장 명령어로 성공적으로 처리했음을 eval()에 알림
    }

    if (!strcmp(argv[0], "jobs")) {
        listjobs(); // 
        return 1;
    }

    if (!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg") || !strcmp(argv[0], "kill")) {
        do_bgfg_kill(argv); // 인자 처리를 위한 헬퍼 함수 호출 [cite: 111-113]
        return 1;
    }

    if (!strcmp(argv[0], "&"))
	    return 1;

    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* 한 줄의 문자열로 들어온 입력을 시스템 콜이 다루기 쉽게 배열(argv) 형태로 자르기 */
int parseline(const char *cmdline, char **argv) {
    static char array[MAXLINE]; /* cmdline의 복사본을 저장할 배열 */
    char *buf = array;          /* array를 순회할 포인터 */
    int argc = 0;               /* 인자의 개수 */
    int bg = 0;                 /* 백그라운드 실행 여부 (&) */

    strcpy(array, cmdline);

    /* 1. 줄바꿈 문자 제거 */
    int len = strlen(array);
    if (len > 0 && array[len - 1] == '\n') {
        array[len - 1] = ' ';
        len--;
    }

    /* 2. 백그라운드 실행 여부(&) 확인 - 문자열 끝에서부터 거꾸로 탐색 */
    /* sort foo.txt& 나 sort foo.txt &   모두 대응 가능  */
    for (int i = len - 1; i >= 0; i--) {
        if (array[i] == ' ') continue; // 공백은 건너뜀
        if (array[i] == '&') {
            bg = 1;
            array[i] = ' '; // '&'를 공백으로 바꿔서 토큰화 시 무시되게 함
            break;
        } else {
            break; // '&'가 아닌 다른 문자를 만나면 백그라운드가 아님
        }
    }

    /* 3. 인자 리스트 빌드 */
    while (*buf && (*buf == ' ')) buf++; // 앞부분 공백 무시

    while (*buf) {
        if (*buf == '\"') { // 따옴표 처리
            buf++;
            argv[argc++] = buf;
            while (*buf && *buf != '\"') buf++;
        } else { // 일반 단어 처리
            argv[argc++] = buf;
            while (*buf && *buf != ' ') buf++;
        }

        if (*buf == '\0') break;
        *buf++ = '\0';
        while (*buf && (*buf == ' ')) buf++;
    }

    argv[argc] = NULL; // 마지막은 항상 NULL

    if (argc == 0) return 1; // 빈 줄 처리

    return bg;
}
/* $end parseline */

/* parse_pipe - 파이프(|)를 기준으로 문자열을 분리하고 명령어의 개수를 반환. */
int parse_pipe(char *cmdline, char **cmds) {
    int count = 0;
    char *ptr = cmdline;
    char *pipe_ptr;

    // 문자열에서 '|'를 찾을 때까지 반복
    while ((pipe_ptr = strchr(ptr, '|')) != NULL) {
        *pipe_ptr = '\0';     // '|' 자리를 널 문자('\0')로 바꿔서 문자열을 끊어버림
        cmds[count++] = ptr;  // 끊어진 왼쪽 덩어리를 배열에 저장
        ptr = pipe_ptr + 1;   // 포인터를 '|' 다음 글자로 이동
    }
    
    cmds[count++] = ptr;      // 마지막 남은 오른쪽 덩어리 저장
    cmds[count] = NULL;       // 배열의 끝을 표시
    
    return count;             // 총 몇 개의 명령어로 쪼개졌는지 반환 (파이프가 없으면 1)
}

// jobs 장부를 초기화
void initjobs() {
    for (int i = 0; i < MAXJOBS; i++) {
        jobs[i].pid = 0;
        jobs[i].jid = 0;
        jobs[i].state = UNDEF;
        jobs[i].cmdline[0] = '\0';
    }
}

// jobs에 새로운 프로세스를 추가
int addjob(pid_t pid, int state, char *cmdline) {
    if (pid < 1) return 0;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == 0) { // 빈 칸 찾기
            jobs[i].pid = pid;
            jobs[i].state = state;
            jobs[i].jid = get_next_jid(jobs);
            if (next_jid > MAXJID) next_jid = 1;
            strcpy(jobs[i].cmdline, cmdline);
            return 1;
        }
    }
    printf("Quota full: maximum number of jobs reached\n");
    return 0;
}

// jobs에서 프로세스를 제거
int deletejob(pid_t pid) {
    struct job_t *job = getjobpid(pid);
    if (job == NULL) return 0;
    clearjob(job);
    return 1;
}

// jobs에서 프로세스를 제거할 때, struct의 데이터를 모두 0으로 만들어줌.
void clearjob(struct job_t *job) {
    job->pid = 0;           /* PID를 0으로 설정하여 빈 칸임을 표시 */
    job->jid = 0;           /* Job ID 초기화 */
    job->state = UNDEF;     /* 상태를 정의되지 않음(0)으로 설정 */
    job->cmdline[0] = '\0'; /* 명령어 문자열 비우기 */
}

// 특정 프로세스의 pid를 가져오기
struct job_t *getjobpid(pid_t pid) {
    if (pid < 1) return NULL;
    for (int i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid) return &jobs[i];
    return NULL;
}

// 특정 프로세스의 jid를 가져오기
struct job_t *getjobjid(int jid) {
    if (jid < 1) return NULL;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].jid == jid) {
            return &jobs[i]; // 찾으면 해당 Job의 주소 반환
        }
    }
    return NULL; // 못 찾으면 NULL 반환 (에러 처리용) [cite: 116]
}

// 각 프로세스가 어떤 상태(running / foreground / stopped / unknown) 인지 출력
// jobs 배열에서, jid 순서대로 job들을 출력할 수 있게, 일단은 2중 반복문으로 구현.
// 후에 가능하면, 최적화를 위해 다른 방안 고려하자
void listjobs() {
    int j, i;
    // 1. JID 1번부터 현재 가질 수 있는 최대 JID까지 순차적으로 확인
    // MAXJOBS만큼 돌면서 현재 존재하는 JID들을 순서대로 찾습니다.
    for (j = 1; j <= MAXJID; j++) { 
        int found_count = 0; // 최적화를 위해 찾은 개수 체크
        for (i = 0; i < MAXJOBS; i++) {
            if (jobs[i].pid != 0 && jobs[i].jid == j) {
                printf("[%d] (%d) ", jobs[i].jid, (int)jobs[i].pid);
                switch (jobs[i].state) {
                    case BG: printf("Running "); break;
                    case FG: printf("Foreground "); break;
                    case ST: printf("Stopped "); break;
                    case TM: printf("Terminated "); break;
                    default: printf("Unknown state ");
                }
                printf("%s", jobs[i].cmdline);

                if (jobs[i].state == TM) {
                    deletejob(jobs[i].pid);
                }
                found_count = 1;
                break; // 해당 JID를 찾았으니 다음 번호(j+1)로 넘어감
            }
        }
        // 더 이상 찾을 JID가 없으면 (장부의 모든 작업을 확인했다면) 종료하는 로직을 넣을 수도 있습니다.
    }
}

// 자식 프로세스가 종료되거나 멈췄을 때 처리해주기
void sigchld_handler(int sig) {

    // 기존 에러 기록해두기
    int olderrno = errno;

    // 변수 선언
    sigset_t mask_all, prev_all;
    pid_t pid;
    int status;

    // 시그널 처리하고 jobs 수정하는 동안 다른 신호가 방해하지 못하게 설정
    Sigfillset(&mask_all);

    // 반복문으로 더 이상 종료된 자식이 없을 때까지 reap.
    // WNOHANG: 종료된 자식 없으면 바로 리턴하게 하여 쉘이 멈추는 것을 방지
    // WUNTRACED: 종료된 자식 뿐만 아니라 Ctrl+Z로 인해 Stopped된 상태도 감지
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        
        // 모든 시그널 차단. 이전 시그널 비트 기록
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);

        // [추가] 장부에서 해당 PID를 가진 job을 찾아옵니다.
        struct job_t *job = getjobpid(pid);

        // 정상 종료는 그냥 삭제
        if (WIFEXITED(status)) {
            if (job) deletejob(pid);
        } 

        // 시그널에 의해 종료된 경우(kill 등)
        else if (WIFSIGNALED(status)) {
            if (job) {
                job->state = TM; // jobs 에서 지우지 않고 '종료됨' 상태로 표시
            }
        }

        // ctrl + z 등에 의해 stopped 된 경우
        else if (WIFSTOPPED(status)) {
            struct job_t *job = getjobpid(pid);
            if (job != NULL) { 
                job->state = ST; // 상태를 ST(Stopped)로 변경

                // 표준 쉘(bash) 느낌으로 출력 포맷 변경!
                Sio_puts("[");
                Sio_putl(job->jid);
                Sio_puts("] Stopped             ");
                Sio_puts(job->cmdline); // 장부에 있는 전체 명령어 문자열 출력 (끝에 \n 포함되어 있음)
            }
        }

        // 이전 시그널 비트 되돌리기
        Sigprocmask(SIG_SETMASK, &prev_all, NULL); // 블록 해제
    }
    
    // 기존 에러 복구하기.
    errno = olderrno;
}

// 컨트롤 C 처리. 현재 포그라운드에서 실행 중인 모든 프로세스에게 SIGINT 시그널 보내서 프로세스 종료시키기.
void sigint_handler(int sig) { // 컨트롤 C 처리
    pid_t pid = fgpid(jobs); // 현재 포그라운드 프로세스 PID 찾기

    if (pid != 0) {
        // 포그라운드 프로세스 그룹 전체에 SIGINT 전달
        Kill(-pid, SIGINT); 
    }
}

// 컨트롤 Z 처리. 현재 포그라운드에서 실행 중인 모든 프로세스에게 SIGTSTP 시그널 보내서 STOPPED STATE로 만들기.
// SIGCONT 시그널을 받을 때까지 STOP.
void sigtstp_handler(int sig) { // 컨트롤 Z 처리. 
    pid_t pid = fgpid(jobs); // 현재 포그라운드 프로세스 PID 찾기

    if (pid != 0) {
        // 포그라운드 프로세스 그룹 전체에 SIGTSTP 전달
        Kill(-pid, SIGTSTP);
    }
}

// 현재 포그라운드에서 실행 중인 작업의 PID를 반환 (없으면 0)
pid_t fgpid(struct job_t *jobs) {
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].state == FG) {
            return jobs[i].pid;
        }
    }
    return 0;
}

// PID를 이용해 해당 작업의 JID를 반환
int pid2jid(pid_t pid) {
    if (pid < 1) return 0;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    }
    return 0;
}


/* 포그라운드 작업이 더 이상 FG 상태가 아닐 때까지(종료되거나 정지될 때까지) 대기 */
void waitfg(pid_t pid) {
    sigset_t mask;
    Sigemptyset(&mask); // 아무것도 차단하지 않는 빈 마스크

    /* 해당 PID의 작업이 jobs 리스트에 있고 상태가 FG인 동안 루프 */
    /* sigchld_handler가 자식을 reap하고 deletejob을 호출하면 루프 탈출 */
    while (fgpid(jobs) == pid) {
        sigsuspend(&mask); // 시그널이 올 때까지 효율적으로 대기
    }
}

void do_bgfg_kill(char **argv) 
{
    struct job_t *job = NULL;
    char *id = argv[1];

    if (id == NULL) {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }

    // 1. JID인지 PID인지 판별하여 Job 찾기
    // JID인 경우 (예: %5)
    if (id[0] == '%') {
        job = getjobjid(atoi(&id[1]));
        if (!job) { printf("%s: No such job\n", id); return; }
    } 

    // PID인 경우 (예: 1234)
    else if (isdigit(id[0])) {
        job = getjobpid(atoi(id));
        if (!job) { printf("(%s): No such process\n", id); return; }
    } 

    else {
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
    }


    // 2. 명령어에 따른 시그널 전송 및 상태 변경
    if (!strcmp(argv[0], "kill")) {
        Kill(-(job->pid), SIGINT); // 프로세스 그룹 전체에 종료 시그널 전송

        // [추가된 로직] 만약 작업이 정지된(Stopped) 상태라면, 깨워야 시그널을 처리하고 종료될 수 있습니다.
        if (job->state == ST) {
            Kill(-(job->pid), SIGCONT); 
        }
    }

    else if (!strcmp(argv[0], "bg")) {
        job->state = BG; // 상태를 Running(BG)으로 변경 [cite: 111]
        Kill(-(job->pid), SIGCONT); // 정지된 프로세스를 다시 깨움 [cite: 133]

        // [추가할 부분] cmdline 끝에 '&'가 없다면 동적으로 붙여주기
        int len = strlen(job->cmdline);
        
        // 1. 끝에 있는 줄바꿈('\n') 잠시 제거
        if (len > 0 && job->cmdline[len - 1] == '\n') {
            job->cmdline[len - 1] = '\0';
            len--; // 길이 갱신
        }
        
        // 2. 맨 끝 문자가 '&'가 아니라면 " &" 추가
        if (len > 0 && job->cmdline[len - 1] != '&') {
            strcat(job->cmdline, " &");
        }
        
        // 3. 줄바꿈('\n') 다시 원상 복구 (listjobs 출력을 위해)
        strcat(job->cmdline, "\n");
        printf("[%d] (%d) %s", job->jid, (int)job->pid, job->cmdline);
    } 
    
    else if (!strcmp(argv[0], "fg")) {
        job->state = FG; // 상태를 Foreground로 변경 [cite: 112]
        Kill(-(job->pid), SIGCONT); // 깨운 후 [cite: 133]

        // [추가할 부분] cmdline 끝에 '&'가 있다면 제거하기
        int len = strlen(job->cmdline);
        
        // 1. 끝에 있는 줄바꿈('\n') 잠시 제거
        if (len > 0 && job->cmdline[len - 1] == '\n') {
            job->cmdline[len - 1] = '\0';
            len--;
        }
        
        // 2. 뒤에서부터 공백을 무시하고 찾았을 때 '&'가 있다면 제거
        for (int i = len - 1; i >= 0; i--) {
            if (job->cmdline[i] == ' ') continue;
            if (job->cmdline[i] == '&') {
                job->cmdline[i] = '\0'; // '&'를 널 문자로 덮어써서 문자열을 여기서 끊음
                break;
            } else {
                break; // '&'가 아닌 다른 글자면 아무것도 안 함
            }
        }
        
        // 3. 줄바꿈('\n') 다시 원상 복구 (출력을 위해)
        strcat(job->cmdline, "\n");

        waitfg(job->pid); // 끝날 때까지 대기
    }
}

// TM 상태의 job 들을 처리하는 함수
// 여기더 JID 순으로 출력할 수 있도록 2중 반복문으로 설정.
void reap_terminated_jobs(struct job_t *jobs) {
    int j, i;
    for (j = 1; j <= MAXJID; j++) {
        for (i = 0; i < MAXJOBS; i++) {
            if (jobs[i].pid != 0 && jobs[i].jid == j && jobs[i].state == TM) {
                // 가시성을 위해 출력 형식을 살짝 다듬었습니다.
                printf("[%d] Terminated             %s", jobs[i].jid, jobs[i].cmdline);
                deletejob(jobs[i].pid);
                break;
            }
        }
    }
}

/* 현재 장부에 있는 작업 중 가장 큰 JID를 찾아 +1을 반환하는 함수 */
int get_next_jid(struct job_t *jobs) {
    int max = 0;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid != 0) { // 현재 사용 중인 작업들 중에서
            if (jobs[i].jid > max) {
                max = jobs[i].jid;
            }
        }
    }
    return max + 1;
}

