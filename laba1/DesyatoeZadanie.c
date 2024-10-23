#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool stop(char* str) {
	if (str[0] == 'S' && str[1] == 't' && str[2] == 'o' && str[3] == 'p' && str[4] == '\0') {
		return true;
	}
	return false;
}

int len(char* string) {
	int count = 0;
	for (int i = 0; string[i] != '\0'; ++i) {
		count++;
	}
	return count;
}

int get_char_value(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';  // Символы '0'-'9' преобразуются в числа 0-9
	} else if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 10;  // Символы 'A'-'Z' преобразуются в числа 10-35
	}
	return -1;  // Некорректный символ
}

bool parse_is_possible(char* str, int sys) {
	int i = (str[0] == '-') ? 1 : 0;
	for (; str[i] != '\0'; ++i) {
		int value = get_char_value(str[i]);
		if (value < 0 || value >= sys) {
			return false;
		}
	}
	return true;
}

// Функция для перевода числа из произвольной системы счисления в десятичную
int translate_from_char(char* buf, int size, int sys, long long* result) {
	int minus = (buf[0] == '-') ? -1 : 1;
	int i = (minus == -1) ? 1 : 0;
	*result = 0;
	long long pred = -999;
	for (; i < size; i++) {
		pred = *result;
		*result += get_char_value(buf[i]) * (long long)pow(sys, size - i - 1);
		if (pred > *result) {
			return 1;
		}
	}
	*result *= minus;
	return 0;
}

int read_number(long long* number, int sys) {
	int buff_size = 10;
	char* str = malloc((buff_size) * sizeof(char));
	if (str == NULL) {
		return 1;
	}
	char el = getchar();
	while (el == ' ' || el == '\t' || el == '\n' || el == '\r') {
		el = getchar();
	}
	int size = 0;
	while (!(el == ' ' || el == '\t' || el == '\n' || el == '\r')) {
		if (size + 1 >= buff_size){
			buff_size *= 2;
			char* tmp = realloc(str, buff_size * sizeof(char));
			if (tmp == NULL){
				free(str);
				return 1;
			}
			str = tmp;
		}
		str[size++] = el;
		el = getchar();
	}
	str[size] = '\0';
	if (stop(str)) {
		free(str);
		return 100;
	}
	if (parse_is_possible(str, sys)) {
		if (translate_from_char(str, size, sys, number) != 0) {
			free(str);
			return 200;
		}
	} else {
		free(str);
		return 2;
	}
	free(str);
	return 0;
}

char from_int_to_char(int n) {
	if (n >= 0 && n <= 9) {
		return (char)(n + 48);
	} else {
		return (char)(n + 55);
	}
}

char* translate_from_long_long(long long number, int sys) {
	int size = 4;
	int index = 0;
	char* result = malloc(sizeof(char) * size);
	if (result == NULL) {
		printf("Memmory allocating error.\n");
		return NULL;
	}
	if (number == 0) {
		result[0] = '0';
		result[1] = '\0';
		return result;
	}
	int is_negative = (number < 0);
	if (is_negative) {
		number = -number;
	}
	while (number > 0) {
		if (index + 1 >= size) {
			size *= 2;
			char* tmp = realloc(result, sizeof(char) * size);
			if (tmp == NULL) {
				free(result);
				printf("Memmory allocating error.\n");
				return NULL;
			}
			result = tmp;
		}
		result[index++] = from_int_to_char(number % sys);
		number /= sys;
	}
	if (is_negative) {
		if (index + 1 >= size) {
			size *= 2;
			char* tmp = realloc(result, sizeof(char) * size);
			if (tmp == NULL) {
				free(result);
				printf("Memmory allocating error.\n");
				return NULL;
			}
			result = tmp;
		}
		result[index++] = '-';
	}
	for (int i = 0; i < index / 2; i++) {
		char temp = result[i];
		result[i] = result[index - i - 1];
		result[index - i - 1] = temp;
	}
	result[index] = '\0';
	return result;
}

void print_res(long long num, int sys){
	char* ans = translate_from_long_long(num, sys);
	printf("%d: %s\n", sys, ans);
	free(ans);
}

int main(int argc, char** argv) {
	if (argc != 1){
		printf("Wrong arguments.\n");
		return 1;
	}
	int sys;
	if (scanf("%d", &sys) != 1) {
		printf("Wrong input\n");
		return 1;
	}
	if (sys < 2 || sys > 36) {
		printf("Wrong number system.\n");
		return 2;
	}
	long long number;
	int index_ll = 0;
	int allocated_size = 200;
	long long* ll_array = malloc(allocated_size * sizeof(long long));
	if (ll_array == NULL) {
		printf("Memmory allocating error.\n");
		return 3;
	}
	while (true) {
		int res = read_number(&number, sys);
		if (res == 1) {
			printf("Memmory allocating error.\n");
			free(ll_array);
			return 4;
		} else if (res == 2) {
			printf("Wrong number.\n");
		} else if (res == 100) {
			break;
		} else if (res == 200) {
			printf("Overlow long long.\n");
			continue;
		} else {
			if (index_ll >= allocated_size) {
				long long* tmp = realloc(ll_array, allocated_size * 2 * sizeof(long long));
				if (tmp == NULL) {
					printf("Memmory allocating error.\n");
					free(ll_array);
					return 5;
				}
				ll_array = tmp;
			}
			ll_array[index_ll++] = number;
		}
	}
	int index_max = 0;
	for (int i = 0; i != index_ll; ++i) {
		if (abs(ll_array[i]) > abs(ll_array[index_max])) {
			index_max = i;
		}
	}
	long long max_number = ll_array[index_max];
	if (index_ll == 0){
		printf("You didn`t write any numbers.\n");
		free(ll_array);
		return 10;
	}
	print_res(max_number, sys);
	print_res(max_number, 9);
	print_res(max_number, 18);
	print_res(max_number, 27);
	print_res(max_number, 36);
	free(ll_array);
}