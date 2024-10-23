#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для вычисления производной полинома
int ComputePolynomialDerivative(double* derivativeCoeffs, int degree, double* originalCoeffs) {
	for (int i = 1; i <= degree; i++) {
		double currentCoeff = originalCoeffs[i];     // Получаем текущий коэффициент
		derivativeCoeffs[i - 1] = currentCoeff * i;  // Производная: умножаем на степень
	}
	return 0;  
}

int EvaluatePolynomial(int degree, double xValue, double* coefficients, double* result) {
	double evaluationResult = 0.0;
	// Цикл для вычисления значения полинома по схеме Горнера
	for (int i = degree; i > 0; i--) {
		evaluationResult += coefficients[i];
		if (fabsl(evaluationResult) > DBL_MAX / fabsl(xValue)) {
			return 4;
		}
		evaluationResult *= xValue;
	}
	evaluationResult += coefficients[0];
	*result = evaluationResult;
	return 0;                    
}

// Функция для нахождения ряда Тейлора полинома
int ComputeTaylorSeries(double epsilon, double expansionPoint, double* taylorCoeffs, int degree, ...) {
	double* originalCoeffs = (double*)calloc(degree + 1, sizeof(double));
	if (originalCoeffs == NULL) {
		return 5;
	}
	double* derivativeCoeffs = (double*)calloc(degree + 1, sizeof(double));
	if (derivativeCoeffs == NULL) {
		free(originalCoeffs);
		return 5;
	}

	va_list args;
	va_start(args, degree);
	for (int i = 0; i <= degree; i++) {
		originalCoeffs[i] = va_arg(args, double);
	}
	va_end(args);  // Завершение работы со списком аргументов

	// Вычисление значения полинома в точке разложения
	int status = EvaluatePolynomial(degree, expansionPoint, originalCoeffs, taylorCoeffs);
	if (status != 0) {
		free(originalCoeffs);
		free(derivativeCoeffs);
		return status;
	}

	double factorial = 1;
	// Цикл для вычисления каждого члена ряда Тейлора
	for (int i = 1; i <= degree; i++) {
		factorial *= i;  // Вычисляем факториал
		// Вычисляем коэффициенты производной
		status = ComputePolynomialDerivative(derivativeCoeffs, degree, originalCoeffs);
		if (status != 0) {
			free(originalCoeffs);
			free(derivativeCoeffs);
			return status;
		}
		derivativeCoeffs[degree - i + 1] = 0.0;  // Обнуляем старшую степень производной
		// Вычисление значения производной в точке разложения
		double derivativeValue;
		status = EvaluatePolynomial(degree, expansionPoint, derivativeCoeffs, &derivativeValue);
		taylorCoeffs[i] = derivativeValue / factorial;  // Сохраняем нормализованное значение производной
		if (status != 0) {
			free(originalCoeffs);
			free(derivativeCoeffs);
			return status;
		}
		// Копируем коэффициенты производной в оригинальные для следующей итерации
		memcpy(originalCoeffs, derivativeCoeffs, sizeof(double) * degree);
	}
	free(originalCoeffs);
	free(derivativeCoeffs);
	return 0;  
}

int PrintPolynomial(double* coefficients, int degree, double expansionPoint) {
	printf("g(x) = ");
	// Цикл для вывода каждого члена полинома
	for (int i = 0; i <= degree; i++) {
		if (i > 0 && coefficients[i] >= 0) {
			printf(" + ");
		} else if (i > 0) {
			printf(" - ");
			coefficients[i] = -coefficients[i];  // Переводим в положительное для печати
		}

		// Печать свободного члена
		if (i == 0) {
			printf("%.2f", coefficients[i]);
		} else {
			// Печать членов полинома в зависимости от точки разложения
			if (expansionPoint > 0) {
				printf("%.2f * (x - %.2f)^%d", coefficients[i], expansionPoint, i);
			} else if (expansionPoint == 0) {
				printf("%.2f * (x)^%d", coefficients[i], i);
			} else {
				printf("%.2f * (x + %.2f)^%d", coefficients[i], fabs(expansionPoint), i);
			}
		}
	}
	printf("\n");
	return 0;      
}

int main() {
	double coefficients1[5] = {0.0, 0.0, 5.0};
	double taylorCoeffs1[5];
	ComputeTaylorSeries(1e-6, -2.5, taylorCoeffs1, 2, 0.0, 0.0, 5.0);
	PrintPolynomial(taylorCoeffs1, 2, -2.5);
	double res = 0;
	EvaluatePolynomial(2, 1, coefficients1, &res);
	printf("%lf\n", res);
	res = 0;
	EvaluatePolynomial(2, -1.5, taylorCoeffs1, & res);
	printf("%lf\n", res);

	EvaluatePolynomial(2, 2, coefficients1, &res);
	printf("%lf\n", res);
	res = 0;
	EvaluatePolynomial(2, -0.5, taylorCoeffs1, & res);
	printf("%lf\n", res);
	

	double coefficients2[5] = {5.0, 1.0, 2.0, 3.0};
	double taylorCoeffs2[5];
	ComputeTaylorSeries(1e-6, 0.0, taylorCoeffs2, 3, 5.0, 1.0, 2.0, 3.0);
	PrintPolynomial(taylorCoeffs2, 3, 0.0);

	double coefficients3[5];
	double taylorCoeffs3[5];
	ComputeTaylorSeries(1e-6, 1.0, taylorCoeffs3, 4, -7.0, 0.0, 4.0, 0.0, -1.0);
	PrintPolynomial(taylorCoeffs3, 4, 1.0);

	double coefficients4[5];
	double taylorCoeffs4[5];
	ComputeTaylorSeries(1e-6, 3.0, taylorCoeffs4, 1, -2.0, 1.0);
	PrintPolynomial(taylorCoeffs4, 1, 3.0);

	return 0;
}