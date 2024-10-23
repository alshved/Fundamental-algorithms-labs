#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

long double factorial(long double n) {
	long double res = 1;
	for (int i = 2; i <= n; i++) res *= i;
	return res;
}

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

void EWIthLimits(long double epsilon, long double* ans) {
	long double n = 1;
	long double first = powl((1 + 1.0 / n), n);
	long double second = powl((1 + 1.0 / (n + 1)), n + 1);
	while (epsilon < fabsl(first - second)) {
		n += 1;
		first = second;
		second = powl((1 + 1.0 / (n + 1)), n + 1);
	}
	*ans = second;
}

void EWithSumm(long double epsilon, long double* ans) {
	*ans = 0;
	int n = 0;
	long double elem = 1.0 / factorial(n);
	while (epsilon < elem) {
		*ans += elem;
		n += 1;
		elem = 1.0 / factorial(n);
	}
	*ans += elem;
}

void EWithEqu(long double epsilon, long double* ans) {
	long double l = -999, r = 1000;
	long double current = (l + r) / 2;
	while (epsilon < fabsl(l - r)) {
		if (log(current) > 1) {
			r = current;
		} else {
			l = current;
		}
		current = (l + r) / 2;
	}
	*ans = current;
}

void PiWIthLimits(long double epsilon, long double* ans) {
	long double n = 1;
	long double res = 4;
	long double prev = 0;
	while (fabs(res - prev) > epsilon) {
		prev = res;
		res *= 4.0 * ((n + 1) * n) / pow(2 * n + 1, 2);
		n++;
	}
	*ans = res;
}

void PiWithSumm(long double epsilon, long double* ans) {
	*ans = 0;
	long double n = 1;
	long double elem = powl(-1, n - 1) / (2 * n - 1);
	while (epsilon < fabsl(elem)) {
		*ans += elem;
		n += 1;
		elem = powl(-1, n - 1) / (2 * n - 1);
	}
	*ans += elem;
	*ans *= 4;
}

void PiWithEqu(long double epsilon, long double* ans) {
	long double l = 2.5, r = 3.5;
	long double current = (l + r) / 2;
	while (epsilon < fabsl(l - r)) {
		if (sin(current) > 0) {
			l = current;
		} else {
			r = current;
		}
		current = (l + r) / 2;
	}
	*ans = current;
}

void LnWIthLimits(long double epsilon, long double* ans) {
	long double n = 1;
	long double edeniza = 1;
	long double first = n * (powl(2, edeniza / n) - 1);
	long double second = (n + 1) * (powl(2, edeniza / (n + 1)) - 1);
	while (epsilon < fabsl(first - second)) {
		n += 1;
		first = second;
		second = (n + 1) * (powl(2, edeniza / (n + 1)) - 1);
	}
	*ans = second;
}

void LnWithSumm(long double epsilon, long double* ans) {
	*ans = 0;
	long double n = 1;
	long double elem = powl(-1, n - 1) / n;
	while (epsilon < fabsl(elem)) {
		*ans += elem;
		n += 1;
		elem = powl(-1, n - 1) / n;
	}
	*ans += elem;
}

void LnWithEqu(long double epsilon, long double* ans) {
	long double e = 0;
	EWithSumm(epsilon, &e);
	long double l = -999, r = 1000;
	long double current = (l + r) / 2;
	while (epsilon < fabsl(l - r)) {
		if (powl(e, current) - 2 > 0) {
			r = current;
		} else {
			l = current;
		}
		current = (l + r) / 2;
	}
	*ans = current;
}

void SqrWIthLimits(long double epsilon, long double* ans) {
	long double first = -0.5;
	long double second = first - (powl(first, 2) / 2) + 1;
	while (epsilon < fabsl(first - second)) {
		first = second;
		second = first - (powl(first, 2) / 2) + 1;
	}
	*ans = second;
}

void SqrWithCom(long double epsilon, long double* ans) {
	*ans = 1;
	long double n = 2;
	long double elem = powl(2, powl(2, -n));
	while (epsilon < fabsl(1 - elem)) {
		*ans *= elem;
		n += 1;
		elem = powl(2, powl(2, -n));
	}
	*ans *= elem;
}

void SqrWithEqu(long double epsilon, long double* ans) {
	long double l = -1000, r = 1000;
	long double current = (l + r) / 2;
	while (epsilon < fabsl(l - r)) {
		if (powl(current, 2) > 2) {
			r = current;
		} else {
			l = current;
		}
		current = (l + r) / 2;
	}
	*ans = current;
}

long double Summ(long double m) {
	long double sum = 0;
	for (long double i = 1; i <= m; ++i) {
		sum += (long double)(1 / i);
	}
	return sum - logl(m);
}

void GammaWIthLimits(long double epsilon, long double* ans) {
	long double m = 100;
	long double first = Summ(1);
	long double second = Summ(2);
	while (epsilon < fabsl(first - second)) {
		++m;
		first = second;
		second = Summ(m);
	}
	*ans = second;
}

long double GammaRowMember(long double n) {
	long double res = 0;
	for (int k = 1; k <= 2 * n; k++) {
		res += 1.0 / (n * n) * ((long double)k / ((long double)k + (long double)n * n));
	}
	return res;
}

void GammaWithSumm(long double epsilon, long double* ans) {
	long double Pi = acos(-1);
	long double result = -Pi * Pi / 6.0 + 1.0 / 2.0 + 2.0 / 3.0;
	long double k = 2;
	long double pred = 123456;
	long double elem = GammaRowMember(k);
	while (epsilon < fabsl(pred - result)) {
		pred = result;
		result += elem;
		++k;
		elem = GammaRowMember(k);
	}
	*ans = result;
}

void Sieve(bool* sv, long long n) {
	for (int i = 0; i < n; i++) {
		sv[i] = 1;
	}
	sv[0] = 1;
	sv[1] = 1;
	for (long long i = 2; i < n; i++) {
		if (!sv[i]) {
			continue;
		}

		for (long long j = 2; j * i < n; j++) {
			sv[i * j] = 0;
		}
	}
}

long double NextForEqu(long double tochnost) {
	bool* is_prime = (bool*)malloc(sizeof(bool) * (tochnost + 1));
	Sieve(is_prime, tochnost + 1);
	long double res = 1;
	for (int p = 2; p <= tochnost; p++) {
		if (is_prime[p]) {
			res *= (long double)(p - 1) / (long double)p;
		}
	}
	free(is_prime);
	return res * logl(tochnost);
}

void GammaWithEqu(long double epsilon, long double* ans) {
	long double first = NextForEqu(2);
	long double second = NextForEqu(3);
	int n = 4;
	while (fabsl(first - second) >= epsilon) {
		first = second;
		second = NextForEqu(n);
		n += 1;
	}
	*ans = second;
}

int main(int argc, char** argv) {
	long double epsilon = 0;
	if (argc != 2) {
		printf("%s ", "Incorrect option");
		return 1;
	}
	if (!ParseDoubleIsPossible(argv[1])) {
		printf("%s ", "Incorrect option");
		return 2;
	}
	epsilon = parseDouble(argv[1]);
	long double ans = 0;
	EWIthLimits(epsilon, &ans);
	printf("%.10Lf ", ans);
	EWithSumm(epsilon, &ans);
	printf("%.10Lf ", ans);
	EWithEqu(epsilon, &ans);
	printf("%.10Lf\n", ans);
	
	PiWIthLimits(epsilon, &ans);
	printf("%.10Lf ", ans);
	PiWithSumm(epsilon, &ans);
	printf("%.10Lf ", ans);
	PiWithEqu(epsilon, &ans);
	printf("%.10Lf\n", ans);
	LnWIthLimits(epsilon, &ans);
	printf("%.10Lf ", ans);
	LnWithSumm(epsilon, &ans);
	printf("%.10Lf ", ans);
	LnWithEqu(epsilon, &ans);
	printf("%.10Lf\n", ans);
	SqrWIthLimits(epsilon, &ans);
	printf("%.10Lf ", ans);
	SqrWithCom(epsilon, &ans);
	printf("%.10Lf ", ans);
	SqrWithEqu(epsilon, &ans);
	printf("%.10Lf\n", ans);
	GammaWIthLimits(epsilon, &ans);
	printf("%.10Lf ", ans);
	GammaWithSumm(epsilon, &ans);
	printf("%.10Lf ", ans);
	GammaWithEqu(epsilon, &ans);
	printf("%.10Lf\n", ans);
}