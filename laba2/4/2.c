#include <math.h>
#include <stdarg.h>
#include <stdio.h>

double polynomialValueHorner(double x, int degree, ...) {
	va_list args;
	va_start(args, degree);

	double result = va_arg(args, double);

	for (int i = 0; i < degree; ++i) {
		double coeff = va_arg(args, double);
		result = result * x + coeff;
		if (isinf(result)) {
			return INFINITY;
		}
		if (isnan(result)) {
			return NAN;
		}
	}
	va_end(args);
	return result;
}

int main() {
	double x = 2.0;
	double result = polynomialValueHorner(x, 3, 3.0, 2.0, 1.0, 1.0);
	printf("Значение многочлена при x = %.2f: %.2f\n", x, result);

	x = 1000000300000000.0;
	result = polynomialValueHorner(x, 4, 4.0, -2.0, 1.0, 125.5, 229.5);
	printf("Значение многочлена при x = %.2f: %.2f\n", x, result);

	x = 7.0;
	result = polynomialValueHorner(x, 7, 111.8, -2.0, -1.1, 123.22, 123.3, 2341231.0, 1234123.0);
	printf("Значение многочлена при x = %.2f: %.2f\n", x, result);

    x = 9.0;
	result = polynomialValueHorner(x, 1, 10.0, 1.0);
	printf("Значение многочлена при x = %.2f: %.2f\n", x, result);

    x = 15.0;
	result = polynomialValueHorner(x, 0, 18.0);
	printf("Значение многочлена при x = %.2f: %.2f\n", x, result);

	return 0;
}