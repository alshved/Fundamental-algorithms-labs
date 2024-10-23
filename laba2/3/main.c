#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "KMP.h"
#include "errors.h"

ResultCode findSub(const char* nameFile, const char* substring, int number) {
	FILE* file = fopen(nameFile, "r");
	if (!file) {
		return FILE_OPEN_ERROR;
	}

	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return SEEK_ERROR;
	}

	long len = ftell(file);
	if (len == -1) {
		fclose(file);
		return SEEK_ERROR;
	}

	if (fseek(file, 0, SEEK_SET) != 0) {
		fclose(file);
		return SEEK_ERROR;
	}

	char* buffer = (char*)malloc(sizeof(char) * (len + 1));
	if (buffer == NULL) {
		fclose(file);
		return MEMORY_ALLOCATING_ERROR;
	}

	size_t read = fread(buffer, sizeof(char), len, file);
	if (read == 0) {
		free(buffer);
		fclose(file);
		return EMPTY_FILE;
	}

	buffer[read] = '\0';
	fclose(file);

	MatchResult* results = NULL;
	int count = 0;
	KMPErrors er = KMPSearch(&results, substring, buffer, &count);

	free(buffer);
	if (er == MEMORY_ERROR){
		return MEMORY_ALLOCATING_ERROR;
	}
	if (er == SUCCESSS) {
		printf("File number %d:\n", number + 1);
		for (int i = 0; i < count; i++) {
			printf("Pattern found at line: %d, index: %d\n", results[i].line, results[i].index);
		}
		free(results);
	}

	return SUCCESS;
}

void func(const char* substring, int count, ...) {
	if (substring == NULL || substring[0] == '\0') {
		printf("You write empty substring.\n");
		return;
	}

	va_list files;
	va_start(files, count);
	for (int i = 0; i < count; ++i) {
		char* nameFile = va_arg(files, char*);
		ResultCode res = findSub(nameFile, substring, i);
		switch (res) {
			case FILE_OPEN_ERROR:
				printf("Error open file.\n");
				break;

			case SEEK_ERROR:
				printf("Fseek error.\n");
				break;

			case MEMORY_ALLOCATING_ERROR:
				printf("Memory allocating error.\n");
				break;

			case EMPTY_FILE:
				printf("Errror read from file.\n");
				break;

			default:
				break;
		}
	}
	va_end(files);
}

int main() {
	char sub[] = "asd";
	func(sub, 3, "1.txt", "2.txt", "3.txt");
}