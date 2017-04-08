#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runArgs(int argc, char *argv[]);

int main(int argc, char* argv[])
{
	runArgs(argc, argv);

	return 0; 
}

void runArgs(int argc, char *argv[])
{
	int   i;
	char  buffer[80];
	char *directory;
	char *outputFile;
	char *head;

	/*
	for (i = 0; i < 80; i++)
	{
		buffer[i] = '\0';
	}*/

	strcpy(buffer, "du -a");

	if (argv[1] == '\0')
	{
		printf("cmd: %s\n", buffer);
		system(buffer);
		return;
	}
	else if (argc >= 2 && argc <= 5)
	{
		for (i = 1; i < argc; i++)
		{
			if (strncmp("-d", argv[i], 2) == 0)
			{
				directory = argv[i] + 2;
				strcpy(buffer+(strlen(buffer)), " ");
				strcpy(buffer+strlen(buffer), directory);
			}
			else if (strcmp(argv[i], "-s") == 0)
			{
				strcpy(buffer+(strlen(buffer)), " | ");
				strcpy(buffer+strlen(buffer), "sort -n -r");
			}
			else if (strncmp("-h", argv[i], 2) == 0)
			{
				head = argv[i] + 2;
				strcpy(buffer+(strlen(buffer)), " | ");
				strcpy(buffer+strlen(buffer), "head -n ");
				strcpy(buffer+strlen(buffer), head);
			}
			else if (strncmp("-o", argv[i], 2) == 0)
			{
				outputFile = argv[i] + 2;
				strcpy(buffer+(strlen(buffer)), " > ");
				strcpy(buffer+strlen(buffer), outputFile);
			}
			else
			{
				printf("Unknown flag... Terminating program.\n");
				return;
			}
		}

		printf("cmd: %s\n", buffer);
		system(buffer);
	}
	else
	{
		printf("ERROR: Invalid amount of arguements entered..\n");
	}
	return;
}
