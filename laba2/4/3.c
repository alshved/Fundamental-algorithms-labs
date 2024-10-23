#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

unsigned long long strToULL(const char *str, int base) {
	char *endptr;
	unsigned long long result = strtoull(str, &endptr, base);

	if (*endptr != '\0' || result == LONG_MAX || result == LONG_MIN) {
		return LONG_MIN;
	}
	return result;
}

int isKaprekar(unsigned long long num, int base) {
	if (num == 0) return 0;
    if (num == 1) return 1;

	unsigned long long square = num * num;
	if (square < num) {
		return 0;
	}
	char squareStr[100];
	snprintf(squareStr, sizeof(squareStr), "%llu", square);

	size_t len = strlen(squareStr);
	for (size_t i = 1; i < len; ++i) {
		char leftStr[32], rightStr[32];
		strncpy(leftStr, squareStr, i);
		leftStr[i] = '\0';
		strcpy(rightStr, squareStr + i);

		unsigned long long leftNum = strToULL(leftStr, base);
		if (leftNum == LONG_MIN) {
			return 0;
		}
		unsigned long long rightNum = strToULL(rightStr, base);
		if (rightNum == LONG_MIN) {
			return 0;
		}
		if (rightNum > 0 && leftNum + rightNum == num) {
			return 1;
		}
	}
	return 0;
}

void findKaprekarNumbers(int base, int count, ...) {
	va_list args;
	va_start(args, count);

	printf("Числа Капрекара в системе счисления с основанием %d:\n", base);

	for (int i = 0; i < count; ++i) {
		const char *numStr = va_arg(args, const char *);
		unsigned long long num = strToULL(numStr, base);
        if (num == LONG_MIN){
            continue;
        }

		if (isKaprekar(num, base)) {
			printf("%s\n", numStr);
		}
	}

	va_end(args);
}

int main() {
	int base = 10;
	findKaprekarNumbers(base, 12, "1", "9", "45", "55", "99", "297", "703", "101", "-1", "ASDAS", "12312312312312312312312312312", "0");
	return 0;
}