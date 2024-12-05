#include "LoadSettings.h"

#include "help.h"

void initHashTable(HashTable* table) {
	insert(table, "not", "not");
	insert(table, "input", "input");
	insert(table, "output", "output");
	insert(table, "add", "add");
	insert(table, "mult", "mult");
	insert(table, "sub", "sub");
	insert(table, "pow", "pow");
	insert(table, "div", "div");
	insert(table, "rem", "rem");
	insert(table, "xor", "xor");
	insert(table, "and", "and");
	insert(table, "or", "or");
	insert(table, "=", "=");
}

void printTable(HashTable* table) {
	printf("%s %s\n", "not", search(table, "not"));
	printf("%s %s\n", "input", search(table, "input"));
	printf("%s %s\n", "output", search(table, "output"));
	printf("%s %s\n", "add", search(table, "add"));
	printf("%s %s\n", "mult", search(table, "mult"));
	printf("%s %s\n", "sub", search(table, "sub"));
	printf("%s %s\n", "pow", search(table, "pow"));
	printf("%s %s\n", "div", search(table, "div"));
	printf("%s %s\n", "rem", search(table, "rem"));
	printf("%s %s\n", "xor", search(table, "xor"));
	printf("%s %s\n", "and", search(table, "and"));
	printf("%s %s\n", "or", search(table, "or"));
	printf("%s %s\n", "=", search(table, "="));
}

ResultSettings loadSettings(FILE* file, HashTable* table1,HashTable* table2, DirectionOfSaving* direction_of_saving,
                            DirectionOfExecution* direction_of_execution) {
	char* line;
	while (true) {
		line = readStringWithSpace(file);
		if (strlen(line) == 0) {
			break;
		}

		if (strstr(line, "[")) {
			if (strstr(line, "]")) {
				char* tmp = removeComment(line);
				strcpy(line, tmp);
				free(tmp);
			} else {
				char* tmp = removeComment(line);
				strcpy(line, tmp);
				free(tmp);
				char temp = fgetc(file);
				while (temp != ']' && temp != EOF) {
					temp = fgetc(file);
				}
				if (temp == EOF) {
					free(line);
					return WRONG_INPUT_DATA;
				}

				if (strlen(line) == 0) {
					free(line);
					continue;
				}
			}
		}
		if (strlen(line) == 1) {  // Обработка случая   []
			free(line);
			continue;
		}

		char* end = line + strlen(line) - 1;
		while (end >= line && isspace(*end)) {
			*end-- = '\0';
		}
		char* command = strtok(line, " ");
		if (command[0] == '#') {
			free(line);
			continue;
		}
		char* synonym = strtok(NULL, " ");
		if (!synonym || synonym[0] == '#') {
			if (strcmp(command, "left=") == 0) {
				*direction_of_saving = DLEFT;
				free(line);
				continue;
			}
			if (strcmp(command, "right=") == 0) {
				*direction_of_saving = DRIGHT;
				free(line);
				continue;
			}
			if (strcmp(command, "op()") == 0) {
				*direction_of_execution = ERIGHT;
				free(line);
				continue;
			}
			if (strcmp(command, "()op") == 0) {
				*direction_of_execution = ELEFT;
				free(line);
				continue;
			}
			if (strcmp(command, "(op)") == 0) {
				*direction_of_execution = EMIDDLE;
				free(line);
				continue;
			}
			free(line);
			return WRONG_INPUT_DATA;
		}

		char* tmp = strtok(NULL, " ");
		if ((tmp != NULL) && tmp[0] != '#') {
			free(line);
			return WRONG_INPUT_DATA;
		}

		if (search(table1, command) != NULL) {
			insert(table2, search(table1, command), ";;;");
			insert(table1, command, synonym);
			insert(table2, synonym, command);
		} else {
			free(line);
			return WRONG_INPUT_DATA;
		}
		free(line);
	}
	free(line);
	return SUCCESS;
}
