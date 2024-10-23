#include "type.h"

Result toRoman(int number, char** r_value) {
	const int r_numbers[13] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
	const char* r_letters[13] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
	int r_size = 0;
	int r_capacity = 2;
	*r_value = (char*)malloc(sizeof(char) * r_capacity);
	if (*r_value == NULL) {
		return MEMORY_ALLOCATING_ERROR;
	}
	if (number == 0) {
		strcpy(*r_value, "0");
		return SUCCESS;
	}
	if (number < 0) {
		(*r_value)[0] = '-';
		r_size++;
		number = -number;
	}
	while (number > 0) {
		int index = 12;
		while (index >= 0 && r_numbers[index] > number) {
			index--;
		}
		if (index >= 0) {
			const char* letter = r_letters[index];
			for (int i = 0; letter[i] != '\0'; i++) {
				if (r_size >= r_capacity) {
					r_capacity *= 2;
					char* tmp = realloc(*r_value, r_capacity * sizeof(char));
					if (!tmp) {
						free(*r_value);
						return MEMORY_ALLOCATING_ERROR;
					}
					*r_value = tmp;
				}
				(*r_value)[r_size++] = letter[i];
			}
			number -= r_numbers[index];
		}
	}
	(*r_value)[r_size] = '\0';
	return SUCCESS;
}

void zeckendorf(unsigned int num, int* count, char** result) {
	*result[0] = '\0';
	*count = 0;
	if (num == 0) {
		*count++;
		strcpy(*result, "0");
		return;
	}
	unsigned int fib1 = 1, fib2 = 1;
	unsigned int fibs[100];
	int count_fibs = 0;
	fibs[count_fibs++] = fib1;
	while (fib2 <= num) {
		fibs[count_fibs++] = fib2;
		unsigned int temp = fib1 + fib2;
		fib1 = fib2;
		fib2 = temp;
	}
	int zeckendorf_representation[100] = {0};
	int printed = 0;
	for (int index = count_fibs - 1; index >= 0; index--) {
		if (fibs[index] <= num) {
			num -= fibs[index];
			zeckendorf_representation[index] = 1;
			printed = 1;
		} else {
			if (printed) {
				zeckendorf_representation[index] = 0;
			}
		}
	}
	for (int i = 0; i < count_fibs; i++) {
		if (zeckendorf_representation[i] == 1) {
			strcat(*result, "1");
			*count++;
		} else {
			strcat(*result, "0");
			*count++;
		}
	}
	*count++;
	strcat(*result, "1");
}

Result convertFromDecimal(long long int decimal_number, int base, int flag, char** result) {
	if (base < 2 || base > 36) {
		base = 10;
	}
	int index = 0;
	bool is_minus = false;
	(*result) = (char*)malloc((64 + 1) * sizeof(char));
	if (*result == NULL) {
		return MEMORY_ALLOCATING_ERROR;
	}
	int size = 0;
	if (decimal_number == 0) {
		(*result)[index] = '0';
		(*result)[index + 1] = '\0';
		return SUCCESS;
	}
	if (decimal_number < 0) {
		is_minus = true;
		(*result)[index++] = '-';
		size++;
	}
	decimal_number = abs(decimal_number);
	while (decimal_number) {
		long long int remainder = decimal_number % base;
		if (remainder < 10) {
			(*result)[index++] = remainder + '0';
		} else {
			if (flag) {
				(*result)[index++] = remainder - 10 + 'A';
			} else {
				(*result)[index++] = remainder - 10 + 'a';
			}
		}
		size++;
		decimal_number /= base;
	}
	(*result)[size] = '\0';
	int length = strlen(*result);
	for (int i = 0, j = 0; length - 1 - j > i; i++, j++) {
		if (is_minus) {
			i++;
			is_minus = false;
		}
		char tmp = (*result)[i];
		(*result)[i] = (*result)[length - 1 - j];
		(*result)[length - 1 - j] = tmp;
	}
	return SUCCESS;
}

Result convertToDecimal(char* number_str, int base, long long int* dec_number, int flag) {
	if (base < 2 || base > 36) {
		base = 10;
	}

	long long int decimal_number = 0;
	int length = strlen(number_str);
	bool is_negative = (number_str[0] == '-');

	int start_index = is_negative ? 1 : 0;

	for (int i = start_index; i < length; i++) {
		char digit = number_str[i];
		int digit_value;

		if (isdigit(digit)) {
			digit_value = digit - '0';
		} else if (isalpha(digit)) {
			digit_value = (flag ? (digit - 'A' + 10) : (digit - 'a' + 10));
		} else {
			return INVALID_PARAMETR;
		}

		if (digit_value >= base) {
			return INVALID_PARAMETR;
		}
		if (decimal_number > (LLONG_MAX - digit_value) / base) {
			return OVERFLOW;
		}

		decimal_number = decimal_number * base + digit_value;
	}
	if (is_negative) {
		decimal_number = -decimal_number;
	}

	*dec_number = decimal_number;
	return SUCCESS;
}

bool isLower(char* str, int size) {
	for (int i = 0; i < size; i++) {
		if ((str[i] >= 'A') && (str[i] <= 'Z')) {
			return false;
		}
	}
	return true;
}

bool isUpper(char* str, int size) {
	for (int i = 0; i < size; i++) {
		if ((str[i] >= 'a') && (str[i] <= 'z')) {
			return false;
		}
	}
	return true;
}

char* print_binary_to_string(const void* data, size_t size) {
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

char* dump_signed_int(int value) {
	char* binary_string = print_binary_to_string(&value, sizeof(value));
	return binary_string;
}

char* dump_unsigned_int(unsigned int value) {
	char* binary_string = print_binary_to_string(&value, sizeof(value));
	return binary_string;
}

char* dump_double(double value) {
	char* binary_string = print_binary_to_string(&value, sizeof(value));
	return binary_string;
}

char* dump_float(float value) {
	char* binary_string = print_binary_to_string(&value, sizeof(value));
	return binary_string;
}

int overfprintf(FILE* stream, char* format, ...) {
	if (!stream) {
		return -1;
	}
	int size_format = strlen(format);
	if (!size_format) {
		return -1;
	}
	int argc = 0;
	for (int i = 0; i < size_format; ++i) {
		if (format[i] == '%') {
			if (format[i + 1] != '%') {
				argc++;
			}
		}
	}
	va_list ptr;
	va_start(ptr, format);
	int index = 0;
	char* flag = NULL;
	int ret_value = 0;
	for (int i = 0; i < argc; i++) {
		int size_flag = 1;
		int capacity_flag = 2;
		while (format[index] != '%' && index < size_format) {
			ret_value += fprintf(stream, "%c", format[index]);
			index++;
		}  // Считаем количество аргументов

		flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
		if (!flag) {
			return -1;
		}
		flag[0] = '%';
		index++;
		while (index < size_format &&
		       (isalpha(format[index]) || isdigit(format[index]) ||
		        (format[index] == '%' && (index == 0 || format[index - 1] != '%')) ||
		        (format[index] == '.' &&
		         (index + 1 < size_format && (isalpha(format[index + 1]) || isdigit(format[index + 1])))) ||
		        (format[index] == '0'))) {
			if (size_flag + 1 >= capacity_flag) {
				capacity_flag *= 2;
				char* tmp = realloc(flag, capacity_flag * sizeof(char));
				if (!tmp) {
					free(flag);
					return MEMORY_ALLOCATING_ERROR;
				}
				flag = tmp;
			}  // Находим флаг
			if (!strcmp(flag, "%Zr\0") || !strcmp(flag, "%Cv\0") || !strcmp(flag, "%CV\0") || !strcmp(flag, "%to\0") ||
			    !strcmp(flag, "%TO\0") || !strcmp(flag, "%mi\0") || !strcmp(flag, "%mu\0") || !strcmp(flag, "%md\0") ||
			    !strcmp(flag, "%mf\0")) {
				break;
			}
			flag[size_flag++] = format[index];
			index++;
		}
		flag[size_flag] = '\0';
		if (!strcmp(flag, "%Ro\0")) {
			char* roman_value = NULL;
			Result st_roman = toRoman(va_arg(ptr, int), &roman_value);
			if (st_roman == MEMORY_ALLOCATING_ERROR) {
				printf("Memory allocation error\n");
				free(roman_value);
				return -2;
			}
			ret_value += fprintf(stream, "%s", roman_value);
			free(roman_value);
		} else if (!strcmp(flag, "%Zr\0")) {
			unsigned int number = va_arg(ptr, unsigned int);
			int printed = 0;
			char* res = malloc(sizeof(char) * 200);
			if (!res) {
				printf("Memory allocation error\n");
				return -1;
			}
			zeckendorf(number, &printed, &res);
			fprintf(stream, "%s", res);
			ret_value += printed;
		} else if (!strcmp(flag, "%Cv\0")) {
			long long int number_to_convert = va_arg(ptr, long long int);
			argc++;
			int base = va_arg(ptr, int);
			char* result = NULL;
			Result st_convert = convertFromDecimal(number_to_convert, base, 0, &result);
			if (st_convert == MEMORY_ALLOCATING_ERROR) {
				free(result);
				printf("Memory allocation error\n");
				break;
			}
			ret_value += fprintf(stream, "%s", result);
			free(result);
		} else if (!strcmp(flag, "%CV\0")) {
			long long int number_to_convert = va_arg(ptr, long long int);
			argc++;
			int base = va_arg(ptr, int);
			char* result = NULL;
			Result st_convert = convertFromDecimal(number_to_convert, base, 1, &result);
			if (st_convert == MEMORY_ALLOCATING_ERROR) {
				free(result);
				printf("Memory allocation error\n");
				break;
			}
			ret_value += fprintf(stream, "%s", result);
			free(result);
		} else if (!strcmp(flag, "%to\0")) {
			char* num_to_convert = va_arg(ptr, char*);
			int size = strlen(num_to_convert);
			argc++;
			int base = va_arg(ptr, int);
			long long int result;
			if (!isLower(num_to_convert, size)) {
				break;
			}
			Result st_convert = convertToDecimal(num_to_convert, base, &result, 0);
			if (st_convert == INVALID_PARAMETR) {
				break;
			}
			ret_value += fprintf(stream, "%lld", result);
		} else if (!strcmp(flag, "%TO\0")) {
			char* num_to_convert = va_arg(ptr, char*);
			int size = strlen(num_to_convert);
			argc++;
			int base = va_arg(ptr, int);
			long long int result;
			if (!isUpper(num_to_convert, size)) {
				break;
			}
			Result st_convert = convertToDecimal(num_to_convert, base, &result, 1);
			if (st_convert == INVALID_PARAMETR) {
				break;
			}
			ret_value += fprintf(stream, "%lld", result);
		} else if (!strcmp(flag, "%mi\0")) {
			int val = va_arg(ptr, int);
			char* res = dump_signed_int(val);
			ret_value += fprintf(stream, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%mu\0")) {
			unsigned int val = va_arg(ptr, unsigned int);
			char* res = dump_unsigned_int(val);
			ret_value += fprintf(stream, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%md\0")) {
			double val = va_arg(ptr, double);
			char* res = dump_double(val);
			ret_value += fprintf(stream, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%mf\0")) {
			float val = va_arg(ptr, double);
			char* res = dump_float(val);
			ret_value += fprintf(stream, "%s", res);
			free(res);
		} else {
			ret_value += vfprintf(stream, flag, ptr);
		}
		free(flag);
		flag = NULL;
	}
	while (index < size_format) {
		ret_value += fprintf(stream, "%c", format[index]);
		index++;
	}
	if (flag) {
		free(flag);
	}
	va_end(ptr);
	return ret_value;
}

int oversprintf(char* buf, char* format, ...) {
	int size_format = strlen(format);
	if (!size_format) {
		return -1;
	}
	int argc = 0;
	for (int i = 0; i < size_format; ++i) {
		if (format[i] == '%') {
			if (format[i + 1] != '%') {
				argc++;
			}
		}
	}
	va_list ptr;
	va_start(ptr, format);
	int index = 0;
	char* flag = NULL;
	int ret_value = 0;
	for (int i = 0; i < argc; i++) {
		int size_flag = 1;
		int capacity_flag = 2;
		while (format[index] != '%' && index < size_format) {
			ret_value += sprintf(buf + ret_value, "%c", format[index]);
			index++;
		}  // Считаем количество аргументов

		flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
		if (!flag) {
			return -1;
		}
		flag[0] = '%';
		index++;
		while (index < size_format &&
		       (isalpha(format[index]) || isdigit(format[index]) ||
		        (format[index] == '%' && (index == 0 || format[index - 1] != '%')) ||
		        (format[index] == '.' &&
		         (index + 1 < size_format && (isalpha(format[index + 1]) || isdigit(format[index + 1])))) ||
		        (format[index] == '0'))) {
			if (size_flag + 1 >= capacity_flag) {
				capacity_flag *= 2;
				char* tmp = realloc(flag, capacity_flag * sizeof(char));
				if (!tmp) {
					free(flag);
					return MEMORY_ALLOCATING_ERROR;
				}
				flag = tmp;
			}  // Находим флаг
			if (!strcmp(flag, "%Zr\0") || !strcmp(flag, "%Cv\0") || !strcmp(flag, "%CV\0") || !strcmp(flag, "%to\0") ||
			    !strcmp(flag, "%TO\0") || !strcmp(flag, "%mi\0") || !strcmp(flag, "%mu\0") || !strcmp(flag, "%md\0") ||
			    !strcmp(flag, "%mf\0")) {
				break;
			}
			flag[size_flag++] = format[index];
			index++;
		}
		flag[size_flag] = '\0';

		if (!strcmp(flag, "%Ro\0")) {
			char* roman_value = NULL;
			Result st_roman = toRoman(va_arg(ptr, int), &roman_value);
			if (st_roman == MEMORY_ALLOCATING_ERROR) {
				printf("Memory allocation error\n");
				free(roman_value);
				return -2;
			}
			ret_value += sprintf(buf + ret_value, "%s", roman_value);
			free(roman_value);
		} else if (!strcmp(flag, "%Zr\0")) {
			unsigned int number = va_arg(ptr, unsigned int);
			int printed = 0;
			char* res = malloc(sizeof(char) * 200);
			if (!res) {
				printf("Memory allocation error\n");
				return -1;
			}
			zeckendorf(number, &printed, &res);
			ret_value += sprintf(buf + ret_value, "%s", res);
		} else if (!strcmp(flag, "%Cv\0")) {
			long long int number_to_convert = va_arg(ptr, long long int);
			argc++;
			int base = va_arg(ptr, int);
			char* result = NULL;
			Result st_convert = convertFromDecimal(number_to_convert, base, 0, &result);
			if (st_convert == MEMORY_ALLOCATING_ERROR) {
				free(result);
				printf("Memory allocation error\n");
				break;
			}
			ret_value += sprintf(buf + ret_value, "%s", result);
			free(result);
		} else if (!strcmp(flag, "%CV\0")) {
			long long int number_to_convert = va_arg(ptr, long long int);
			argc++;
			int base = va_arg(ptr, int);
			char* result = NULL;
			Result st_convert = convertFromDecimal(number_to_convert, base, 1, &result);
			if (st_convert == MEMORY_ALLOCATING_ERROR) {
				free(result);
				printf("Memory allocation error\n");
				break;
			}
			ret_value += sprintf(buf + ret_value, "%s", result);
			free(result);
		} else if (!strcmp(flag, "%to\0")) {
			char* num_to_convert = va_arg(ptr, char*);
			int size = strlen(num_to_convert);
			argc++;
			int base = va_arg(ptr, int);
			long long int result;

			if (!isLower(num_to_convert, size)) {
				break;
			}

			Result st_convert = convertToDecimal(num_to_convert, base, &result, 0);
			if (st_convert == INVALID_PARAMETR) {
				break;
			}
			ret_value += sprintf(buf + ret_value, "%lld", result);
		} else if (!strcmp(flag, "%TO\0")) {
			char* num_to_convert = va_arg(ptr, char*);
			int size = strlen(num_to_convert);
			argc++;
			int base = va_arg(ptr, int);
			long long int result;

			if (!isUpper(num_to_convert, size)) {
				break;
			}

			Result st_convert = convertToDecimal(num_to_convert, base, &result, 1);
			if (st_convert == INVALID_PARAMETR) {
				break;
			}
			ret_value += sprintf(buf + ret_value, "%lld", result);
		} else if (!strcmp(flag, "%mi\0")) {
			int val = va_arg(ptr, int);
			char* res = dump_signed_int(val);
			ret_value += sprintf(buf + ret_value, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%mu\0")) {
			unsigned int val = va_arg(ptr, unsigned int);
			char* res = dump_unsigned_int(val);
			ret_value += sprintf(buf + ret_value, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%md\0")) {
			double val = va_arg(ptr, double);
			char* res = dump_double(val);
			ret_value += sprintf(buf + ret_value, "%s", res);
			free(res);
		} else if (!strcmp(flag, "%mf\0")) {
			float val = va_arg(ptr, double);
			char* res = dump_float(val);
			ret_value += sprintf(buf + ret_value, "%s", res);
			free(res);
		} else {
			ret_value += vsprintf(buf + ret_value, flag, ptr);
		}
		free(flag);
		flag = NULL;
	}
	while (index < size_format) {
		ret_value += sprintf(buf + ret_value, "%c", format[index]);
		index++;
	}
	if (flag) {
		free(flag);
	}
	va_end(ptr);
	return ret_value;
}