/* Karen wants to speak to the task manager */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Tasks {
	char *uid;
	int  pid;
	int  ppid;
	int  c;
	char *stime;
	char *tty;
	char *time;
	char *cmd;
} Task;

typedef Task LINKED_LIST_TYPE;
#include "LinkedList.h"

int rawString2Int(char *str) {
	// get number of char in str
	int n = sizeof(str) / sizeof(char);
	// declare a new string
	char *newstring;
	// filter out every space but copy the rest into the new string
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (str[i] != ' ') {
			newstring[index] = str[i];
			index++;
		}
	}

	// return the generated string converted to an int
	return atoi(newstring);
}

char* splitStringByIndex(char string[], size_t start_index, size_t stop_index) {
	// length of the char to be generated
	size_t size = stop_index - start_index;
	// allocate memory
	char *substring = calloc(size, sizeof(char));
	// go char to char
	for (int i = 0; i < size; i++) {
		substring[i] = string[start_index + i];
	}
	// return pointer to substring
	return substring;
}

Task* taskFromLine(char line[255]) {
	// allocate new memory for task
	Task *task 	= malloc(sizeof(Task));
	if (task == NULL) {
		fprintf(stderr, "Memory allocation for Task failed. Exiting\n");
		exit(EXIT_FAILURE);
	}
	// UID :	0 	-> 11
	task->uid 	= splitStringByIndex(line, 0, 11);
	// PID :	11	-> 16
	task->pid 	= rawString2Int(splitStringByIndex(line, 11, 16));
	// PPID :	16	-> 22
	task->ppid 	= rawString2Int(splitStringByIndex(line, 16, 22));
	// C :		22	-> 24
	task->c 	= rawString2Int(splitStringByIndex(line, 22, 24));
	// STIME :	24	-> 30
	task->stime = splitStringByIndex(line, 24, 30);
	// TTY :	30	-> 43
	task->tty 	= splitStringByIndex(line, 30, 43);
	// TIME :	43	-> 48
	task->time 	= splitStringByIndex(line, 43, 48);
	// CMD :	48	-> end
	int lineLength = 254; // (sizeof(line) / sizeof(char));
	task->cmd 	= splitStringByIndex(line, 43, lineLength);
	// return pointer to task
	return task;
}

LinkedList* listAllTasks() {
	FILE *fp;
	char line[255];
	// alloc mem for LinkedList
	LinkedList *list = malloc(sizeof(LinkedList));
	if (list == NULL) {
		fprintf(stderr, "Memory allocation for LinkedList failed. Exiting\n");
		exit(EXIT_FAILURE);
	}
	// alloc mem for Task
	Task *baseTask = malloc(sizeof(Task));
	if (baseTask == NULL) {
		fprintf(stderr, "Memory allocation for Task failed. Exiting\n");
		exit(EXIT_FAILURE);
	}
	// initialize LinkedList
	initListAtValue(list, *baseTask);

	// Open the Command for reading
	fp = popen("ps -ef", "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to run command \"ps -ef\"\n");
		exit(EXIT_FAILURE);
	}

	// Read the output one line at the time
	while (fgets(line, sizeof(line)-1, fp) != NULL) {
		Task *lineTask = taskFromLine(line);
		addValueAtEnd(list, *lineTask);
	}

	// close
	pclose(fp);

	// return pointer to list
	return list;
}
