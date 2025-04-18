#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"
#include "helper.h"
#include "command.h"
#include <signal.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "statuslist.h"
#include "execute.h"

#include "debug.h"


#ifndef NOLIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif


/*
 * the cmd structure is build and maintained by the parser only!
 * Do not worry about it, it works!
 */
extern Command * cmd;
extern List *statuslist;
extern ProcessList *processList;

#ifndef NOLIBREADLINE
extern char *current_readline_prompt;
#endif

int fdtty;
int shell_pid;


typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex();
extern char *yytext;




void disable_signal(int signo, void(*handler)(int) ,int flags, int action){
     struct sigaction siga;
    siga.sa_handler= (action == 0) ? handler : SIG_IGN;
    siga.sa_flags=flags;
    sigemptyset(&siga.sa_mask);
    if(sigaction(signo, &siga, NULL) < 0){
        perror("Error in sigaction");
        exit(1);
    }
}

void signal_handler(int signo) {
    pid_t wpid;
    int status;
    Process *process;

    while ((wpid = waitpid(-1, &status, WNOHANG)) > 0) {
        process = findProcess(wpid, processList);
        if (process != NULL) {
            process->status = status;
            process->isRunning = NOT_RUNNING;  // Beispiel für die Aktualisierung des Status
        }
    }

    if (wpid == -1 && errno != ECHILD) {
        perror("waitpid");
    }
}


void disable_signals() {
    disable_signal(SIGINT, NULL ,SA_RESTART,1);
    disable_signal(SIGTTOU, NULL,0,1);
    disable_signal(SIGTSTP, NULL,0,1 );
}

int main(int argc, char *argv[], char **envp) {
    char * line = NULL;
    disable_signals();
    disable_signal(SIGCHLD, signal_handler, SA_RESTART, 0);
    shell_pid = getpid();
    setpgid(0, shell_pid);
    tcsetpgrp(fdtty, shell_pid);
    fdtty = open("/dev/tty", O_RDONLY|O_CLOEXEC);
    int print_commands=0;
    if (argc > 1){
        print_commands = strcmp(argv[1], "--print-commands") ==0 ? 1: 0; 
    }
#ifndef NOLIBREADLINE
    using_history();
    current_readline_prompt = malloc(1024);
#endif
    while (1) {
        int parser_res;
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) == NULL){
            perror("getcwd");
            cwd[0]='\0';
        }
#ifndef NOLIBREADLINE
        sprintf(current_readline_prompt, "bshell [%s]> ", cwd);
#else
        printf("bshell [%s]> ", cwd);
#endif
        fflush(stdout);
        parser_res=yyparse();
        if (parser_res == 0){
            if ((line=command_get(cmd)) != NULL){
#ifndef NOLIBREADLINE
                add_history(line);
#endif
                free(line);
            }
            if (print_commands ==1) {
                command_print(cmd);
            }
            execute(cmd);
            command_delete(cmd);
        } else if (parser_res == 1){
            fprintf(stderr, "[%s %s %i] Parser error: yyerror triggered parser_res=[1]!\n"
                    , __FILE__, __func__, __LINE__);
        } else if (parser_res == 2){
            fprintf(stderr, "[%s %s %i] Parser error: Invalid input! parser_res=[2]\n"
                    , __FILE__, __func__, __LINE__);
        } else {
            fprintf(stderr, "[%s %s %i] Parser memory or other error. parser_res=[%i]\n"
                    , __FILE__, __func__, __LINE__, parser_res);
        }
    }
}

