#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "func.h"

void CheckGeoma(ResultCode res, long double number) {
	switch (res) {
		case SUCCESS:
			printf("%.21Lf\n", number);
			break;

		case INVALID_ARGUMENTS:
			printf("Invalid arguments\n");
			break;

		case OVERFLOW:
			printf("Overflow\n");
			break;

		case IS_NAN:
			printf("Nan\n");
			break;

		case NON_POSITIVE_VALUE:
			printf("Non positive value.\n");
			break;

		default:
			break;
	}
}

void Geoma() {
	printf("Geometric mean:\n");
	long double number = 0;
	ResultCode res;
	res = GeometricMean(&number, 6, 2.0, 3.0, 4.0, 5.0, 6.0, 10.0);
	CheckGeoma(res, number);
	number = 0;
	res = GeometricMean(&number, 3, 1.2, 5.6, 4.0);
	CheckGeoma(res, number);
	res = GeometricMean(&number, 2, 6.0, 1.0);
	CheckGeoma(res, number);
	res = GeometricMean(&number, 2, -5.0, 3.0);
	CheckGeoma(res, number);
	res = GeometricMean(&number, 1, 5.0);
	CheckGeoma(res, number);
	res = GeometricMean(&number, 6, 1337, 1338, 1339, 1310, 1311, 1312);
	CheckGeoma(res, number);
}

void CheckPow(ResultCode res, long double number) {
	switch (res) {
		case SUCCESS:
			printf("%.20Lf\n", number);
			break;

		case OVERFLOW:
			printf("Overflow\n");
			break;

		case IS_NAN:
			printf("Nan\n");
			break;

		default:
			break;
	}
}

void Pow() {
	printf("Pow:\n");
	long double number;
	ResultCode t;
	t = RecursivePow(&number, -5, 3);
	CheckPow(t, number);
	t = RecursivePow(&number, 2, 8);
	CheckPow(t, number);
	t = RecursivePow(&number, 123, 12443);
	CheckPow(t, number);
	t = RecursivePow(&number, 0.00000001, 7);
	CheckPow(t, number);
	t = RecursivePow(&number, 0.00000001, 2);
	CheckPow(t, number);
	t = RecursivePow(&number, 256, -3);
	CheckPow(t, number);
}

int main() {
	Geoma();
	Pow();
	return 0;
}