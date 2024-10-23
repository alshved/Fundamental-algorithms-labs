#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct string {
	char* data;
	int cap;
} string;

int charToInt(char x) { return isdigit(x) ? x - '0' : islower(x) ? x - 'a' + 10 : x - 'A' + 10; }
char intToChar(int x) { return (x < 10) ? '0' + x : 'A' + x - 10; }

void reverseString(char* str) {
	int len = strlen(str);
	for (int i = 0; i < len / 2; i++) {
		char temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
	}
}

int addition(string* sum, char* el, int base) {
	int carry = 0;
	int len1 = strlen(sum->data);
	int len2 = strlen(el);
	int maxLen = len1 > len2 ? len1 : len2;
    while (*el == '0' && len2 > 0) { 
        el++;
        len2--;
    }
	char* res = (char*)malloc(maxLen + 2);
	if (!res) {
		return -1;
	}
    bool skipzeros = true;
	int i, j, k;
	for (i = len1 - 1, j = len2 - 1, k = 0; i >= 0 || j >= 0 || carry; --i, --j) {
		int sumDigits = carry;
		if (i >= 0) sumDigits += charToInt(sum->data[i]);
		if (j >= 0) sumDigits += charToInt(el[j]);
		carry = sumDigits / base;
		res[k++] = intToChar(sumDigits % base);
	}
	res[k] = '\0';

	reverseString(res);

	free(sum->data);
	sum->data = res;
	return 0;
}

int sum(int base, int count, ...) {
	if (base < 2 || base > 36){
		printf("Некорректное основание системы счисления.\n");
		return 0;
	}
	string st;
	st.cap = 2;
	st.data = (char*)malloc(sizeof(char) * st.cap);
	if (!st.data) {
		return -1;
	}
	st.data[0] = '\0';

	va_list elements;
	va_start(elements, count);
	for (int i = 0; i < count; ++i) {
		char* elem = va_arg(elements, char*);
		if (elem == NULL){
			return 1;
		}
		if (addition(&st, elem, base) != 0) {
			free(st.data);
			va_end(elements);
			return -1;
		}
	}
	va_end(elements);
    if (strlen(st.data) == 0){
        st.data[0] = '0';
        st.data[1] = '\0';
    }
	printf("Сумма: %s\n", st.data);
	free(st.data);
	return 0;
}

int main() {
	if (sum(10, 3, "123", "123", "0123") != 0) {
		printf("Ошибка выделения памяти.\n");
	}
	if (sum(5, 4, "123", "400321", "420", "100") != 0) {
		printf("Ошибка выделения памяти.\n");
	}
	if (sum(16, 3, "AAA", "AAA", "7") != 0) {
		printf("Ошибка выделения памяти.\n");
	}
    if (sum(12, 3, "00", "00000000", NULL) != 0) {
		printf("Ошибка выделения памяти.\n");
	}
	return 0;
}