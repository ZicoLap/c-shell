#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "statuslist.h"

extern ProcessList *processList ;

Process *createNewProcess(pid_t pid, pid_t pgid, char **cmd)
{
    Process *pc = malloc(sizeof(Process));
    if (!pc)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    pc->pid = pid;
    pc->pgid = pgid;

    // Allocate memory for cmd
    size_t cmd_len = 0;
    char **temp_cmd = cmd;
    while (*temp_cmd)
    {
        cmd_len += strlen(*temp_cmd++) + 1; 
    }
    pc->cmd = malloc(cmd_len + 1); 
    if (!pc->cmd)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Copy the entire command to cmd
    char *cmd_ptr = pc->cmd;
    for (char **temp_cmd = cmd; *temp_cmd; ++temp_cmd)
    {
        cmd_ptr = stpcpy(cmd_ptr, *temp_cmd);
        if (*(temp_cmd + 1))
        {
            *cmd_ptr++ = ' ';
        }
    }
    *cmd_ptr = '\0';

    pc->status = 0;
    pc->isRunning = RUNNING;
    pc->isShown = NOT_SHOWN;
    return pc;
}

ProcessList *addProcessToList(Process *process, ProcessList *processList)
{
    ProcessList *newNode = malloc(sizeof(ProcessList));
    if (!newNode)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    newNode->head = process;
    newNode->next = processList;
    newNode->previous = NULL;

    if (processList)
    {
        processList->previous = newNode;
    }

    return newNode;
}

Process *recursiveFindPid(ProcessList *pl, pid_t pid) {
    if (pl == NULL) {
        return NULL;
    }

    Process *p = pl->head;
   
        if (p->pid == pid) {
            return p;
        }

    
    return recursiveFindPid(pl->next, pid);
}

Process *findProcess(pid_t pid, ProcessList *pl) {
    return recursiveFindPid(pl, pid);
}

ProcessList *deleteProcess(ProcessList *processList)
{
    ProcessList *current = processList;
    ProcessList *newProcessList = NULL;     
    ProcessList *newProcessListTail = NULL; 

    while (current != NULL)
    {
        ProcessList *next = current->next;

        
        if (current->head->isShown == SHOWN)
        {
            
            free(current->head->cmd);
            free(current->head);
            free(current);
        }
        else
        {
            
            if (newProcessList == NULL)
            {
                newProcessList = current;
                newProcessListTail = current;
                newProcessListTail->previous = NULL;
            }
            else
            {
                newProcessListTail->next = current;
                current->previous = newProcessListTail;
                newProcessListTail = current;
            }
            newProcessListTail->next = NULL;
        }

        current = next;
    }


    return newProcessList;
}

 char *getFirstWord(const char *cmd)
{
    char *cmdCopy = strdup(cmd);
    char *firstWord = strtok(cmdCopy, " ");
    char *result = strdup(firstWord);
    free(cmdCopy);
    return result;
}

void printProcessList(ProcessList* processList) {
    
    ProcessList* stack[1024]; 
    int top = -1;

    
    while (processList != NULL) {
        stack[++top] = processList;
        processList = processList->next;
    }

   
    printf("PID\tPGID\tSTATUS\tPROG\n");
    while (top >= 0) {
        ProcessList* current = stack[top--];
        Process* p = current->head;

        if (p->isRunning == NOT_RUNNING) {
            p->isShown = SHOWN;
        }

        if (p->isRunning == RUNNING) {
            printf("%d\t%d\t%s\t%s\n", p->pid, p->pgid, "running", getFirstWord(p->cmd));
        } else if (WIFEXITED(p->status)) {
            printf("%d\t%d\texit(%d)\t%s\n", p->pid, p->pgid, WEXITSTATUS(p->status), getFirstWord(p->cmd));
        } else if (WIFSIGNALED(p->status)) {
            printf("%d\t%d\tsignal(%d)\t%s\n", p->pid, p->pgid, WTERMSIG(p->status), getFirstWord(p->cmd));
        } else {
            printf("%d\t%d\t%d\t%s\n", p->pid, p->pgid, p->status, getFirstWord(p->cmd));
        }
    }
}

void updateList(pid_t pid, int status) {
    for (ProcessList* list = processList; list != NULL; list = list->next) {
        Process* p = list->head;
        if (p->pid == pid || p->pid == -1) {
            p->pid = pid;
            p->status = status;
            p->isRunning = NOT_RUNNING;
        }
    }
}

