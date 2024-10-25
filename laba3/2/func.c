#include "func.h"

void PrintVector(Vector *arr) {
	printf("<");
	for (int i = 0; i < arr->dimension; i++) {
		printf("%.3lf", arr->coordinates[i]);
		if (i != arr->dimension - 1) {
			printf(", ");
		}
	}
	printf(">");
	printf("\n");
}

double NormInfinity(Vector *vec) {
	double max_val = fabs(vec->coordinates[0]);
	for (int i = 1; i != vec->dimension; ++i) {
		if (fabs(vec->coordinates[i]) > max_val) {
			max_val = fabs(vec->coordinates[i]);
		}
	}
	return max_val;
}

double NormP(Vector *vec) {
	double sum = 0.0;
	for (int i = 0; i != vec->dimension; ++i) {
		sum += pow(fabs(vec->coordinates[i]), 2);
	}
	return pow(sum, 1.0 / 2);  // P = 2
}

double NormMatrix(Vector *vec) {
	double A[vec->dimension][vec->dimension];
	for (int i = 0; i != vec->dimension; ++i) {
		for (int j = 0; j != vec->dimension; ++j) {
			if (i == j) {
				A[i][j] = 1.0;
			} else {
				A[i][j] = 0.0;
			}
		}
	}

	double result = 0.0;
	for (int i = 0; i != vec->dimension; ++i) {
		double temp = 0.0;
		for (int j = 0; j != vec->dimension; ++j) {
			temp += A[i][j] * vec->coordinates[j];
		}
		result += temp * vec->coordinates[i];
	}
	return sqrt(result);
}

void Fill(int size, char *string, char element) {
	for (int i = 0; i != size; ++i) {
		string[i] = '\0';
	}
}

int FindLongestVectors(Vector **longest_vectors, int *size, double norm(Vector *vec), int dimension, int count, ...) {
	if (count < 1 || dimension < 1) {
		return 1;
	}
	const double epsilon = 1e-10;

	va_list ptr;
	va_start(ptr, count);
	Vector vectors[count];
	for (int i = 0; i != count; ++i) {
		vectors[i] = va_arg(ptr, Vector);
		vectors[i].dimension = dimension;
	}
	va_end(ptr);
	char longest_vectors_mask[count];  // Маска для поиска самых длинных векторов
	Fill(count, longest_vectors_mask, '\0');

	double max = 0.0;
	int index_max = 0;
	int count_max_len_vectors = 0;
	for (int i = 0; i != count; ++i) {
		double current_norm = norm(&vectors[i]);
		if (isinf(current_norm) || isnan(current_norm)) {
			return 2;
		}
		if (current_norm - max > epsilon) {
			max = current_norm;
			index_max = 1;
			Fill(count, longest_vectors_mask, '\0');
			longest_vectors_mask[i] = (char)1;
			count_max_len_vectors = 1;
		} else if (fabs(current_norm - max) < epsilon) {
			// Если найдена такая же максимальная норма, добавляем к маске
			longest_vectors_mask[i] = (char)1;
			count_max_len_vectors++;
		}
	}
	*size = count_max_len_vectors;
	*longest_vectors = malloc(sizeof(Vector) * count_max_len_vectors);
	if (*longest_vectors == NULL) {
		return 3;
	}

	int index = 0;
	for (int i = 0; i < count; ++i) {
		if (longest_vectors_mask[i]) {
			(*longest_vectors)[index++] = vectors[i];
		}
	}
	return 0;
}