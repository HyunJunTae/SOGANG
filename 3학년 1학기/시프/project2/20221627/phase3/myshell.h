#ifndef __MYSHELL_H__
#define __MYSHELL_H__


/* 상수 정의 */
#define MAXJOBS 16      /* 동시에 실행될 수 있는 최대 작업 수 [cite: 110] */
#define MAXJID 1<<16    /* Job ID의 최대치 */

/* 작업 상태(Job States) 정의 [cite: 106, 133] */
#define UNDEF 0 /* 미정의 */
#define FG    1 /* Foreground: 포그라운드에서 실행 중 */
#define BG    2 /* Background: 백그라운드에서 실행 중 [cite: 107] */
#define ST    3 /* Stopped: 정지됨 (Ctrl+Z 등에 의해) [cite: 106, 133] */
#define TM 4    /* terminated (추가!) */

/* * myshell.h - Prototypes and definitions for the MyShell project.
 * Based on the CS:APP3e shell example (shellex.c).
 */

#include "csapp.h" // 조교님이 제공할 라이브러리 헤더 포함

/* Misc constants */
#define MAXARGS 128 /* Max number of arguments in a command line */

/* Function prototypes for myshell logic */

void eval(char *cmdline);
int parseline(const char *cmdline, char **argv);
int builtin_command(char **argv);
int parse_pipe(char *cmdline, char **cmds);


/* --- 작업 구조체 정의 --- */
struct job_t {              
    pid_t pid;              
    int jid;                
    int state;              
    char cmdline[MAXLINE];  
};

/* --- [Phase 3] Job 관리 함수 프로토타입 --- */
void initjobs(); 
int addjob(pid_t pid, int state, char *cmdline);
int deletejob(pid_t pid);
void clearjob(struct job_t *job);
struct job_t *getjobpid(pid_t pid);
struct job_t *getjobjid(int jid); 
void listjobs();

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);
pid_t fgpid(struct job_t *jobs);
int pid2jid(pid_t pid);
void waitfg(pid_t pid);
void do_bgfg_kill(char **argv);
void reap_terminated_jobs(struct job_t *jobs);
int get_next_jid(struct job_t *jobs);


#endif /* __MYSHELL_H__ */