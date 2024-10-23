#include "func.h"

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

ResultCode GeometricMean(long double* res, int count, ...) {
	if (count < 1) {
		return INVALID_ARGUMENTS;
	}
	va_list ptr;
	long double product = 1.0;
	va_start(ptr, count);
	for (int i = 0; i < count; i++) {
		double value = va_arg(ptr, double);
		if (value <= 0) {
			va_end(ptr);
			return NON_POSITIVE_VALUE;
		}

		product *= value;
		if (product == INFINITY) {
			va_end(ptr);
			return OVERFLOW;
		}
	}
	va_end(ptr);
	*res = pow(product, 1.0 / count);

	if (isnan(*res)) {
		return IS_NAN;
	}
	return SUCCESS;
}

ResultCode RecursivePow(long double* res, long double number, int p) {
	if (p == 0) {
		*res = 1;
		return SUCCESS;
	}
	if (number == 0) {
		*res = 0;
		return SUCCESS;
	}
	if (fabsl(number) <= 1e-20) {
		return IS_NAN;
	}
	if (p < 0) {
		RecursivePow(res, 1.0 / number, -p);
	} else {
		long double tmp = 1;
		if (p % 2 == 0) {
			RecursivePow(&tmp, number, p / 2);
			*res = tmp * tmp;
		} else {
			RecursivePow(&tmp, number, (p - 1) / 2);
			*res = number * tmp * tmp;
		}
	}
	if (*res == INFINITY) {
		return OVERFLOW;
	}
	if (fabsl(*res) < 1e-20) {
		return IS_NAN;
	}
	if (isnan(*res)) {
		return IS_NAN;
	}
	return SUCCESS;
}