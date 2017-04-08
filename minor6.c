/*
 * Author      : Brandon Reid
 * Assignment  : minor 6
 * Date        : 10/29/16
 * Course      : CSCE 3600
 * Description : modeling command-line arguements using pipes with two children
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum { READ, WRITE };

int main(int argc, char *argv[])
{
	// check for correct argument count
	if (argc < 3 || argc > 3)
	{
		printf("error: too few or too many arguments: %d\n", argc);
		exit(1);
	}

	// take in char strings for system commands
	char *sysCmd1 = argv[1];
	char *sysCmd2 = argv[2];

	printf("executing: %s | %s\n", sysCmd1, sysCmd2);

	int fd[2];

	// create unnamed pipe	
	if (pipe(fd) == -1)
	{
		perror("error: pipe");
		exit(1);
	}

	// first child writting through pipe
	int pid = fork();
	switch (pid)
	{
		case -1: // error check
			perror("error: pid");
			exit(2);
		case 0:  // inside WRITE child
			// create dup fd for write to stdout
			dup2(fd[WRITE], fileno(stdout));
			close(fd[READ]);  // close prev fd
			close(fd[WRITE]);

			// create array of args for execvp
			char *args[2];
			args[0] = sysCmd1;
			args[1] = NULL;
			execvp(sysCmd1, args);
			exit(3); 
	}

	// second child reading through pipe
	int pid2 = fork();
	switch (pid2)
	{
		case -1: // error check
			perror("error: pid2");
			exit(2);
		case 0:  // inside READ child
			// create dup fd for read to stdin
			dup2(fd[READ], fileno(stdin));
			close(fd[READ]); // close prev fd
			close(fd[WRITE]);

			// create array of args for execvp
			char *args[2];
			args[0] = sysCmd2;
			args[1] = NULL;
			execvp(sysCmd2, args);
			exit(3);
	}

	// parent needs to close fd for children to talk to each other
	close(fd[READ]);
	close(fd[WRITE]);

	// wait for second child to be done reading
	int status;
	waitpid(pid2, &status, 0);

	printf("command line pipe completed\n");

	return 0;
}
