#ifndef __MYSHELL_H__
#define __MYSHELL_H__

/* * myshell.h - Prototypes and definitions for the MyShell project.
 * Based on the CS:APP3e shell example (shellex.c).
 */

#include "csapp.h" // 조교님이 제공할 라이브러리 헤더 포함

/* Misc constants */
#define MAXARGS 128 /* Max number of arguments in a command line */

/* Function prototypes for myshell logic */

/* eval - Evaluate a command line */
void eval(char *cmdline);

/* parseline - Parse the command line and build the argv array */
int parseline(const char *cmdline, char **argv);

/* builtin_command - If first arg is a builtin command, run it and return true */
int builtin_command(char **argv);



int parse_pipe(char *cmdline, char **cmds);

/* * Phase 1에서 추가할 수 있는 유틸리티 함수 선언 예시
 * (필요에 따라 직접 추가해 보세요)
 */
// void execute_external_command(char **argv, int bg);

#endif /* __MYSHELL_H__ */