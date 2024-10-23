#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "string.h"

int Atoi(char* str) {
	int result = 0;
	int sign = 1;
	if (*str == '-') {
		sign = -1;
		str++;
	}
	while (*str) {
		result = result * 10 + (*str - '0');
		str++;
	}
	return result * sign;
}

bool parse_int_is_possible(char* str) {
	int i = 0;
	if (str[0] == '-') {
		i++;
	}
	for (; str[i] != '\0'; ++i) {
		if (str[i] < '0' || str[i] > '9') {
			return false;
		}
	}
	return true;
}

StringResult ParseArguments(int argc, char** argv, String* parsed_args) {
	if (argc < 3) {
		return MEMORY_ALLOCATING_ERROR;
	}

	for (int i = 2; i < argc; i++) {
		StringResult result = StringInit(&parsed_args[i - 2], 10);
		if (result != SUCCESS) {
			for (int j = 0; j < i - 2; j++) {
				StringFree(&parsed_args[j]);
			}
			return MEMORY_ALLOCATING_ERROR;
		}

		result = StringSet(&parsed_args[i - 2], argv[i]);
		if (result != SUCCESS) {
			StringFree(&parsed_args[i - 2]);
			for (int j = 0; j < i - 2; j++) {
				StringFree(&parsed_args[j]);
			}
			return MEMORY_ALLOCATING_ERROR;
		}
	}
	return SUCCESS;
}

ParseResult GetOpts(int argc, char** argv, Opts* option, String* words, int* count) {
	if (argc < 3) {
		return FEW_ARGUMENTS;
	}
	if (argv[1][0] != '-' || argv[1][2] != '\0') {
		return BAD_FLAG;
	}

	switch (argv[1][1]) {
		case 'l':
		case 'r':
		case 'u':
		case 'n':
			if (argc != 3) {
				return A_LOT_OF_ARGUMENTS;
			}
			*count = 1;

			*option = (argv[1][1] == 'l') ? OPT_L : (argv[1][1] == 'r') ? OPT_R : (argv[1][1] == 'u') ? OPT_U : OPT_N;

			if (ParseArguments(argc, argv, words) != SUCCESS) {
				return MEMORY_ALLOCATING_ERROR;
			}
			break;

		case 'c':
			*option = OPT_C;
            if (argc < 4){
                return FEW_ARGUMENTS;
            }
			if (ParseArguments(argc, argv, words) != SUCCESS) {
				return MEMORY_ALLOCATING_ERROR;
			}
			*count = argc - 2;
			if (!parse_int_is_possible(words[0].data)) {
				for (int j = 0; j < *count; j++) {
					StringFree(&words[j]);
				}
				return BAD_ARGUMENTS;
			}
			break;

		default:
			return BAD_FLAG;
	}
	return SUCCESS;
}

void HandlerOptL(String* words, int len) { printf("Длина строки: %d\n", StringLength(&words[0])); }

void HandlerOptR(String* words, int len) {
	printf("Перевёрнутая строка :\n\t");
	StringReverse(&words[0]);
	StringPrint(&words[0]);
}

void HandlerOptU(String* words, int len) {
	for (int i = 1; i < StringLength(&words[0]); i += 2) {
		if (words[0].data[i] >= 'a' && words[0].data[i] <= 'z') {
			words[0].data[i] -= 32;
		}
	}
	printf("Новая строка: \n");
	StringPrint(&words[0]);
}

void HandlerOptN(String* words, int len) {
	int i, digits_index = 0, letters_index = 0, others_index = 0;
	int length = StringLength(&words[0]);
	String digit, letters, others;

	if (StringInit(&digit, length + 1) != SUCCESS || StringInit(&letters, length + 1) != SUCCESS ||
	    StringInit(&others, length + 1) != SUCCESS) {
		printf("String initialization error.\n");
		return;
	}

	for (i = 0; i < length; i++) {
		if (words[0].data[i] >= '0' && words[0].data[i] <= '9') {
			digit.data[digits_index++] = words[0].data[i];
		} else if ((words[0].data[i] >= 'A' && words[0].data[i] <= 'Z') ||
		           (words[0].data[i] >= 'a' && words[0].data[i] <= 'z')) {
			letters.data[letters_index++] = words[0].data[i];
		} else {
			others.data[others_index++] = words[0].data[i];
		}
	}

	digit.data[digits_index] = '\0';
	letters.data[letters_index] = '\0';
	others.data[others_index] = '\0';

	StringConcat(&digit, letters.data);
	StringConcat(&digit, others.data);

	printf("Результат:\n");
	StringPrint(&digit);

	StringFree(&digit);
	StringFree(&letters);
	StringFree(&others);
}

void shuffle(int* array, int n) {
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

void HandlerOptC(String* words, int len) {
	if (words == NULL || len <= 0) {
		printf("Invalid arguments.\n");
		return;
	}

	int seed = Atoi(words[0].data);
	srand(seed);

	int* newOrder = malloc(sizeof(int) * (len - 1));
	if (newOrder == NULL) {
		printf("Memory allocating error.\n");
		return;
	}

	for (int i = 0; i < len - 1; ++i) {
		newOrder[i] = i + 1;
	}

	shuffle(newOrder, len - 1);

	String result;
	if (StringInit(&result, 10) != SUCCESS) {
		printf("String initialization error.\n");
		free(newOrder);
		return;
	}

	if (StringSet(&result, words[newOrder[0]].data) != SUCCESS) {
		printf("String set error.\n");
		StringFree(&result);
		free(newOrder);
		return;
	}

	for (int i = 1; i < len - 1; ++i) {
		if (StringConcat(&result, words[newOrder[i]].data) != SUCCESS) {
			printf("String concat error.\n");
			StringFree(&result);
			free(newOrder);
			return;
		}
	}
	free(newOrder);
	StringPrint(&result);
	StringFree(&result);
}

int main(int argc, char** argv) {
	Opts opt = 0;
	int count = 0;
	if (argc <= 2) {
		printf("Few arguments.\n");
		return 1;
	}
	String words[argc - 2];
	void (*handlers[5])(String*, int) = {HandlerOptL, HandlerOptR, HandlerOptU, HandlerOptN, HandlerOptC};

	ParseResult res = GetOpts(argc, argv, &opt, words, &count);

	if (res == FEW_ARGUMENTS) {
		printf("Few arguments.\n");
		return 1;
	} else if (res == MEMORY_ALLOCATING_ERROR) {
		printf("Memory allocating error.\n");
		return 1;
	} else if (res == BAD_ARGUMENTS) {
		printf("Bad arguments.\n");
		for (int i = 0; i < count; i++) {
			StringFree(&words[i]);
		}
		return 1;
	} else if (res == BAD_FLAG) {
		printf("Bad flag.\n");
		return 1;
	} else if (res == A_LOT_OF_ARGUMENTS) {
		printf("A lot of arguments.\n");
		return 1;
	}

	handlers[opt](words, count);

	for (int i = 0; i < count; ++i) {
		StringFree(&words[i]);
	}

	return 0;
}