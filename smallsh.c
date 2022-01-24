/*
* Author: Colin Francis
* ONID: francico
* Title: Smallsh
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct command {
	char* pathName;
	char** argv;
	bool inputRedirect;  // <
	char* newInput;
	bool outputRedirect;  // >
	char* newOutput;
	bool backgroundProcess;  // &
};

char* getCommandLineInput(void) {
	char* userInput = NULL;
	char* inputBuffer = NULL;
	size_t length;

	printf(": ");
	getline(&inputBuffer, &length, stdin);
	*(inputBuffer + strlen(inputBuffer) - 1) = '\0';

	userInput = (char*)malloc((strlen(inputBuffer) + 1) * sizeof(char));
	strcpy(userInput, inputBuffer);

	return userInput;
}

void initializeCommandStruct(struct command* command, int numArgs) {
	command->inputRedirect = false;
	command->newInput = NULL;

	command->outputRedirect = false;
	command->newOutput = NULL;

	command->backgroundProcess = false;

	command->argv = (char**)malloc(numArgs * sizeof(char*));
	for (int index = 0; index < numArgs; index++) {
		command->argv[index] = NULL;
	}
}

char** appendArg(char* arg, char* argv[], int numArgs, int argvIndex) {
	int index = 0;
	char** newArgv = (char**)malloc((numArgs + 1) * sizeof(char*));

	argv[argvIndex] = (char*)malloc((strlen(arg) + 1) * sizeof(char));
	strcpy(argv[argvIndex], arg);

	while (index <= numArgs) {
		newArgv[index] = argv[index];
		index++;
	}
	newArgv[index] = NULL;

	free(argv);
	return newArgv;
}

struct command* parseUserInput(char* userInput) {
	int numArgs = 2;
	int argvIndex = 0;
	char* token = NULL;
	char* savePtr = NULL;
	struct command* command = (struct command*)malloc(sizeof(struct command));

	initializeCommandStruct(command, numArgs);

	token = strtok_r(userInput, " ", &savePtr);
	if (!token || (strcmp(token, "#") == 0)) {
		return NULL;
	}
	command->pathName = (char*)malloc((strlen(token) + strlen("/bin/") + 1) * sizeof(char));
	strcpy(command->pathName, "/bin/");
	strcat(command->pathName, token);

	command->argv[argvIndex] = command->pathName;
	argvIndex++;

	token = strtok_r(NULL, " ", &savePtr);
	while (token) {
		if (strcmp(token, "<") == 0) {
			command->inputRedirect = true;
		}
		else if (strcmp(token, ">") == 0) {
			command->outputRedirect = true;
		}
		else if (strcmp(token, "&") == 0) {
			command->backgroundProcess = true;
		}

		command->argv = appendArg(token, command->argv, numArgs, argvIndex);
		argvIndex++;
		numArgs++;

		token = strtok_r(NULL, " ", &savePtr);
	}

	for (int index = 0; index < numArgs; index++) {
		printf("%s ", command->argv[index]);
	}
	printf("\n");

	return command;
}

int main(void) {
	char* userInput = NULL;
	struct command* command = NULL;

	while (true) {
		userInput = getCommandLineInput();
		command = parseUserInput(userInput);
		if (!command) {
			continue;
		}
	}

	return EXIT_SUCCESS;
}
