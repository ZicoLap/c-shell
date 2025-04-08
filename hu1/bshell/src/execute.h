#ifndef EXECUTE_H
#define EXECUTE_H

int execute(Command *);

void handleExitCommand(char **tokens);
int handleCdCommand(char **tokens);
void handleRedirection(List *redirections);

int executeOr(List *commands, int background);
int executeAnd(List *commands, int background);
int executeSequence(List *commands, int background);
static int executePipe(Command *cmd, int background);

#endif /* EXECUTE_H */
