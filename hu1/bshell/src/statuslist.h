#ifndef STATUSLIST_H

#define STATUSLIST_H
#include <sys/types.h>

typedef enum {
    NOT_RUNNING = 0,
    RUNNING = 1
} RunningStatus;

typedef enum {
    NOT_SHOWN = 0,
    SHOWN = 1
} ShowStatus;


typedef struct {
    pid_t pid;
    pid_t pgid;
    RunningStatus isRunning; // Use RunningStatus enum
    int status; // Keep status as int
    ShowStatus isShown; // Use ShowStatus enum
    char * cmd;
} Process;

typedef struct processList
{
    Process *head;
    struct processList *next;
    struct processList *previous;
} ProcessList;

Process *createNewProcess(pid_t pid, pid_t pgid, char **cmd);
ProcessList *addProcessToList(Process *process, ProcessList *processList);
Process *findProcess(pid_t pid, ProcessList *pl);
ProcessList *deleteProcess(ProcessList *processList);
void printProcessList(ProcessList *processList);
void updateList(pid_t pid, int status);

ProcessList *deleteProcessHelper(ProcessList *processList, ProcessList **newProcessList);


#endif /* end of include guard: STATUSLIST_H */
