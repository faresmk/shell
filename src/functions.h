#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"

// Frees memory used by a command sequence
void freeseq(char ***seq);

// Frees memory used by a command line
void freecmd(struct cmdline *s);

// Quits the shell if the command is "quit" or "exit"
void quit(char **cmd);

// Redirects input and/or output according to the given file names
void redirect(char *in, char *out);

// Executes the given command
void execute(char **cmd);

// Executes a sequence of commands separated by pipes
void command_seq(struct cmdline *l , char **cmd, int i, int fd[2]);
void command_bg(struct cmdline *l, char **cmd);
void handler(int sig);
#endif /* FUNCTIONS_H */
