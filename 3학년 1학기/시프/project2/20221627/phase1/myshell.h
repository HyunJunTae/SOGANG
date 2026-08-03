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

#endif /* __MYSHELL_H__ */