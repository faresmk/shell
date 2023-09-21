#include "functions.h"

 void freeseq(char ***seq)
{
	int i, j;

	for (i=0; seq[i]!=0; i++) {
		char **cmd = seq[i];

		for (j=0; cmd[j]!=0; j++) free(cmd[j]);
		free(cmd);
	}
	free(seq);
}

 void freecmd(struct cmdline *s)
{
	if (s->in) free(s->in);
	if (s->out) free(s->out);
	if (s->seq) freeseq(s->seq);
}


//exit the shell
void quit(char **cmd){
	if ((strcmp(cmd[0], "quit")&&strcmp(cmd[0], "exit")) == 0)
				exit(0);
}
	

// redirect the input and output
void redirect(char *in, char *out){ 
	if(in){
		//check the permission of the file
		if (access(in, R_OK) == -1) {
			printf("Permission denied\n");
			exit(0);
		}
		int fd0 = open(in, O_RDONLY, 0);
		dup2(fd0, 0);
		close(fd0);
	}
	if(out){
		//check the permission of the file
		if (access(out, W_OK) == -1) {
			printf("Permission denied\n");
			exit(0);
		}
		int fd1 = open(out, O_WRONLY, 0);
		dup2(fd1, 1);
		close(fd1);
	}
}

//execute command 
void execute(char **cmd){
	
		if (execvp(cmd[0], cmd) == -1) { //check if the command is in the path
			printf("%s : Command not found\n",cmd[0]);
			exit(EXIT_FAILURE); //exit failure
		}
}

//execute the sequance of commands using pipe
void command_seq(struct cmdline *l , char **cmd, int i, int fd[2]){
	pid_t pid; 
			int status;
			int fd2[2];
			pipe(fd2);

			if((pid=fork())==0){
				//redirect the input and output
				redirect(l->in, l->out);
				pid_t pid3;
				if(l->seq[i+1]!=0){ //if there is a next command
					dup2(fd2[1], 1); //redirect the output to the pipe
					close(fd2[0]);
					close(fd2[1]);
					
				}
				if(i!=0){ //if it is not the first command
					dup2(fd[0], 0); //redirect the input to the pipe
					close(fd[0]);
					close(fd[1]);
				}
				
				if((pid3=fork())==0){
					//redirect(l->in, l->out);
					execute(cmd);
				}
				else{
					waitpid(pid3,&status,0);
					exit(0);
				}
	
			}
			else{
				close(fd[0]);
				close(fd[1]);
				fd[0]=fd2[0];
				fd[1]=fd2[1];
				waitpid(pid,&status,0);
			}
}



void command_bg(struct cmdline *l, char **cmd) {
    pid_t pid;
    int status;

    if ((pid = fork()) == 0) {
        redirect(l->in, l->out);
        execute(cmd);
        exit(0);
    } else {
		waitpid(pid, &status, WNOHANG);
        printf("Background job started with pid %d\n", pid);
    }
}

void handler(int sig){
	int status;
	pid_t pid;
	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		if (WIFEXITED(status)) {
			printf("Background job with pid %d exited normally\n", pid);
		} else if (WIFSIGNALED(status)) {
			printf("Background job with pid %d was killed by signal %d\n", pid, WTERMSIG(status));
		} else if (WIFSTOPPED(status)) {
			printf("Background job with pid %d was stopped by signal %d\n", pid, WSTOPSIG(status));
		}
	}
}
