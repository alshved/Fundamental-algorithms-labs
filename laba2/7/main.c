#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int binsearch(long double (*function)(long double x), long double a, long double b, long double eps, long double* result) {
	if (function(a) * function(b) < 0) {
		double mid;
		while (fabs(a - b) > eps) {
			mid = (a + b) / 2;
			if (function(a) * function(mid) > 0) {
				a = mid;
			} else if (function(b) * function(mid) > 0) {
				b = mid;
			} else {
				if (isnan(function(mid)) || isinf(function(mid))) {
					return 2;
				}
				*result = mid;
				return 0;
			}
		}
		if (isnan(function(mid)) || isinf(function(mid))) {
			return 2;
		}
		*result = (a + b) / 2;
		return 0;
	}
	return 1;
}

long double f1(long double x) { return log(x) - 1; }

long double f2(long double x) { return powl(x, 2) - powl(x, 3) + 0.5; }

long double f3(long double x) { return powl(logl(x) * logl(x), 2) - 3; }

long double f4(long double x) { return powl((x - 1), 4) / (x - 1); }

long double f5(long double x) {return powl(x, 2);}

int main() {
	long double res;
	int prob;
	if ((prob = binsearch(f1, 0, 100, 0.0001, &res)) == 1) {
		printf("Необходимо, чтобы функцяи на концах отрезков принимала значения разных знаков.\n");
	} else if (prob == 2) {
		printf("Нет корней.\n");
	} else {
		printf("%Lf\n", res);
	}

	if ((prob = binsearch(f2, 0, 100, 0.0001, &res)) == 1) {
		printf("Необходимо, чтобы функцяи на концах отрезков принимала значения разных знаков.\n");
	} else if (prob == 2) {
		printf("Нет корней.\n");
	} else {
		printf("%Lf\n", res);
	}

	if ((prob = binsearch(f3, 0.4, 100, 0.0001, &res)) == 1) {
		printf("Необходимо, чтобы функцяи на концах отрезков принимала значения разных знаков.\n");
	} else if (prob == 2) {
		printf("Нет корней.\n");
	} else {
		printf("%Lf\n", res);
	}

	if ((prob = binsearch(f4, -1, 1000, 0.0000000001, &res)) == 1) {
		printf("Необходимо, чтобы функцяи на концах отрезков принимала значения разных знаков.\n");
	} else if (prob == 2) {
		printf("Нет корней.\n");
	} else {
		printf("%Lf\n", res);
	}

    if ((prob = binsearch(f5, -1, 1000, 0.00001, &res)) == 1) {
		printf("Необходимо, чтобы функцяи на концах отрезков принимала значения разных знаков.\n");
	} else if (prob == 2) {
		printf("Нет корней.\n");
	} else {
		printf("%Lf\n", res);
	}
}
