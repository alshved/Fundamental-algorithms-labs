#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR_ALLOC 1
#define INVALID_PARAM 2

int lenDouble(double number) {
	const double epsilon = 1e-10;
	int size = 0;
	int i = 0;
	while (fabs((long long)number - number) > epsilon) {
		if (i == 10) {
			break;
		}
		++i;
		number *= 10;
		size++;
	}
	return size;
}

int gcd(int a, int b) {
	while (b) {
		int tmp = b;
		b = a % b;
		a = tmp;
	}
	return a;
}

void toDrobi(double number, int* numerator, int* denominator) {
	const double epsilon = 1e-10;
	if (fabs(number) < epsilon) {
		*denominator = -1;
		*numerator = 0;
		return;
	}
	int size = lenDouble(number);
	int pow = 1;
	*numerator = 0;
	*denominator = 1;
	for (int i = 0; i < size; i++) {
		pow *= 10;
		*numerator = *numerator * 10 + ((int)(number * pow) % 10);
		*denominator *= 10;
	}
	int gcd_value = gcd(*numerator, *denominator);
	*numerator /= gcd_value;
	*denominator /= gcd_value;
}

int factorize(int** nums, int* size, int number) {
	if (number < 2) {
		return INVALID_PARAM;
	}
	int index = 0;
	if (number % 2 == 0) {
		(*nums)[index++] = 2;
	}
	for (int i = 3; i <= number; i += 2) {
		if (number % i == 0) {
			bool is_prime = true;
			for (int j = 0; j < index && (*nums)[j] * (*nums)[j] <= i; j++) {
				if (i % (*nums)[j] == 0) {
					is_prime = false;
					break;
				}
			}
			if (is_prime) {
				if (index >= *size) {
					*size *= 2;
					int* temp = realloc(*nums, *size * sizeof(int));
					if (!temp) {
						return ERROR_ALLOC;
					}
					*nums = temp;
				}
				(*nums)[index++] = i;
			}
		}
	}
	*size = index;
	return SUCCESS;
}

int isFiniteRepresentation(int base, int denominator, bool* result) {
	int size_number = 64;
	int* primes = malloc(size_number * sizeof(int));
	if (!primes) {
		return ERROR_ALLOC;
	}

	int prime_status = factorize(&primes, &size_number, denominator);
	if (prime_status != SUCCESS) {
		free(primes);
		return prime_status;
	}

	for (int i = 0; i < size_number; i++) {
		if (base % primes[i] != 0) {
			*result = false;
			free(primes);
			return SUCCESS;
		}
	}

	free(primes);
	*result = true;
	return SUCCESS;
}

int checkFinite(double** res, int base, int* size, int count, ...) {
	if (count < 1) {
		return INVALID_PARAM;
	}

	*res = malloc(sizeof(double) * count);
	if (*res == NULL) {
		return ERROR_ALLOC;
	}
	int index = 0;
	va_list args;
	va_start(args, count);
	for (int i = 0; i < count; i++) {
		double number = va_arg(args, double);
		int numerator, denominator;
		toDrobi(number, &numerator, &denominator);
		if (numerator == 0 && denominator == -1) {
			return INVALID_PARAM;
		}
		bool finite;
		int status = isFiniteRepresentation(base, denominator, &finite);
		if (status != SUCCESS) {
			va_end(args);
			free(*res);
			return status;
		}
		if (finite) {
			(*res)[index++] = number;
		}
	}

	*size = index;
	va_end(args);
	return SUCCESS;
}

int main() {
	double* result;
	int size;
	int status = checkFinite(&result, 5, &size, 4, 0.2, 0.4, 0.44, 0.1);
	if (status == SUCCESS) {
		for (int i = 0; i < size; i++) {
			printf("%f ", result[i]);
		}
		printf("\n");
	} else if (status == INVALID_PARAM) {
		printf("Invalid parameter.\n");
	} else if (status == ERROR_ALLOC) {
		printf("Memory allocation error occurred.\n");
	}
	free(result);
	return 0;
}