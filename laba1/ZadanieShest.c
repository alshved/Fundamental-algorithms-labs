#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool ParseDoubleIsPossible(char* str) {
	if (str[0] == '.') {
		return false;
	}
	int i = 0;
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

int GetOpts(int argc, char** argv, long double* epsilon) {
	if (argc != 2) {
		return 1;
	}
	if (!ParseDoubleIsPossible(argv[1])) {
		return 2;
	}
	*epsilon = parseDouble(argv[1]);
	return 0;
}

long double FuncForA(long double x) { return ((logl(1 + x) / x)); }

long double FuncForB(long double x) {
	long double e = 2.71828182845904523536028747135266249775724709369995;
	return (powl(e, -(x * x) / 2));
}

long double FuncForC(long double x){
    return (logl(1.0 / (1 - x)));
}

long double FuncForD(long double x){
    if (x > 0){
        return powl(x, x);
    } else {
        return 1;
    }
}

long double FindVal(long double h, long double (*func)(long double)) {
	long double answer = 0;
	for (long double i = 0; i <= 1; i += h) {
		answer += (func(fabsl(i - h / 2)));
	}
	return answer * h;
}

void IntValue(long double eps, long double* ans, long double (*func)(long double)) {
	int n = 1.0 / (2 * sqrtl(eps)) + 1;
	long double h = 1.0 / (long double)n;
	long double I1 = 0, I2 = 0;
	I1 = FindVal(h, func);
	I2 = FindVal(h / 2, func);
	while (fabs(I2 - I1) / 3 > eps) {
		h /= 2;
		I1 = I2;
		I2 = FindVal(h / 2, func);
	}
	*ans = I2;
}

int main(int argc, char** argv) {
	long double epsilon = 0;
	if (GetOpts(argc, argv, &epsilon)) {
		printf("%s", "Incorrect option");
		return 1;
	}
	if (epsilon == 0){
		printf("You can`t input zero.\n");
		return 2;
	}
	long double ans = 0;
	IntValue(epsilon, &ans, FuncForA);
	printf("%Lf\n", ans);
	IntValue(epsilon, &ans, FuncForB);
	printf("%Lf\n", ans);
	IntValue(epsilon, &ans, FuncForC);
	printf("%Lf\n", ans);
	IntValue(epsilon, &ans, FuncForD);
	printf("%Lf\n", ans);
	return 0;
}