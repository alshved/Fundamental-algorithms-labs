#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum Opts { OPT_Q, OPT_M, OPT_T } Opts;

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

bool ParseIntIsPossible(char* str) {
	int i = 0;
	if (str[0] == '-') {
		i++;
	}
	for (; str[i] != '\0'; ++i) {
		if (str[i] < 48 || str[i] > 57) {
			return false;
		}
	}
	return true;
}

int ParseInt(char* str) {
	int result = 0.0;
	int sign = 1;
	if (*str == '-') {
		sign = -1;
		str++;
	}
	while (*str) {
		result = result * 10.0 + (*str - '0');
		str++;
	}
	return result;
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

int ParseForQandT(char** argv, long double* eps, long double* a, long double* b, long double* c) {
	if (!(ParseDoubleIsPossible(argv[2]) && ParseDoubleIsPossible(argv[3]) && ParseDoubleIsPossible(argv[4]) &&
	      ParseDoubleIsPossible(argv[5]))) {
		return 1;
	}
	*eps = parseDouble(argv[2]);
	*a = parseDouble(argv[3]);
	*b = parseDouble(argv[4]);
	*c = parseDouble(argv[5]);
	return 0;
}

void SolveEqu(long double eps, long double a, long double b, long double c) {
	if (a == 0){
		return;
	}
	long double disk = b * b - 4 * a * c;
	if (disk + eps < 0) {
		return;
	} else if (disk <= eps) {
		printf("%Lf\n", -b / (2 * a));
	} else {
		printf("%Lf ", ((-b + sqrtl(disk)) / (2 * a)));
		printf("%Lf\n", ((-b - sqrtl(disk)) / (2 * a)));
	}
}

void SolveT(long double eps, long double a, long double b, long double c) {
	if (a * a + b * b - c * c < eps && a * a + b * b - c * c > -eps) {
		printf("YES\n");
		return;
	}
	if (b * b + c * c - a * a < eps && b * b + c * c - a * a > -eps) {
		printf("YES\n");
		return;
	}
	if (a * a + c * c - b * b < eps && a * a + c * c - b * b > -eps) {
		printf("YES\n");
		return;
	}
	printf("NO\n");
	return;
}

void SolveQ(long double eps, long double a, long double b, long double c) {
	SolveEqu(eps, a, b, c);
	SolveEqu(eps, a, c, b);
	SolveEqu(eps, b, a, c);
	SolveEqu(eps, b, c, a);
	SolveEqu(eps, c, a, b);
	SolveEqu(eps, c, b, a);
}

void SolveM(int a, int b) {
	if (a % b == 0) {
		printf("Yes\n");
	} else {
		printf("No\n");
	}
}

int GetOpts(int argc, char** argv, Opts* option) {
	if (argc != 6 && argc != 4) {
		return 1;
	}
	if (argc == 4) {
		if ((argv[1][0] != '-' && argv[1][0] != '/') || argv[1][1] != 'm' || argv[1][2] != '\0') {
			return 2;
		}
		int a, b;
		a = ParseInt(argv[2]);
		b = ParseInt(argv[3]);
		if (a == 0 || b == 0) {
			return 6;
		}
		SolveM(a, b);
	}
	if (argc == 6) {
		if ((argv[1][0] != '-' && argv[1][0] != '/') || (argv[1][1] != 'q' && argv[1][1] != 't') ||
		    argv[1][2] != '\0') {
			return 6;
		}
		if (argv[1][1] == 'q') {
			long double eps, a, b, c;
			if (ParseForQandT(argv, &eps, &a, &b, &c) != 0) {
				return 7;
			}
			SolveQ(eps, a, b, c);
		}
		if (argv[1][1] == 't') {
			long double eps, a, b, c;
			if (ParseForQandT(argv, &eps, &a, &b, &c) != 0) {
				return 7;
			}
			if (a == 0 || b == 0 || c == 0) return 8;
			SolveT(eps, a, b, c);
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	Opts opt = 0;

	if (GetOpts(argc, argv, &opt)) {
		printf("%s", "Incorrect option");
		return 1;
	}
	return 0;
}