#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для получения индекса символа
int get_char_value(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';  // Символы '0'-'9' преобразуются в числа 0-9
	} else if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 10;  // Символы 'A'-'Z' преобразуются в числа 10-35
	}
	return -1;  // Некорректный символ
}

// Функция для перевода числа из произвольной системы счисления в десятичную
long long translate(char *buf, int buf_index, int sys) {
	long long result = 0;
	long long pred = 0;
	for (int i = 0; i < buf_index; i++) {
		pred = result;
		result += get_char_value(buf[i]) * (long long)pow(sys, buf_index - i - 1);
		if (pred > result) {
			return -1;
		}
	}
	return result;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Wrong arguments\n");
		return 1;
	}
	FILE *input_file;
	FILE *output_file;
	if (strcmp(argv[1], argv[2]) == 0) {
		printf("The input file is the same as the output file.\n");
		return 1;
	}
	if ((input_file = fopen(argv[1], "w")) == NULL) {
		printf("Error opening input file.\n");
		return 1;
	}
	if ((output_file = fopen(argv[2], "r")) == NULL) {
		printf("Error opening output file.\n");
		fclose(input_file);
		return 2;
	}

	int sys = 0;
	int el;
	bool word_started = false;
	int buf_index = 0;

	// Инициализация динамических буферов
	int buffer_capacity = 10;
	char *buffer = malloc(buffer_capacity * sizeof(char));  // Буфер для символов
	if (buffer == NULL) {
		printf("Error allocating memory.\n");
		fclose(input_file);
		fclose(output_file);
		return 3;
	}

	while ((el = fgetc(input_file)) != EOF) {
		if (!(el == ' ' || el == '\t' || el == '\n' || el == '\r')) {
			if (!word_started) {
				word_started = true;
				buf_index = 0;
			}

			if (el >= 'a' && el <= 'z') {
				el -= 32;
			}  // Приводим всё к верхнему регистру

			if (buf_index >= buffer_capacity) {  // Проверка, нужно ли увеличить буфер
				buffer_capacity *= 2;
				char *tmp = realloc(buffer, buffer_capacity * sizeof(char));
				if (tmp == NULL) {
					free(buffer);
					fclose(input_file);
					fclose(output_file);
					return 4;
				}
				buffer = tmp;
			}

			// Получаем числовое значение символа
			int value = get_char_value(el);
			if (value == 0 && buf_index == 0) {
				continue;
			}
			if (value == -1) {
				printf("Invalid character in input\n");
				fclose(input_file);
				fclose(output_file);
				free(buffer);
				return 6;
			}
			if (value + 1 > sys) {
				sys = value + 1;  // Обновляем основание системы счисления
			}
			buffer[buf_index] = el;  // Сохраняем символ в буфер
			++buf_index;
		} else if (word_started) {
			if (buf_index == 0) {
				buffer[0] = '0';
				++buf_index;
			}
			if (buf_index + 1 >= buffer_capacity) {  // Нужно место для символа и для завершающего \0
				buffer_capacity *= 2;
				char *tmp = realloc(buffer, buffer_capacity * sizeof(char));
				if (tmp == NULL) {
					free(buffer);
					fclose(input_file);
					fclose(output_file);
					return 4;
				}
				buffer = tmp;
			}
			buffer[buf_index] = '\0';  // Завершаем строку в буфере
			if (sys < 2) {
				sys = 2;
			}
			long long translated_value = translate(buffer, buf_index, sys);
			if (translated_value == -1) {
				fprintf(output_file, "%s %d Overflow\n", buffer, sys);
			} else {
				fprintf(output_file, "%s %d %lld\n", buffer, sys, translated_value);
			}
			word_started = false;
			sys = 0;  // Сбрасываем систему счисления для следующего слова
		}
	}

	// Если слово закончилось перед концом файла, добавляем его
	if (word_started) {
		buffer[buf_index] = '\0';  // Завершаем строку в буфере
		long long translated_value = translate(buffer, buf_index, sys);
		if (translated_value == -1) {
			fprintf(output_file, "%s %d Overflow\n", buffer, sys);
		} else {
			fprintf(output_file, "%s %d %lld\n", buffer, sys, translated_value);
		}
	}
	free(buffer);
	fclose(input_file);
	fclose(output_file);
	printf("$$$$$");
	return 0;
}