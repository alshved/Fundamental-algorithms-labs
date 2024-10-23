#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool ParseDoubleIsPossible(char* str) {
	if (str[0] == '.') {
		return false;
	}
	int i = 0;
	if (str[0] == '-') {
		i++;
		if (str[1] == '.') {
			return false;
		}
	}
	int count_dot = 0;
	for (; str[i] != '\0'; ++i) {
		if (str[i] < 48 || str[i] > 57) {
			if (str[i] == 46) {
				if (count_dot == 0) {
					count_dot++;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}
	return true;
}

long double parseDouble(char* str) {
	long double result = 0.0;    // целая часть
	long double fraction = 0.0;  // дробная часть
	int sign = 1;                // для обработки отрицательных чисел
	int afterDecimal = 0;        // флаг для отслеживания дробной части
	long double divisor = 1.0;   // делитель для дробной части
	if (*str == '-') {
		sign = -1;
		str++;
	}
	// Обрабатываем целую часть и дробную часть
	while (*str) {
		if (*str >= '0' && *str <= '9') {
			if (!afterDecimal) {
				result = result * 10.0 + (*str - '0');
			} else {
				fraction = fraction * 10.0 + (*str - '0');
				divisor *= 10.0;
			}
		} else if (*str == '.') {
			afterDecimal = 1;
		}
		str++;
	}
	// Складываем целую и дробную части, применяя делитель
	result += fraction / divisor;
	// Применяем знак
	return sign * result;
}

int GetOpts(int argc, char** argv, long double* epsilon, long double* x) {
	if (argc != 3) {
		return 1;
	}
	if (!ParseDoubleIsPossible(argv[1])) {
		return 2;
	}
	if (!ParseDoubleIsPossible(argv[2])) {
		return 3;
	}
	*epsilon = parseDouble(argv[1]);
	*x = parseDouble(argv[2]);
	return 0;
}

void SummA(long double eps, long double x, long double* ans) {
	long double el = 1;
	long double n = 1;
	*ans = 1;
	do {
		el *= (x / n);
		*ans += el;
		n++;
	} while (fabsl(el) > eps);
}

void SummB(long double eps, long double x, long double* ans) {
	long double el = 1;
	long double n = 0;
	*ans = 1;
	do {
		el *= ((-1) * powl(x, 2) / (2 * n + 1) / (2 * n + 2));
		*ans += el;
		n++;
	} while (fabsl(el) > eps);
}

void SummC(long double eps, long double x, long double* ans) {
	long double el = 1;
	long double n = 0;
	*ans = 1;
	long double cons = 9 * powl(x, 2);
	do {
		el *= cons * ((n * n + 2 * n + 1) / (9 * n * n + 9 * n + 2));
		*ans += el;
		n++;
	} while (fabsl(el) > eps);
}

void SummD(long double eps, long double x, long double* ans) {
	*ans = -(x * x) / 2;
	int n = 2;
	long double elem = -(x * x) / 2;
	do {
		elem *= powl(x, 2) * (2 * n - 1) / (2 * n);
		elem *= (n % 2 == 0) ? 1 : -1;
		*ans += elem;
		n++;
	} while (fabsl(elem) > eps);
}

int main(int argc, char** argv) {
	long double epsilon = 0;
	long double x = 0;
	if (GetOpts(argc, argv, &epsilon, &x)) {
		printf("Incorrect option.\n");
		return 1;
	}
	if (epsilon <= 0) {
		printf("Incorrect option.\n");
		return 2;
	}
	if (fabsl(x) > 10000) {
		printf("You can`t input this X.\n");
		return 3;
	}
	long double ans = 0;

	SummA(epsilon, x, &ans);
	printf("%.20Lf\n", ans);

	SummB(epsilon, x, &ans);
	printf("%.20Lf\n", ans);

	if (fabsl(x) >= 1) {
		printf("inf\n");
	} else {
		SummC(epsilon, x, &ans);
		printf("%.20Lf\n", ans);
	}

	if (fabsl(x) >= 1) {
		printf("inf\n");
	} else {
		SummD(epsilon, x, &ans);
		printf("%.20Lf\n", ans);
	}

	return 0;
}