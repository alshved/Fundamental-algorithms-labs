#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Deikstra/Deikstra.h"
#include "PrefixTree.h"
#include "Settings/LoadSettings.h"
#include "Settings/hashTable.h"
#include "Settings/help.h"

bool validateBase(unsigned long base) {
	if (base < 2 || base > 36) {
		return false;
	}
	return true;
}

char* uintToString(unsigned long long num, int base) {
	if (base < 2 || base > 36) {
		return NULL;  // Неверная система счисления
	}

	char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";  // Цифры и буквы для систем счисления до 36
	char* str = (char*)malloc(65);  // Достаточно для unsigned long long (64-бит) в базе 2
	if (str == NULL) {
		return NULL;
	}
	str[0] = '\0';  // Инициализируем строку нулевым символом

	if (num == 0) {
		strcat(str, "0");
		return str;
	}

	// Алгоритм преобразования в обратном порядке
	while (num > 0) {
		unsigned long long remainder = num % base;
		char digit = digits[remainder];
		char temp[2] = {digit, '\0'};  // Создаем временную строку с цифрой
		str = strcat(str, temp);       // Добавляем цифру в начало строки
		num /= base;
	}

	// Реверсируем строку
	size_t len = strlen(str);
	for (size_t i = 0; i < len / 2; i++) {
		char temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}

	return str;
}

int parseBase(int argc, char** argv, bool debug, unsigned long* base_assign, unsigned long* base_input,
              unsigned long* base_output) {
	char* endptr;
	if (debug) argc--;
	if (argc - 3 > 0) {
		*base_assign = strtoul(argv[3], &endptr, 10);
		if (*endptr != '\0' || !validateBase(*base_assign)) {
			return 1;
		}
	}
	if (argc - 4 > 0) {
		*base_input = strtoul(argv[4], &endptr, 10);
		if (*endptr != '\0' || !validateBase(*base_input)) {
			return 1;
		}
	}
	if (argc - 5 > 0) {
		*base_output = strtoul(argv[5], &endptr, 10);
		if (*endptr != '\0' || !validateBase(*base_output)) {
			return 1;
		}
	}
	return 0;
}

unsigned long long calculateCommandEright(HashTable* synonym, HashTable* rev_synonym, trieNode* root, char* command,
                                          unsigned long base_assign, unsigned long base_input,
                                          unsigned long base_output) {
	// Присваивание и копирование переменной
	if (strchr(command, '(') == NULL) {
		unsigned long long find = treeSearch(root, command);
		if (find != ULLONG_MAX) {
			return find;
		} else {
			if (!isValidNumber(command, base_assign)) {
				return ULLONG_MAX;
			} else {
				unsigned long long value = strtoull(command, NULL, base_assign);
				return value;
			}
		}
	}

	char** parts = splitIntoBrackets(command);
	if (parts == NULL) {
		return ULLONG_MAX;
	}
	char* func = search(rev_synonym, parts[0]);
	if (func == NULL || func[0] == ';') {
		return ULLONG_MAX;
	}
	char* next_func = removeFirstAndLast(parts[1]);
	free(parts[0]);
	free(parts[1]);
	free(parts);

	if (strcmp(func, "not") == 0) {
		unsigned long long res =
		    calculateCommandEright(synonym, rev_synonym, root, next_func, base_assign, base_input, base_output);
		free(next_func);
		free(func);
		return (unsigned long long)(~((unsigned int)(res)));
	} else {
		char** summands = splitStringComma(next_func);
		free(next_func);
		if (summands == NULL) {
			free(func);
			return ULLONG_MAX;
		}
		char* first = trimTrailingSpaces(summands[0]);
		if (!first) {
			free(summands[0]);
			free(summands[1]);
			free(summands);
			return ULLONG_MAX;
		}
		char* second = trimTrailingSpaces(summands[1]);
		if (!second) {
			free(first);
			free(summands[0]);
			free(summands[1]);
			free(summands);
			return ULLONG_MAX;
		}

		free(summands[0]);
		free(summands[1]);
		free(summands);
		if (strlen(first) == 0 || strlen(second) == 0) {
			free(first);
			free(second);
			return ULLONG_MAX;
		}

		unsigned long long resl =
		    calculateCommandEright(synonym, rev_synonym, root, first, base_assign, base_input, base_output);
		unsigned long long resr =
		    calculateCommandEright(synonym, rev_synonym, root, second, base_assign, base_input, base_output);
		free(first);
		free(second);
		if (resl == ULLONG_MAX || resr == ULLONG_MAX) {
			return ULLONG_MAX;
		}
		{  // Считаем результат
			if (strcmp(func, "add") == 0) {
				return (resl + resr) % ((unsigned long long)(UINT_MAX) + 1);
			}
			if (strcmp(func, "mult") == 0) {
				return (resl * resr) % ((unsigned long long)(UINT_MAX) + 1);
			}
			if (strcmp(func, "sub") == 0) {
				return resl - resr;
			}
			if (strcmp(func, "pow") == 0) {
				return fastModExp(resl, resr, 4294967296);
			}
			if (strcmp(func, "div") == 0) {
				return resl / resr;
			}
			if (strcmp(func, "rem") == 0) {
				return resl % resr;
			}
			if (strcmp(func, "xor") == 0) {
				return resl ^ resr;
			}
			if (strcmp(func, "and") == 0) {
				return resl & resr;
			}
			if (strcmp(func, "or") == 0) {
				return resl | resr;
			}
			free(func);
		}
		return ULLONG_MAX;
	}
	return 1;
}

unsigned long long calculateUnOPEright(HashTable* synonym, HashTable* rev_synonym, trieNode* root, char* command,
                                       unsigned long base_assign, unsigned long base_input, unsigned long base_output) {
	char** command_and_variable;
	command_and_variable = splitIntoBrackets(command);
	if (command_and_variable == NULL) {
		return ULLONG_MAX;
	}

	char* op = trimTrailingSpaces(command_and_variable[0]);
	if (!op) {
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}
	char* temp = trimTrailingSpaces(command_and_variable[1]);
	if (!temp) {
		free(op);
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}

	{  // Очистка
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
	}

	char* var = removeFirstAndLast(temp);
	if (!var) {
		free(op);
		free(temp);
		return ULLONG_MAX;
	}
	free(temp);
	if (strcmp(op, search(synonym, "input")) == 0) {
		char* read = readStringWithSpace(stdin);
		if (!isValidNumber(read, base_input)) {
			free(var);
			free(read);
			free(op);
			return ULLONG_MAX;
		}
		unsigned long long res = strtoull(read, NULL, base_input);
		treeInsert(root, var, res);
		free(var);
		free(op);
		free(read);
	} else if (strcmp(op, search(synonym, "output")) == 0) {
		unsigned long long res;
		res = calculateCommandEright(synonym, rev_synonym, root, var, base_assign, base_input, base_output);
		free(var);
		free(op);
		if (res == ULLONG_MAX) {
			return ULLONG_MAX;
		}
		char* out = uintToString(res, base_output);
		if (out == NULL) {
			return ULLONG_MAX;
		}
		printf("%s\n", out);
		free(out);
	} else {
		free(var);
		free(op);
		return ULLONG_MAX;
	}
	return 0;
}

unsigned long long calculateCommandEleft(HashTable* synonym, HashTable* rev_synonym, trieNode* root, char* command,
                                         unsigned long base_assign, unsigned long base_input,
                                         unsigned long base_output) {
	// Присваивание и копирование переменной
	if (strchr(command, '(') == NULL) {
		unsigned long long find = treeSearch(root, command);
		if (find != ULLONG_MAX) {
			return find;
		} else {
			if (!isValidNumber(command, base_assign)) {
				return ULLONG_MAX;
			} else {
				unsigned long long value = strtoull(command, NULL, base_assign);
				return value;
			}
		}
	}

	char** parts = splitIntoBracketsRev(command);
	if (parts == NULL) {
		return ULLONG_MAX;
	}
	char* func = search(rev_synonym, parts[0]);
	if (func == NULL || func[0] == ';') {
		if (func) {
			free(func);
		}
		free(parts[0]);
		free(parts[1]);
		free(parts);
		return ULLONG_MAX;
	}
	char* next_func = removeFirstAndLast(parts[1]);
	if (!next_func) {
		free(func);
		free(parts[0]);
		free(parts[1]);
		free(parts);
		return ULLONG_MAX;
	}
	free(parts[0]);
	free(parts[1]);
	free(parts);
	if (strcmp(func, "not") == 0) {
		unsigned long long res =
		    calculateCommandEleft(synonym, rev_synonym, root, next_func, base_assign, base_input, base_output);
		free(next_func);
		free(func);
		return (unsigned long long)(~((unsigned int)(res)));
	} else {
		char** summands = splitStringComma(next_func);
		free(next_func);
		if (!summands) {
			free(func);
			return ULLONG_MAX;
		}
		char* first = trimTrailingSpaces(summands[0]);
		if (!first) {
			free(func);
			return ULLONG_MAX;
		}
		char* second = trimTrailingSpaces(summands[1]);
		if (!second) {
			free(func);
			free(first);
		}
		free(summands[0]);
		free(summands[1]);
		free(summands);
		if (strlen(first) == 0 || strlen(second) == 0) {
			free(func);
			free(first);
			free(second);
			return ULLONG_MAX;
		}

		unsigned long long resl =
		    calculateCommandEleft(synonym, rev_synonym, root, first, base_assign, base_input, base_output);
		unsigned long long resr =
		    calculateCommandEleft(synonym, rev_synonym, root, second, base_assign, base_input, base_output);
		free(first);
		free(second);
		if (resl == ULLONG_MAX || resr == ULLONG_MAX) {
			free(func);
			return ULLONG_MAX;
		}
		{  // Считаем результат
			if (strcmp(func, "add") == 0) {
				return (resl + resr) % ((unsigned long long)(UINT_MAX) + 1);
			}
			if (strcmp(func, "mult") == 0) {
				return (resl * resr) % ((unsigned long long)(UINT_MAX) + 1);
			}
			if (strcmp(func, "sub") == 0) {
				return resl - resr;
			}
			if (strcmp(func, "pow") == 0) {
				return fastModExp(resl, resr, 4294967296);
			}
			if (strcmp(func, "div") == 0) {
				return resl / resr;
			}
			if (strcmp(func, "rem") == 0) {
				return resl % resr;
			}
			if (strcmp(func, "xor") == 0) {
				return resl ^ resr;
			}
			if (strcmp(func, "and") == 0) {
				return resl & resr;
			}
			if (strcmp(func, "or") == 0) {
				return resl | resr;
			}
		}
		free(func);
		return ULLONG_MAX;
	}
	return 1;
}

unsigned long long calculateUnOPEleft(HashTable* synonym, HashTable* rev_synonym, trieNode* root, char* command,
                                      unsigned long base_assign, unsigned long base_input, unsigned long base_output) {
	char** command_and_variable = splitIntoBracketsRev(command);
	if (command_and_variable == NULL) {
		return ULLONG_MAX;
	}
	char* op = trimTrailingSpaces(command_and_variable[0]);
	if (!op) {
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}
	char* temp = trimTrailingSpaces(command_and_variable[1]);
	if (!temp) {
		free(op);
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}
	{  // Очистка
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
	}
	char* var = removeFirstAndLast(temp);
	free(temp);
	if (!var) {
		free(op);
	}
	if (strcmp(op, search(synonym, "input")) == 0) {
		char* read = readStringWithSpace(stdin);
		if (!isValidNumber(read, base_input)) {
			free(var);
			free(read);
			free(op);
			return ULLONG_MAX;
		}
		unsigned long long res = strtoull(read, NULL, base_input);
		treeInsert(root, var, res);
		free(var);
		free(op);
	} else if (strcmp(op, search(synonym, "output")) == 0) {
		unsigned long long res;
		res = calculateCommandEleft(synonym, rev_synonym, root, var, base_assign, base_input, base_output);
		if (res == ULLONG_MAX) {
			free(var);
			free(op);
			return ULLONG_MAX;
		}
		free(var);
		free(op);
		char* out = uintToString(res, base_output);
		if (out == NULL) {
			return ULLONG_MAX;
		}
		printf("%s\n", out);
		free(out);
	} else {
		free(var);
		free(op);
		return ULLONG_MAX;
	}
	return 0;
}

unsigned long long calculateUnOPEmiddle(HashTable* synonym, HashTable* rev_synonym, trieNode* root, char* command,
                                        unsigned long base_assign, unsigned long base_input,
                                        unsigned long base_output) {
	char** command_and_variable = splitIntoBrackets(command);
	if (command_and_variable == NULL) {
		return ULLONG_MAX;
	}
	char* op = trimTrailingSpaces(command_and_variable[0]);
	if (!op) {
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}
	char* temp = trimTrailingSpaces(command_and_variable[1]);
	if (!temp) {
		free(op);
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
		return ULLONG_MAX;
	}
	char* var = removeFirstAndLast(temp);
	{  // Очистка
		free(temp);
		free(command_and_variable[0]);
		free(command_and_variable[1]);
		free(command_and_variable);
	}
	if (strcmp(op, search(synonym, "input")) == 0) {
		char* read = readStringWithSpace(stdin);
		if (!isValidNumber(read, base_input)) {
			free(op);
			free(var);
			free(read);
			return ULLONG_MAX;
		}
		unsigned long long res = strtoull(read, NULL, base_input);
		treeInsert(root, var, res);
		free(read);
		free(var);
	} else if (strcmp(op, search(synonym, "output")) == 0) {
		unsigned long long res;
		res = calc(root, base_assign, var, synonym, rev_synonym);
		if (res == ULLONG_MAX) {
			free(var);
			free(op);
			return ULLONG_MAX;
		}
		free(var);
		char* out = uintToString(res, base_output);
		if (out == NULL) {
			free(op);
			return ULLONG_MAX;
		}
		printf("%s\n", out);
		free(out);
	} else {
		free(var);
		free(op);
		return ULLONG_MAX;
	}
	free(op);
	return 0;
}

void printMenu() {
	printf("\n1. Output the value of the variable and the memory dump.\n");
	printf("2. Print all variables.\n");
	printf(
	    "3. Сhange the value of a variable to be read from the standard input stream as a number in the number system "
	    "with a base of 16.\n");
	printf("4. Declare a variable whose name is read from the standard input stream and initialize it with a value.\n");
	printf("5. Cancel the declaration of a variable whose name is read from the standard input stream.\n");
	printf("6. End the interactive dialog and continue code execution.\n");
	printf("7. Shut down the interpreter.\n");
}

char* printBinaryToString(const void* data, size_t size) {
	size_t str_size = size * 9;
	char* binary_string = (char*)malloc(str_size + 1);
	if (!binary_string) {
		return NULL;
	}
	binary_string[0] = '\0';

	const unsigned char* byte_ptr = (const unsigned char*)data;

	for (size_t i = 0; i < size; i++) {
		for (int bit = 7; bit >= 0; bit--) {
			char bit_str[2];
			snprintf(bit_str, sizeof(bit_str), "%u", (byte_ptr[i] >> bit) & 1);
			strcat(binary_string, bit_str);
		}
		if (i < size - 1) {
			strcat(binary_string, " ");
		}
	}
	return binary_string;
}

int dumpUnsignedInt(unsigned int value) {
	char* binary_string = printBinaryToString(&value, sizeof(value));
	if (binary_string == NULL) {
		return 1;
	}
	printf("%s\n", binary_string);
	free(binary_string);
	return 0;
}

int romanCharToInt(char c) {
	switch (c) {
		case 'I':
			return 1;
		case 'V':
			return 5;
		case 'X':
			return 10;
		case 'L':
			return 50;
		case 'C':
			return 100;
		case 'D':
			return 500;
		case 'M':
			return 1000;
		default:
			return -1;
	}
}

int romanToInt(const char* roman, unsigned long long* result) {
	*result = 0;
	unsigned long long prevValue = 0;

	for (const char* p = roman; *p != '\0'; ++p) {
		unsigned long long currentValue = romanCharToInt(*p);
		if (currentValue == -1) {
			return -1;
		}
		if (currentValue > prevValue) {
			*result += currentValue - 2 * prevValue;
		} else {
			*result += currentValue;
		}
		prevValue = currentValue;
	}
	return 0;
}

int zeckendorfToInt(const char* zeckendorf, unsigned long long* result) {
	*result = 0;
	unsigned long long fibs[47];
	fibs[0] = 1;
	fibs[1] = 2;
	for (int i = 2; i < 47; i++) {
		fibs[i] = fibs[i - 1] + fibs[i - 2];
	}
	int i = 0;
	while (*zeckendorf == '0' || *zeckendorf == '1') {
		if (*zeckendorf == '1') {
			*result += fibs[i];
		}
		zeckendorf++;
		i++;
	}
	if (*zeckendorf != '\0') {
		return 1;
	}
	return 0;
}

int breakPointMenu(HashTable* synonym, HashTable* rev_synonym, trieNode* root) {
	printMenu();
	while (true) {
		printf("Choise option:\n");
		char* oper = readStringWithSpace(stdin);
		if (oper[0] == 0) {
			free(oper);
			printf("Exiting...\n");
			return 2;
		}
		if (oper == NULL) {
			return 1;
		}
		if (!isValidNumber(oper, 10)) {
			printf("Wrong input.\n");
			free(oper);
			continue;
		}
		unsigned int op = strtoull(oper, NULL, 10);
		free(oper);
		switch (op) {
			case 1: {
				printf("Enter the name of the variable.\n");
				char* temp = readStringWithSpace(stdin);
				if (!temp) {
					return 1;
				}
				char* name = trimTrailingSpaces(temp);
				if (!name) {
					free(temp);
					return 1;
				}
				free(temp);
				unsigned long long find = treeSearch(root, name);
				if (find == ULLONG_MAX) {
					printf("There is no such variable.\n");
				} else {
					printf("%llX\n", find);
					int res = dumpUnsignedInt(find);
					if (res) {
						free(name);
						return 1;
					}
				}
				free(name);
				break;
			}

			case 2: {
				printTrie(root, "", 0);
				break;
			}

			case 3: {
				printf("Enter the name of the variable.\n");
				char* temp = readStringWithSpace(stdin);
				if (!temp) {
					return 1;
				}
				char* name = trimTrailingSpaces(temp);
				if (!name) {
					free(temp);
					return 1;
				}
				free(temp);
				unsigned long long find = treeSearch(root, name);
				if (find == ULLONG_MAX) {
					printf("There is no such variable.\n");
				} else {
					printf("Enter the value of the variable.\n");
					char* tmp = readStringWithSpace(stdin);
					if (!tmp) {
						return 1;
					}
					char* value = trimTrailingSpaces(tmp);
					free(tmp);
					if (!value) {
						return 1;
					}
					if (!isValidNumber(value, 16)) {
						printf("Wrong value for base 16.\n");
						free(value);
						continue;
					}
					unsigned long long res = strtoull(value, NULL, 16);
					treeInsert(root, name, res);
					free(value);
				}
				free(name);
				break;
			}

			case 4: {
				printf("Enter the name of the variable.\n");
				char* temp = readStringWithSpace(stdin);
				if (!temp) {
					return 1;
				}
				char* name = trimTrailingSpaces(temp);
				if (!name) {
					free(temp);
					return 1;
				}
				free(temp);
				unsigned long long find = treeSearch(root, name);
				if (find != ULLONG_MAX) {
					printf("The variable should not be known at the time of the operation.\n");
					free(name);
					continue;
				}

				printf("\nSelect the input method.\n");
				printf("1. Zeckendorf representation.\n");
				printf("2. In Roman numerals.\n");
				char* tmp = readStringWithSpace(stdin);
				if (!tmp) {
					return 1;
				}
				char* choise = trimTrailingSpaces(tmp);
				free(tmp);
				if (!choise) {
					return 1;
				}
				if (!isValidNumber(choise, 10)) {
					printf("Wrong value for base 16.\n");
					free(choise);
					continue;
				}
				unsigned long long res = strtoull(choise, NULL, 10);
				free(choise);
				if (res == 1) {
					printf("Write the value in the Zeckendorf representation.\n");
					char* tmp = readStringWithSpace(stdin);
					if (!tmp) {
						return 1;
					}
					char* val = trimTrailingSpaces(tmp);
					free(tmp);
					if (!val) {
						return 1;
					}
					unsigned long long value = 0;
					if (zeckendorfToInt(val, &value) != 0) {
						printf("Wrong value for zeckendorf representation.\n");
					}
					treeInsert(root, name, value);
					free(val);
				} else if (res == 2) {
					printf("Write the value in the roman representation.\n");
					char* tmp = readStringWithSpace(stdin);
					if (!tmp) {
						return 1;
					}
					char* val = trimTrailingSpaces(tmp);
					free(tmp);
					if (!val) {
						return 1;
					}
					unsigned long long value = 0;
					if (romanToInt(val, &value) != 0) {
						printf("Wrong value for roman representation.\n");
					}
					treeInsert(root, name, value);
					free(val);
				}
				free(name);
				break;
			}

			case 5: {
				printf("Write the name of the variable you want to delete.\n");
				char* temp = readStringWithSpace(stdin);
				if (!temp) {
					return 1;
				}
				char* name = trimTrailingSpaces(temp);
				if (!name) {
					free(temp);
					return 1;
				}
				free(temp);
				unsigned long long find = treeSearch(root, name);
				if (find == ULLONG_MAX) {
					printf("There is no such variable.\n");
				} else {
					treeInsert(root, name, ULLONG_MAX);
				}
				free(name);
				break;
			}

			case 6:
				return 1;
				break;

			case 7:
				return 2;
				break;

			default:
				printf("Wrong input.\n");
				break;
		}
	}
}

int main(int argc, char** argv) {
	unsigned long base_assign = 10;
	unsigned long base_input = 10;
	unsigned long base_output = 10;
	bool debug = false;
	if (argc < 3) {
		printf("Few arguments.\n");
		return 1;
	}
	if (strcmp(argv[argc - 1], "--debug") == 0 || strcmp(argv[argc - 1], "-d") == 0 ||
	    strcmp(argv[argc - 1], "/debug") == 0) {
		debug = true;
	}

	char resolved_path1[PATH_MAX];
    char resolved_path2[PATH_MAX];
    if (realpath(argv[1], resolved_path1) == NULL) {
        perror("Path resolution error for file1");
        return 1;
    }
    if (realpath(argv[2], resolved_path2) == NULL) {
        perror("Path resolution error for file2");
        return 1;
    }
    if (strcmp(resolved_path1, resolved_path2) == 0) {
        printf("The same file.\n");
		return 1;
    }
	

	if (parseBase(argc, argv, debug, &base_assign, &base_input, &base_output) != 0) {
		printf("Wrong base in arguments.\n");
		return 1;
	}
	FILE* settings = fopen(argv[1], "r");
	if (!settings) {
		printf("Error reading file with the settings.\n");
		return 1;
	}
	FILE* program = fopen(argv[2], "r");
	if (!program) {
		fclose(settings);
		printf("Error reading file with the text of the program.\n");
	}

	HashTable* synonyms = createHashTable();
	HashTable* rev_synonyms = createHashTable();
	initHashTable(synonyms);
	initHashTable(rev_synonyms);
	DirectionOfSaving direction_of_saving = DLEFT;
	DirectionOfExecution direction_of_execution = ERIGHT;
	if (loadSettings(settings, synonyms, rev_synonyms, &direction_of_saving, &direction_of_execution) != SUCCESS) {
		printf("Error reading file with the settings.\n");
		fclose(settings);
		fclose(program);
		destroyHashTable(synonyms);
		destroyHashTable(rev_synonyms);
		return 1;
	}
	fclose(settings);
	trieNode* root = createNode();
	char* line;
	while (true) {
		int error;
		line = readStringToSemicolon(program, &error);
		if (error) {
			if (error == 1) printf("Error: You somewhere forgot ;.\n");
			free(line);
			destroyHashTable(synonyms);
			destroyHashTable(rev_synonyms);
			fclose(program);
			freeTrie(root);
			return 1;
		}
		if (line == NULL) {
			printf("Error allocating memory.\n");
			destroyHashTable(synonyms);
			destroyHashTable(rev_synonyms);
			fclose(program);
			freeTrie(root);
			return 1;
		}
		if (line[0] == 0 && feof(program)) {
			free(line);
			break;
		}
		if (strlen(line) == 0) {
			free(line);
			continue;
		}

		{  // Обработка комментариев
			char* commentStart = strchr(line, '#');
			if (commentStart != line && commentStart != NULL) {
				freeTrie(root);
				fclose(program);
				free(line);
				destroyHashTable(synonyms);
				destroyHashTable(rev_synonyms);
				return 0;
			}
			if (commentStart != NULL) {
				char* tmp = trimTrailingSpaces(line);
				if (strcmp(tmp, "#BREAKPOINT") == 0 && debug) {
					printf("\nBreakpoint.\n");
					int res = breakPointMenu(synonyms, rev_synonyms, root);
					if (res == 2) {
						freeTrie(root);
						fclose(program);
						free(line);
						free(tmp);
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						return 0;
					}
				}
				free(tmp);
				*commentStart = '\0';  // Заменяем символ '#' на нулевой символ, чтобы обрезать строку
			}
			if (strlen(line) == 0) {
				free(line);
				continue;
			}
		}

		char** com = splitString(line, search(synonyms, "="));
		free(line);
		// Проверка и очистка
		if (com == NULL) {
			printf("Wrong program.\n");
			destroyHashTable(synonyms);
			freeTrie(root);
			destroyHashTable(rev_synonyms);
			fclose(program);
			return 1;
		}
		char* tmp1 = com[0];
		char* tmp2 = com[1];
		char* variable = trimTrailingSpaces(tmp1);
		char* command = trimTrailingSpaces(tmp2);
		free(tmp1);
		free(tmp2);
		free(com);
		if (direction_of_saving == DRIGHT) {
			char* tmp = variable;
			variable = command;
			command = tmp;
		}
		if (variable == NULL) {
			char* tmp = command;
			command = NULL;
			variable = tmp;
		}
		if (command == NULL) {
			unsigned long long res;
			switch (direction_of_execution) {
				case ERIGHT:
					res = calculateUnOPEright(synonyms, rev_synonyms, root, variable, base_assign, base_input,
					                          base_output);
					if (res != 0) {
						printf("Wrong program.\n");
						free(variable);
						freeTrie(root);
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						fclose(program);
						return 1;
					}
					break;

				case ELEFT:
					res = calculateUnOPEleft(synonyms, rev_synonyms, root, variable, base_assign, base_input,
					                         base_output);
					if (res != 0) {
						printf("Wrong program.\n");
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						fclose(program);
						freeTrie(root);
						free(variable);
						return 1;
					}
					break;

				case EMIDDLE:
					res = calculateUnOPEmiddle(synonyms, rev_synonyms, root, variable, base_assign, base_input,
					                           base_output);
					if (res != 0) {
						printf("Wrong program.\n");
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						fclose(program);
						free(variable);
						freeTrie(root);
						return 1;
					}
					break;

				default:
					break;
			}
			free(variable);

		} else {
			// Проверка на ошибки в написании имени переменной
			if (!isValidVariableName(variable)) {
				printf("Wrong program.\n");
				destroyHashTable(synonyms);
				destroyHashTable(rev_synonyms);
				fclose(program);
				free(command);
				free(variable);
				freeTrie(root);
				return 1;
			}

			unsigned long long value;
			switch (direction_of_execution) {
				case ERIGHT:
					value = calculateCommandEright(synonyms, rev_synonyms, root, command, base_assign, base_input,
					                               base_output);
					if (value == ULLONG_MAX) {
						printf("Wrong program.\n");
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						fclose(program);
						free(command);
						free(variable);
						freeTrie(root);
						return 1;
					}
					treeInsert(root, variable, value);
					break;

				case ELEFT:
					value = calculateCommandEleft(synonyms, rev_synonyms, root, command, base_assign, base_input,
					                              base_output);
					if (value == ULLONG_MAX) {
						printf("Wrong program.\n");
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						free(command);
						free(variable);
						fclose(program);
						freeTrie(root);
						return 1;
					}
					treeInsert(root, variable, value);
					break;

				case EMIDDLE:
					value = calc(root, base_assign, command, synonyms, rev_synonyms);
					if (value == ULLONG_MAX) {
						printf("Wrong program.\n");
						destroyHashTable(synonyms);
						destroyHashTable(rev_synonyms);
						free(command);
						freeTrie(root);
						free(variable);
						fclose(program);
						return 1;
					}
					treeInsert(root, variable, value);
					break;

				default:
					break;
			}
			free(command);
			free(variable);
		}
	}

	freeTrie(root);
	fclose(program);
	destroyHashTable(synonyms);
	destroyHashTable(rev_synonyms);
}