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
#include "debug.h"
#include "execute.h"

/* do not modify this */
#ifndef NOLIBREADLINE
#include <readline/history.h>
#endif /* NOLIBREADLINE */

extern int shell_pid;
extern int fdtty;

//////////////////////////////////////////////////////////////////////////////////////////////////
///////// TODO : our Variables
ProcessList *processList;
////////////////////////////////////////////////////////////////////////////////////////////////

/* do not modify this */
#ifndef NOLIBREADLINE
static int builtin_hist(char **command)
{

    register HIST_ENTRY **the_list;
    register int i;
    printf("--- History --- \n");

    the_list = history_list();
    if (the_list)
        for (i = 0; the_list[i]; i++)
            printf("%d: %s\n", i + history_base, the_list[i]->line);
    else
    {
        printf("history could not be found!\n");
    }

    printf("--------------- \n");
    return 0;
}
#endif /*NOLIBREADLINE*/
void unquote(char *s)
{
    if (s != NULL)
    {
        if (s[0] == '"' && s[strlen(s) - 1] == '"')
        {
            char *buffer = calloc(sizeof(char), strlen(s) + 1);
            strcpy(buffer, s);
            strncpy(s, buffer + 1, strlen(buffer) - 2);
            s[strlen(s) - 2] = '\0';
            free(buffer);
        }
    }
}

void unquote_command_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i] != NULL)
    {
        unquote(tokens[i]);
        i++;
    }
}

void unquote_redirect_filenames(List *redirections)
{
    List *lst = redirections;
    while (lst != NULL)
    {
        Redirection *redirection = (Redirection *)lst->head;
        if (redirection->r_type == R_FILE)
        {
            unquote(redirection->u.r_file);
        }
        lst = lst->tail;
    }
}

void unquote_command(Command *cmd)
{
    List *lst = NULL;
    switch (cmd->command_type)
    {
    case C_SIMPLE:
    case C_OR:
    case C_AND:
    case C_PIPE:
    case C_SEQUENCE:
        lst = cmd->command_sequence->command_list;
        while (lst != NULL)
        {
            SimpleCommand *cmd_s = (SimpleCommand *)lst->head;
            unquote_command_tokens(cmd_s->command_tokens);
            unquote_redirect_filenames(cmd_s->redirections);
            lst = lst->tail;
        }
        break;
    case C_EMPTY:
    default:
        break;
    }
}

static int execute_fork(SimpleCommand *cmd_s, int background)
{
    char **command = cmd_s->command_tokens;
    pid_t pid, wpid;
    Process *process = createNewProcess(-1, -1, cmd_s->command_tokens);
    processList = addProcessToList(process, processList);

    pid = fork();
    if (pid == 0)
    {
        /* child */
        signal(SIGINT, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        /*
         * handle redirections here
         */
        if (cmd_s->redirections != NULL)
        {
            // printf("Handling of redirection is still missing ... implement it!\n");
            // exit(0);
            handleRedirection(cmd_s->redirections);
        }
        if (execvp(command[0], command) == -1)
        {
            fprintf(stderr, "-bshell: %s : command not found \n", command[0]);
            perror("");
        }
        /*exec only return on error*/
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("shell");
    }
    else
    {
        /*parent*/
        setpgid(pid, pid);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// TODO :  set new process
        //Process *process = newProcess(pid, pid, cmd_s->command_tokens);
        process->pid = pid;
        process->pgid = pid;
        ///////////////////////////////////////////////////////////////////////////////////////

        if (background == 0)
        {
            int status;
            /* wait only if no background process */
            tcsetpgrp(fdtty, pid);

            /**
             * the handling here is far more complicated than this!
             * vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
             */

            wpid = waitpid(pid, &status, 0);

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^

            process->isRunning = NOT_RUNNING;
            process->status = status;

            tcsetpgrp(fdtty, shell_pid);
            return status;
        }
        else
        {
        
            printf("PID=%d  PGID=%d\n", process->pid, process->pgid);
        }
    }

    return 0;
}

static int do_execute_simple(SimpleCommand *cmd_s, int background)
{
    if (cmd_s == NULL)
    {
        return 0;
    }
    if (strcmp(cmd_s->command_tokens[0], "false") == 0)
    {
        return 0;
    }
    if (strcmp(cmd_s->command_tokens[0], "true") == 0)
    {
        return 1;
    }

    if (strcmp(cmd_s->command_tokens[0], "exit") == 0)
    {
        // exit(0);
        handleExitCommand(cmd_s->command_tokens);

/* do not modify this */
#ifndef NOLIBREADLINE
    }
    else if (strcmp(cmd_s->command_tokens[0], "hist") == 0)
    {
        return builtin_hist(cmd_s->command_tokens);
#endif /* NOLIBREADLINE */
    }
    else if (strcmp(cmd_s->command_tokens[0], "cd") == 0)
    {
        return handleCdCommand(cmd_s->command_tokens);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO : added status comand
    else if (strcmp(cmd_s->command_tokens[0], "status") == 0)
    {
        printProcessList(processList);
        processList = deleteProcess(processList);
        return 0;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        return execute_fork(cmd_s, background);
    }
    fprintf(stderr, "This should never happen!\n");
    exit(1);
}

/*
 * check if the command is to be executed in back- or foreground.
 *
 * For sequences, the '&' sign of the last command in the
 * sequence is checked.
 *
 * returns:
 *      0 in case of foreground execution
 *      1 in case of background execution
 *
 */
int check_background_execution(Command *cmd)
{
    List *lst = NULL;
    int background = 0;
    switch (cmd->command_type)
    {
    case C_SIMPLE:
        lst = cmd->command_sequence->command_list;
        background = ((SimpleCommand *)lst->head)->background;
        break;
    case C_OR:
    case C_AND:
    case C_PIPE:
    case C_SEQUENCE:
        /*
         * last command in sequence defines whether background or
         * forground execution is specified.
         */
        lst = cmd->command_sequence->command_list;
        while (lst != NULL)
        {
            background = ((SimpleCommand *)lst->head)->background;
            lst = lst->tail;
        }
        break;
    case C_EMPTY:
    default:
        break;
    }
    return background;
}

int execute(Command *cmd)
{
    unquote_command(cmd);

    int res = 0;
    List *lst = NULL;

    int execute_in_background = check_background_execution(cmd);
    switch (cmd->command_type)
    {
    case C_EMPTY:
        break;
    case C_SIMPLE:
        res = do_execute_simple((SimpleCommand *)cmd->command_sequence->command_list->head, execute_in_background);
        fflush(stderr);
        break;

    case C_OR:
        res = executeOr(cmd->command_sequence->command_list, execute_in_background);
        break;
    case C_AND:
        res = executeAnd(cmd->command_sequence->command_list, execute_in_background);
        break;
    case C_SEQUENCE:
        res = executeSequence(cmd->command_sequence->command_list, execute_in_background);
        break;
    case C_PIPE:
        // printf("[%s] PIPES are not yet implemented ... do it ... \n", __func__);
        res = executePipe(cmd, execute_in_background);
        break;
    default:
        printf("[%s] unhandled command type [%i]\n", __func__, cmd->command_type);
        break;
    }
    return res;
}

///////////////////////////////////////////////////////////////////////////////////

/*
3. Terminieren der Shell: Syntax: exit [ Exit-Wert ]
◦ Wenn kein Exit-Wert angegeben ist, soll die Shell mit exit(0) terminieren.
*/
void handleExitCommand(char **tokens)
{
    if (tokens[1] == NULL)
    {
        exit(0);
    }
    else
    {
        char *endptr;
        int exit_code = strtol(tokens[1], &endptr, 10);
        if (*endptr != '\0')
        {
            fprintf(stderr, "basicsh: exit: invalid exit code: %s\n", tokens[1]);
            exit(1);
        }
        exit(exit_code);
    }
}

/*
4. Verzeichnis wechseln: Syntax: cd [ Dateipfad ]
  Die Shell ändert ihr aktuelles Verzeichnis. Bei fehlendem Pfad wird das Login-Verzeichnis
  des Benutzers verwendet. (Systemaufruf chdir() )
*/

int handleCdCommand(char **tokens)
{
    const char *dir = (tokens[1] == NULL) ? getenv("HOME") : tokens[1];

    if (dir == NULL)
    {
        fprintf(stderr, "basicsh: cd: HOME not set\n");
        return 1;
    }

    if (chdir(dir) != 0)
    {
        perror("basicsh: cd");
        return 1;
    }

    return 0;
}

/*
5. Umlenkungen der Ein- und/oder Ausgabe
    a. Ausgabeumlenkung Syntax: Programmaufruf > Dateipfad
        ▪ Die Standardausgabe des Programms wird in die angegebene Datei umgelenkt. Diese
        wird bei Bedarf erzeugt. Falls sie schon vorhanden ist, wird der alte Inhalt gelöscht.
    b. Ausgabeumlenkung mit Anfügen: Syntax: Programmaufruf >> Dateipfad
        ▪ Falls die Datei schon vorhanden ist, wird der neue Inhalt hinter den alten geschrieben.
    c. Eingabeumlenkung Syntax: Programmaufruf < Dateipfad
        ▪ Die Datei wird zur Standardeingabe des Programms.

*/

void handleRedirection(List *redirections)
{
    int fileDescriptor;
    
    while (redirections != NULL)
    {
        Redirection *redirection = (Redirection *)redirections->head;
        
        if (redirection->u.r_file != NULL)
        {
            switch (redirection->r_mode)
            {
                case M_READ:
                    fileDescriptor = open(redirection->u.r_file, O_RDONLY);
                    if (fileDescriptor == -1)
                    {
                        perror(redirection->u.r_file);
                        exit(EXIT_FAILURE);
                    }
                    if (dup2(fileDescriptor, STDIN_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    break;

                case M_APPEND:
                    fileDescriptor = open(redirection->u.r_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fileDescriptor == -1)
                    {
                        perror(redirection->u.r_file);
                        exit(EXIT_FAILURE);
                    }
                    if (dup2(fileDescriptor, STDOUT_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    break;

                case M_WRITE:
                    fileDescriptor = open(redirection->u.r_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fileDescriptor == -1)
                    {
                        perror(redirection->u.r_file);
                        exit(EXIT_FAILURE);
                    }
                    if (dup2(fileDescriptor, STDOUT_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    break;

                default:
                    fprintf(stderr, "Unknown redirection mode: %d\n", redirection->r_mode);
                    exit(EXIT_FAILURE);
            }

            close(fileDescriptor);
        }

        redirections = redirections->tail;
    }
}


/*
    6. Sequenz Syntax: Programmaufruf ; Programmaufruf ; . . . ; Programmaufruf
    ◦ Die Programme werden in Shell-Subprozessen nacheinander ausgeführt.
*/

int executeSequence(List *commands, int background)
{
    int res = 0;
    while (commands != NULL)
    {

        SimpleCommand *command = (SimpleCommand *)commands->head;
        res = do_execute_simple(command, background);
        
        fflush(stderr);
        commands = commands->tail;
    }
    return res;
}

/*
    7. Ausführung bei Erfolg: Syntax: Programmaufruf && Programmaufruf && . . . && Programmaufruf
    ◦ Wie bei Sequenz, aber Abbruch, falls ein Programm nicht mit exit(0) terminiert.
*/

int executeAnd(List *commands, int background)
{
    int res = 0;
    while (commands != NULL)
    {

        SimpleCommand *command = (SimpleCommand *)commands->head;
        res = do_execute_simple(command, background);

        fflush(stderr);
        if (res == 0)
        {
            break;
        }
        commands = commands->tail;
    }
    return res;
}

/*
    8. Ausführung bei Misserfolg: Syntax: Programmaufruf || Programmaufruf || ... || Programmaufruf
    ◦ Wie bei Sequenz, aber Abbruch, falls ein Programm mit exit(0) terminiert.
*/

int executeOr(List *commands, int background)
{
    int res = 0;
    while (commands != NULL)
    {

        SimpleCommand *command = (SimpleCommand *)commands->head;
        res = do_execute_simple(command, background);

        fflush(stderr);
        if (res != 0)
        {
            break;
        }
        else
        {
            commands = commands->tail;
        }
    }
    return res;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static int executePipe(Command *cmd, int background)
{
    List *lst = cmd->command_sequence->command_list;
    int numCommands = 0;
    pid_t wpid;
    for (List *tmp = lst; tmp != NULL; tmp = tmp->tail)
    {
        numCommands++;
    }
    // da für jede Verbindung zwischen zwei Befehlen zwei Deskriptoren benötigt wird
    // (Lesen und   Screiben)
    int pfd[2 * (numCommands - 1)];
    for (int i = 0; i < numCommands - 1; i++)
    {
        if (pipe(pfd + 2 * i) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int commandIndex = 0;
    pid_t pids[numCommands];
    for (List *tmp = lst; tmp != NULL; tmp = tmp->tail)
    {
        SimpleCommand *cmd_s = (SimpleCommand *)tmp->head;

        pid_t pid = fork();
        if (pid == 0)
        {

            // Handlung von SIGINT (Ctrl+C)

            signal(SIGINT, SIG_DFL);

            if (commandIndex > 0)
            {
                if (dup2(pfd[2 * (commandIndex - 1)], STDIN_FILENO) == -1)
                {
                    perror("dup2 stdin");
                    exit(EXIT_FAILURE);
                }
            }
            if (commandIndex < numCommands - 1)
            {
                if (dup2(pfd[2 * commandIndex + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 stdout");
                    exit(EXIT_FAILURE);
                }
            }
            for (int i = 0; i < 2 * (numCommands - 1); i++)
            {
                close(pfd[i]);
            }

            if (strcmp(cmd_s->command_tokens[0], "exit") == 0)
            {
                handleExitCommand(cmd_s->command_tokens);
            }

            execvp(cmd_s->command_tokens[0], cmd_s->command_tokens);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            pids[commandIndex] = pid;

            // Erstellen des Process-Strukturs mit der korrekten pid und pgid
            processList = addProcessToList( createNewProcess(pid, pids[0], cmd_s->command_tokens), processList); 
        }

        commandIndex++;
    }

    for (int i = 0; i < 2 * (numCommands - 1); i++)
    {
        close(pfd[i]);
    }

    signal(SIGINT, SIG_IGN);

    for (int i = 0; i < numCommands; i++)
    {
        int status;
        wpid = waitpid(pids[i], &status, 0);
        updateList(wpid, status);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {

            kill(0, SIGINT);
        }
    }

    signal(SIGINT, SIG_DFL);

    return 0;
}
