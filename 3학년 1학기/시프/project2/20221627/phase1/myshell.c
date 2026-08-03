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
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */
    if (!builtin_command(argv)) {
        // 1. Fork()를 호출하여 자식 프로세스 생성
        pid = Fork();

        // 2. 자식 프로세스일 경우: execve() 실행
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
    return;
}

/* If first arg is a builtin command, run it and return true */
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

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
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
