#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int romanToInt(const char *roman) {
	int result = 0;
	int prevValue = 0;

	for (const char *p = roman; *p != '\0'; ++p) {
		int currentValue = romanCharToInt(*p);
		if (currentValue == -1) {
			return -1;
		}
		if (currentValue > prevValue) {
			result += currentValue - 2 * prevValue;
		} else {
			result += currentValue;
		}
		prevValue = currentValue;
	}

	return result;
}

unsigned int zeckendorfToInt(const char *zeckendorf) {
	unsigned int result = 0;
	unsigned int fibs[47];
	fibs[0] = 1;
	fibs[1] = 1;
	for (int i = 2; i < 47; i++) {
		fibs[i] = fibs[i - 1] + fibs[i - 2];
	}
	int i = 0;
	while (*zeckendorf == '0' || *zeckendorf == '1') {
		if (*zeckendorf == '1' && *(zeckendorf + 1) == '\0') {
			return result;
		}
		if (*zeckendorf == '0' && *(zeckendorf + 1) == '\0') {
			return -1;
		}
		if (*zeckendorf == '1') {
			result += fibs[i];
		}
		zeckendorf++;
		i++;
	}
}

int baseToInt(const char *str, int base) {
	if (base < 2 || base > 36) {
		base = 10;
	}
	int result = 0;
	while (*str) {
		int value;
		if (isdigit(*str)) {
			value = *str - '0';
		} else if (isalpha(*str)) {
			value = tolower(*str) - 'a' + 10;
		} else {
			return -1;
		}
		if (value >= base) {
			return -1;
		}
		result = result * base + value;
		str++;
	}
	return result;
}

int overSscanf(const char *str, const char *format, ...) {
	va_list args;
	va_start(args, format);
	int count = 0;
	const char *cur = str;

	while (*format != '\0') {
		while (isspace(*cur) && *cur != '\0') {
			cur++;
		}
		if (*format == '%') {
			format++;
			if (*format == 'R' && *(format + 1) == 'o') {
				format += 2;
				char buffer[1024];
				sscanf(cur, "%s", buffer);
				int *tmp = (va_arg(args, int *));
				if (tmp == NULL) {
					return -1;
				}
				*tmp = romanToInt(buffer);
				cur += strlen(buffer);
				count++;
			} else if (*format == 'Z' && *(format + 1) == 'r') {
				format += 2;
				char buffer[1024];
				sscanf(cur, "%s", buffer);
				unsigned int *tmp = (va_arg(args, unsigned int *));
				if (tmp == NULL) {
					return -1;
				}
				*tmp = zeckendorfToInt(buffer);

				cur += strlen(buffer);
				count++;
			} else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {
				void **resultAddress = va_arg(args, void **);
				if (resultAddress == NULL) {
					return -1;
				}
				format += 2;
				int base = va_arg(args, int);
				char buffer[1024];
				sscanf(cur, "%s", buffer);
				*resultAddress = (void *)baseToInt(buffer, base);
				cur += strlen(buffer);
				count++;
			} else {
				char *flag = malloc(sizeof(char) * 4);
				if (!flag) {
					va_end(args);
					return -1;
				}
				int size_flag = 1;
				flag[0] = '%';
				int capacity_flag = 4;
				while (*format != '\0' && (isalpha(*format) || isdigit(*format))) {
					if (size_flag + 1 >= capacity_flag) {
						capacity_flag *= 2;
						char *tmp = realloc(flag, capacity_flag * sizeof(char));
						if (!tmp) {
							free(flag);
							va_end(args);
							return -1;
						}
						flag = tmp;
					}
					flag[size_flag++] = *format;
					++format;
				}
				flag[size_flag] = '\0';
				void *arg = va_arg(args, void *);
				if (arg == NULL) {
					return -1;
				}
				int chars_read = sscanf(cur, flag, arg);
				free(flag);
				if (chars_read > 0) {
					while (*cur && !isspace(*cur)) {
						cur++;
					}
					count += chars_read;
				} else {
					cur++;
				}
			}

		} else {
			if (*cur == *format) {
				cur++;
			}
			format++;
		}
	}
	va_end(args);
	return count;
}

int overFscanf(FILE *stream, const char *format, ...) {
	va_list args;
	va_start(args, format);
	int count = 0;
	char buffer[1024];

	while (*format != '\0') {
		char space;
		space = fgetc(stream);
		while (isspace(space)) {
			space = fgetc(stream);
		}
		ungetc(space, stream);
		if (*format == '%') {
			format++;
			if (*format == 'R' && *(format + 1) == 'o') {
				format++;
				fscanf(stream, "%s", buffer);
				int *tmp = (va_arg(args, int *));
				if (tmp == NULL) {
					return -1;
				}
				*tmp = romanToInt(buffer);
				count++;
			} else if (*format == 'Z' && *(format + 1) == 'r') {
				format++;
				fscanf(stream, "%s", buffer);
				unsigned int *tmp = (va_arg(args, unsigned int *));
				if (tmp == NULL) {
					return -1;
				}
				*tmp = zeckendorfToInt(buffer);
				count++;
			} else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {
				format += 2;
				void **resultAddress = va_arg(args, void **);
				if (resultAddress == NULL){
					return -1;
				}
				int base = va_arg(args, int);
				char buffer[1024];
				fscanf(stream, "%s", buffer);
				*resultAddress = (void *)baseToInt(buffer, base);
				count++;
			} else {
				char *flag = malloc(sizeof(char) * 4);
				if (!flag) {
					va_end(args);
					return -1;
				}
				int size_flag = 1;
				flag[0] = '%';
				int capacity_flag = 4;
				while (*format != '\0' && (isalpha(*format) || isdigit(*format))) {
					if (size_flag + 1 >= capacity_flag) {
						capacity_flag *= 2;
						char *tmp = realloc(flag, capacity_flag * sizeof(char));
						if (!tmp) {
							free(flag);
							va_end(args);
							return -1;
						}
						flag = tmp;
					}
					flag[size_flag++] = *format;
					++format;
				}
				flag[size_flag] = '\0';
				void *arg = va_arg(args, void *);
				if (arg == NULL){
					return -1;
				}
				int chars_read = fscanf(stream, flag, arg);
				free(flag);
				if (chars_read > 0) {
					count += chars_read;
				}
			}

		} else {
			format++;
		}
	}
	va_end(args);
	return count;
}