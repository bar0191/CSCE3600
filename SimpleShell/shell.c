/*
 ============================================================================
 Name        : shell.c
 Author(s)   : Brandon Reid, Ravnit Benipal, Robert Torres
 Version     : v1
 Copyright   : 2016
 Description : Major Assignment 1 - Simplified Shell
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

// used to create a bool data type
typedef enum { false, true } bool;

// function to strip whitespaces
char *strstripWS(char *str)
{
	char *end = str + strlen(str) - 1;
    
	while (end > str && isspace(*end))
        *end-- = '\0';
    
	return str;
}

// main
int main(int argc, char *argv[])
{
	char cmdLine[512];          // command line
	char *cmds[50];             // each parsed cmd in line
	char cmd[512];              // copy of command line
	int  index = 0;             // index for array of cmds
	char history[50][512];		// array of cmdLines
	bool customPrompt = false;	// bool value for users prompt
    FILE* file;                 // FILE ptr
    
	// if argc is greater then 2, an error message prints to the user
	if (argc > 2)
	{
		fprintf(stderr, "error: too many arguments\n");
		exit(EXIT_FAILURE);
	}

	// batch mode
	if (argc == 2)
	{
		// bash file input needs to be done
		printf("*** batch mode ***\n\n");
        
		// Takes in an argument from the command line and stores it into fileName
        char const* const fileName = argv[1];
        printf("--> file: %s\n", fileName);
        
		// Error check: Sends an error message to the user if the file does not exist or cannot be located
        if((file = fopen(fileName, "r")) == NULL)
        {
            fprintf(stderr, "error: unable to locate file..\n");
            exit(EXIT_FAILURE);
        }
        
        // shell
        while (fgets(cmdLine, sizeof(cmdLine), file))
        {
            // trim trailing newline character
            if (cmdLine[strlen(cmdLine)-1] == '\n')
                cmdLine[strlen(cmdLine)-1] = '\0';
            
            // if enter is typed, re-prompt
            if(cmdLine[0] == '\n')
                continue;
            
            printf("batch line~$ %s\n\n", cmdLine);
            
            // copy cmdLine to history array
            strcpy(history[index], cmdLine);
            // make a copy of cmdLine before strsep()
            strcpy(cmd, cmdLine);
            index++; // increment index for history
            
            int i, j;           // forloop values
            int cmdCount = 0;	// count of cmds
            char *line = cmd;	// pointer to copy of cmdLine
            
            // parse cmdLine into array of commands
            for (i = 0; i < 50; i++)
            {
                // ';' is the delimiter
                cmds[i] = strsep(&line, ";");
                cmdCount++; // increment cmd count
                
                // once end of command line is met, break loop
                if (cmds[i] == NULL) break;
            }
            
            pid_t pids[cmdCount];
            int pidCount = 0;
            
            for (i = 0; i < cmdCount-1; i++)
            {
                // strip WS
                strstripWS(cmds[i]);
                
                // strip WS
                while(isspace((unsigned char)*cmds[i])) cmds[i]++;
                
                // check cmds for special strings
                if(strcmp(cmds[i], "quit") == 0) exit(EXIT_SUCCESS);
                if(strcmp(cmds[i], "exit") == 0) exit(EXIT_SUCCESS);
                
                // after checks, fork each cmd
                if((pids[pidCount] = fork()) < 0)
                {
                    perror("fork");	//Error check for fork command
                }
                else if (pids[pidCount] == 0)
                {
                    system(cmds[pidCount]);
                    exit(0);
                }
                
                pidCount++; // increment pidCount
            }
            
            // parent waiting for children
            int status = 0;
            pid_t pid;
            
            // while the child hasn't returned a ZERO, continue to wait
            while((pid = wait(&status)) > 0);
            fflush(stdin);
        }
        fclose(file);		//closes the file after use
		exit(EXIT_SUCCESS);	//exits the program
	}

	// interactive mode
	if (argc == 1)
	{
		char *prompt = "shell~$";	// default prompt
		char usersPrompt[25];		// users custom prompt

		printf("*** interactive mode ***\n");
		printf("--> type ""prompt"" at any time to change prompt \n");
		printf("--> type ""history"" at any time to display history \n");

		// shell
		while (1)
		{
			// get username for shell prompt
			char *euid = getenv("USER");
		
			// if user hasn't changed prompt
			if (customPrompt == false) // shell prompt
				printf("%s@%s ", euid, prompt);

			// if user changed prompt
			if(customPrompt == true) // shell prompt
				printf("%s@%s ", euid, usersPrompt);

			// fgets returns NULL on CTRL-D, so exit loop on NULL
			if(fgets(cmdLine, sizeof(cmdLine), stdin) == NULL) 
			{
				printf("\n");
				break;
			}

			// if enter is typed, re-prompt
			if(cmdLine[0] == '\n')
				continue;

			// trim trailing newline character
			if (cmdLine[strlen(cmdLine)-1] == '\n')
				cmdLine[strlen(cmdLine)-1] = '\0';

			// copy cmdLine to history array
			strcpy(history[index], cmdLine);
            index++; // increment index for history
            
			// make a copy of cmdLine before strsep()
			strcpy(cmd, cmdLine);
        
			int i, j;           // forloop values
			int cmdCount = 0;	// count of cmds
			char *line = cmd;	// pointer to copy of cmdLine

			// parse cmdLine into array of commands
			for (i = 0; i < 50; i++)
			{
				// ';' is the delimiter
				cmds[i] = strsep(&line, ";");
				cmdCount++; // increment cmd count
                
                // once end of command line is met, break loop
				if (cmds[i] == NULL) break;
			}
			
			//printf("command count: %d\n", cmdCount-1);
			char *directory;
			pid_t pids[cmdCount];
			int pidCount = 0;

			// loop through each cmds[i]
			for (i = 0; i < cmdCount-1; i++)
			{
				// strip WS
				strstripWS(cmds[i]);
                
                // strip WS
				while(isspace((unsigned char)*cmds[i]))
                    cmds[i]++;
                
				// check cmds for special strings
				if(strcmp(cmds[i], "quit") == 0) exit(EXIT_SUCCESS);
				if(strcmp(cmds[i], "exit") == 0) exit(EXIT_SUCCESS);

				// if cmd = history, print history
				if(strcmp(cmds[i], "history") == 0)
				{
					for(j = 0; j < index-1; j++)
						printf("%s\n", history[j]);
                    
                    cmds[i] = "";
				}

				// if cmd = prompt, prompt user for new prompt
				if(strcmp(cmds[i], "prompt") == 0) 
				{
					printf("enter new prompt: \n");
					fgets(usersPrompt, sizeof(usersPrompt), stdin);
					
					// trim any trailing new lines
					if (usersPrompt[strlen(usersPrompt)-1] == '\n')
						usersPrompt[strlen(usersPrompt)-1] = 0;

					// change bool value to true for new prompt
					customPrompt = true;
					break;
				}
			
				// if cmd = cd, change directory
				if (strstr(cmds[i], "cd") != NULL)
				{
					// directory will point to the third position in the cmds[i] string
                    // should be the directory, this might need error checks
					directory = cmds[i] + 3;
					chdir(directory);
                    break;
				}

				// after checks, fork each cmd
                if((pids[pidCount] = fork()) < 0)
                {
                    perror("error: fork");
                }
                else if (pids[pidCount] == 0)
                {
                    system(cmds[pidCount]);
                    exit(0);
                }
                pidCount++; // increment pidCount
			}
            
			// parent waiting for children
			int status = 0;
			pid_t pid;
            
			// while the child hasn't returned a ZERO, continue to wait
            while((pid = wait(&status)) > 0);
            fflush(stdin);
		}
	}
	return 0;
}

