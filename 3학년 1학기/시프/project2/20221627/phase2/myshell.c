/* $begin shellmain */
#include "myshell.h"
#include<errno.h>
#define MAXARGS   128

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
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

    char *cmds[MAXARGS]; // 덩어리들을 담을 새로운 배열
    int num_cmds;        // 명령어의 개수

    // 1. 일단 파이프를 기준으로 명령어들을 자르기
    num_cmds = parse_pipe(cmdline, cmds);

    // 2. 파이프가 없는 경우, 즉 명령어가 단일 명령어 하나인 경우.
    if (num_cmds == 1) {

        char *argv[MAXARGS]; /* Argument list execve() */
        char buf[MAXLINE];   /* Holds modified command line */
        int bg;              /* Should the job run in bg or fg? */
        pid_t pid;           /* Process id */
        
        strcpy(buf, cmdline);
        bg = parseline(buf, argv); 
        if (argv[0] == NULL)  return;   /* Ignore empty lines */

        if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
            // 1. Fork()를 호출하여 자식 프로세스 생성
            pid = Fork();

            // 2. 자식 프로세스일 경우: execvp() 실행
            if (pid == 0) { 
                if (execvp(argv[0], argv) < 0) {
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                }
            }

            // 3. 부모 프로세스일 경우 (쉘) - 지금은 포그라운드만 보니까 무조건 이거.
            if (!bg){  // 지금 단계에서는 bg는 무조건 0
                int status;

                // 포그라운드 실행이므로 자식(pid)이 종료될 때까지 대기
                Waitpid(pid, &status, 0);
            }

            // 부모 프로세스 - 백그라운드 경우. 일단 printf 로만 처리
            else
                printf("%d %s", pid, cmdline);
        }
    }

    // 3. 파이프가 있는 경우
    else {
        int fd[2];
        int fd_in = 0; // 이전 파이프의 출구 (처음엔 표준 입력 0)
        pid_t pids[MAXARGS]; // 자식들의 PID를 보관할 배열
        char *argv[MAXARGS];

        for (int i = 0; i < num_cmds; i++) {

            // 마지막 명령어가 아니면 새로운 파이프 생성
            if (i < num_cmds - 1) {
                if (pipe(fd) < 0) { // 파이프 새로 만들기. fd[0], fd[1]에 번호 붙음(3 4, 5 6, ...)
                    perror("pipe");
                    return;
                }
            }

            if ((pids[i] = Fork()) == 0) { // 자식 프로세스

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
                if (execvp(argv[0], argv) < 0) {
                    fprintf(stderr, "%s: Command not found.\n", argv[0]);
                    fflush(stderr); // 메시지를 즉시 화면으로 밀어내기
                    exit(1);
                }
            }

            else { // 부모 프로세스 (셸)
                
                if (fd_in != 0) close(fd_in);

                if (i < num_cmds - 1) {
                    close(fd[1]);
                    fd_in = fd[0]; // 다음 프로세스가 이전 파이프의 출력부 fd[0]으로부터 입력받을 수 있도록 fd_in 기록
                }
            }
        }

        // ★ 모든 자식을 다 낳은 후, 여기서 한꺼번에 기다립니다.
        for (int i = 0; i < num_cmds; i++) {
            int status;
            Waitpid(pids[i], &status, 0);
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
    int bg;                     /* 백그라운드 실행 여부 (&) */

    strcpy(array, cmdline);
    /* 줄바꿈 문자 제거 */
    if (array[strlen(array) - 1] == '\n')
        array[strlen(array) - 1] = ' ';

    /* 앞부분 공백 무시 */
    while (*buf && (*buf == ' '))
        buf++;

    /* 인자 리스트 빌드 */
    while (*buf) {
        if (*buf == '\"') { // 1. 따옴표로 시작하는 경우
            buf++; // 시작 따옴표 제거(건너뛰기)
            argv[argc++] = buf;
            /* 다음 따옴표를 만날 때까지 공백이 있어도 계속 전진 */
            while (*buf && *buf != '\"') 
                buf++;
        } else { // 2. 일반 단어인 경우
            argv[argc++] = buf;
            /* 다음 공백을 만날 때까지 전진 */
            while (*buf && *buf != ' ') 
                buf++;
        }

        if (*buf == '\0') break; // 문자열 끝이면 종료

        /* 단어의 끝(공백 혹은 닫는 따옴표)을 NULL로 바꿔서 단어 분리 */
        *buf++ = '\0';

        /* 다음 단어 시작 전까지의 공백들을 모두 건너뜀 */
        while (*buf && (*buf == ' '))
            buf++;
    }

    argv[argc] = NULL; // argv의 마지막은 항상 NULL

    if (argc == 0) return 1; // 빈 명령줄인 경우

    /* 백그라운드 실행 여부 확인 (&) */
    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }

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

