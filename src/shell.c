/*
 * Copyright (C) 2002, Simon Nieuviarts
 */


#include "functions.h"


int main()
{
	//signal handler for background jobs
	signal(SIGCHLD, handler);
	while (1) {
		struct cmdline *l;
		int i, j;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */

		if (!l) { // if l is null
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);


		/* Display each command of the pipe */	
		//stoke the output of the pervious command in the pipe
		int fd[2];
		pipe(fd);
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j=0; cmd[j]!=0; j++) {
				printf("%s ", cmd[j]);
			}
			printf("\n");
			//exit shell 
			quit(cmd);

			if (l->bg) { //if command is to be executed in background
    		command_bg(l, cmd); //execute command in background
    		continue; //go back to reading command
			}
			
			//execute the command
			command_seq(l, cmd, i, fd);
			
		}
	}
}