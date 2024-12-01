#include "help.h"

#include "hashTable.h"

void reverseString(char *str) {
	int length = strlen(str);
	for (int i = 0; i < length / 2; i++) {
		char temp = str[i];
		str[i] = str[length - i - 1];
		str[length - i - 1] = temp;
	}
}

char *readStringWithSpace(FILE *file) {
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	ungetc(c, file);
	char *str = NULL;
	int length = 0;
	int capacity = 16;
	str = (char *)malloc(capacity * sizeof(char));
	if (!str) {
		perror("Ошибка выделения памяти для строки");
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != '\n' && ch != EOF) {
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char *)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				perror("Ошибка перераспределения памяти для строки");
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';
	return str;
}

char *readStringToSemicolon(FILE *file, int *error) {
	*error = 0;
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	char stop_symb;
	if (c == '#') {
		stop_symb = '\n';
	} else {
		stop_symb = ';';
	}
	ungetc(c, file);
	char *str = NULL;
	size_t length = 0;
	size_t capacity = 16;
	str = (char *)malloc(capacity * sizeof(char));
	if (!str) {
		perror("Memory reallocation error for a string.\n");
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != stop_symb) {

		if (ch == '['){
			char tm = '1';
			int count_open = 1;
			int couunt_close = 0;
			while (count_open != couunt_close && tm != EOF){
				tm = getc(file);
				if (tm == '['){
					count_open++;
				}
				if (tm == ']'){
					couunt_close++;
				}
			}
			if (tm == EOF){
				*error = 2;
				return NULL;
			}
			continue;
		}
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char *)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				free(str);
				perror("Memory reallocation error for a string.\n");
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';
	// Костыль проверка, что на последней строке есть ; если она не комментарий
	char tmp;
	if (stop_symb == ';' && ch != ';') {
		if (fscanf(file, "%c", &tmp) == EOF && strlen(str) != 0 && str[length - 1] != ']') {
			free(str);
			*error = 1;
			return NULL;
		}
	}
	return str;
}

char *removeComment(char *str) {
	char *start = strstr(str, "[");
	char *end = strstr(str, "]");

	if (start != NULL && end != NULL) {
		// Вычисляем длину строки без комментария
		size_t len = start - str + end - start - 1;

		// Выделяем память для новой строки
		char *new_str = malloc(len + 1);
		if (new_str == NULL) {
			return NULL;  // Возвращаем NULL, если не удалось выделить память
		}

		// Копируем часть строки до комментария
		strncpy(new_str, str, start - str);

		// Копируем часть строки после комментария
		strcpy(new_str + (start - str), end + 1);

		return new_str;  // Возвращаем указатель на новую строку
	} else {
		// Вычисляем длину строки без комментария
		size_t len = start - str;

		// Выделяем память для новой строки
		char *new_str = malloc(len + 1);
		if (new_str == NULL) {
			return NULL;  // Возвращаем NULL, если не удалось выделить память
		}

		// Копируем часть строки до комментария
		strncpy(new_str, str, len);

		// Добавляем нулевой символ в конце
		new_str[len] = '\0';

		return new_str;  // Возвращаем указатель на новую строку
	}
}

bool isValidVariableName(const char *name) {
	if (name == NULL || *name == '\0') {
		return false;
	}
	if (strchr(name, '_') == NULL){
		return false;
	}
	if (isdigit(*name)) {
		return false;
	}
	// Проверка остальных символов
	for (const char *p = name + 1; *p != '\0'; p++) {
		if (!isalnum(*p) && *p != '_') {
			return false;  // Недопустимый символ
		}
	}
	return true;  // Все символы корректны
}

char *trimTrailingSpaces(char *str) {
	if (str == NULL) {
		return NULL;
	}
	// Удаление пробелов с начала
	size_t start = 0;
	while (str[start] != '\0' && isspace(str[start])) {
		start++;
	}
	// Удаление пробелов с конца
	size_t len = strlen(str);
	while (len > 0 && (isspace(str[len - 1]))) {
		len--;
	}
	// Копирование без пробелов в новую строку
	if (start > len) {
		return NULL;
	}
	size_t new_len = len - start;

	char *new_str = malloc(new_len + 1);
	if (new_str == NULL) {
		return NULL;  // Ошибка выделения памяти
	}
	strncpy(new_str, str + start, new_len);
	new_str[new_len] = '\0';
	return new_str;
}

char **splitIntoBrackets(char *string) {
	if (string == NULL) return NULL;
	char **parts = malloc(sizeof(char *) * 2);
	if (parts == NULL) return NULL;
	char *open_paren = strchr(string, '(');
	if (open_paren == NULL) {
		free(parts);
		return NULL;
	}
	char *temp1 = strndup(string, open_paren - string);
	char *temp2 = strdup(open_paren);
	parts[1] = trimTrailingSpaces(temp2);
	parts[0] = trimTrailingSpaces(temp1);
	free(temp1);
	free(temp2);
	return parts;
}

char **splitIntoBracketsRev(char *string) {
	if (string == NULL) return NULL;
	char **parts = malloc(sizeof(char *) * 2);
	if (parts == NULL) return NULL;
	int close_paren = 0;
	for (int i = strlen(string); i != 0; --i) {
		if (string[i] == ')') {
			close_paren = i;
			break;
		}
	}
	if (close_paren == 0) {
		free(parts);
		return NULL;
	}
	char *temp1 = strndup(string, close_paren + 1);
	char *temp2 = strdup(string + close_paren + 1);
	parts[1] = trimTrailingSpaces(temp1);
	parts[0] = trimTrailingSpaces(temp2);
	free(temp1);
	free(temp2);
	return parts;
}

bool isValidNumber(const char *str, int base_assign) {
	if (str == NULL || *str == '\0') {
		return false;  // Пустая строка
	}
	// Проверка на допустимые символы в зависимости от основания
	for (const char *p = str; *p != '\0'; p++) {
		if (isdigit(*p)) {
			if (*p - '0' >= base_assign) {
				return false;  // Недопустимый символ для данного основания
			}
		} else if (isalpha(*p)) {
			if (toupper(*p) - 'A' >= base_assign - 10) {
				return false;  // Недопустимый символ для данного основания
			}
		} else {
			return false;  // Недопустимый символ
		}
	}

	// Проверка
	char *end;
	unsigned long long value = strtoull(str, &end, base_assign);
	if (value > UINT_MAX || *end != '\0') {
		return false;
	}

	return true;  // Число допустимо
}

char **splitString(const char *str, const char *delimiter) {
	if (str == NULL || delimiter == NULL) {
		return NULL;
	}

	char *pos = strstr(str, delimiter);

	if (pos == NULL) {
		char **parts = malloc(sizeof(char *) * 2);
		if (parts == NULL) {
			return NULL;
		}
		parts[0] = strdup(str);
		parts[1] = NULL;
		return parts;
	}

	// Выделяем память для массива указателей на строки
	char **parts = malloc(sizeof(char *) * 2);
	if (parts == NULL) {
		return NULL;
	}

	// Извлекаем первую часть
	size_t delimiter_len = strlen(delimiter);
	parts[0] = strndup(str, pos - str);

	// Извлекаем вторую часть
	parts[1] = strdup(pos + delimiter_len);

	return parts;
}

char *removeFirstAndLast(char *str) {
	if (str == NULL || strlen(str) < 2) {
		return str;  // Строка пустая или содержит один символ
	}

	// Выделяем память для новой строки
	size_t len = strlen(str) - 2;
	char *new_str = malloc(len + 1);
	if (new_str == NULL) {
		return NULL;  // Ошибка выделения памяти
	}

	// Копируем строку без первого и последнего символа
	strncpy(new_str, str + 1, len);
	new_str[len] = '\0';

	return new_str;
}

char **splitStringComma(char *str) {
	int count_open = 0;
	int count_close = 0;

	// Выделяем память для массива указателей
	char **new_str = malloc(sizeof(char *) * 2);
	if (!new_str) {
		return NULL;
	}

	// Выделяем память для строк
	new_str[0] = malloc(strlen(str) + 1);  // +1 для завершающего '\0'
	if (!new_str[0]) {
		free(new_str);  // Освобождаем память, если выделение не удалось
		return NULL;
	}
	new_str[1] = malloc(strlen(str) + 1);  // +1 для завершающего '\0'
	if (!new_str[1]) {
		free(new_str[0]);
		free(new_str);
		return NULL;
	}

	// Инициализируем строки как пустые
	new_str[0][0] = '\0';
	new_str[1][0] = '\0';

	int i = 0, j = 0, k = 0;
	for (; i < strlen(str); ++i) {
		if (str[i] == ',' && count_close == count_open) {
			// Нашли запятую, разделяющую строки
			i++;  // Переходим к следующему символу
			break;
		}
		if (str[i] == '(') {
			count_open++;
		}
		if (str[i] == ')') {
			count_close++;
		}
		new_str[0][j++] = str[i];
	}

	for (; i < strlen(str); ++i) {
		new_str[1][k++] = str[i];
	}

	// Добавляем завершающие нулевые символы
	new_str[0][j] = '\0';
	new_str[1][k] = '\0';

	return new_str;
}

unsigned long long fastModExp(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
	unsigned long long result = 1;
	if (base == 0 && exponent == 0) {
		return 1;
	}
	if (base == 0) {
		return 0;
	}
	base %= modulus;  // Оптимизация: сразу берем остаток от деления на модуль
	while (exponent > 0) {
		if (exponent % 2 == 1) {
			result = (result * base) % modulus;
		}
		base = (base * base) % modulus;
		exponent >>= 1;  // Побитовый сдвиг вправо (эквивалентно делению на 2)
	}
	return result;
}
