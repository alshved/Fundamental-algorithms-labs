#include "func.h"

typedef double (*func_ptr)(Vector *);

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
	double** A = malloc(sizeof(double*) * vec->dimension);
	for (int i = 0; i != vec->dimension; ++i) {
		A[i] = malloc(sizeof(double) * vec->dimension);
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

	for (size_t i = 0; i != vec->dimension; ++i){
		free(A[i]);
	}
	free(A);

	return sqrt(result);
}

void Fill(int size, char *string, char element) {
	for (int i = 0; i != size; ++i) {
		string[i] = '\0';
	}
}

Vector CopyVector(Vector *vec) {
	Vector tmp;
	tmp.dimension = vec->dimension;
	tmp.coordinates = malloc(sizeof(double) * vec->dimension);
	if (!tmp.coordinates) {
		return (Vector) {0};
	}
	for (size_t i = 0; i < vec->dimension; ++i) {
		tmp.coordinates[i] = vec->coordinates[i];
	}
	return tmp;
}

int FindAllLongestVectors(Vector ***longest_vectors, size_t **size, size_t count_vectors, size_t count_norm, ...) {
	if (count_norm < 1 || count_norm > 3) {
		return 1;
	}
	Vector *vectors = malloc(sizeof(Vector) * count_vectors);
	if (!vectors) {
		return 1;
	}
	va_list ptr;
	va_start(ptr, count_norm);

	for (size_t i = 0; i != count_vectors; ++i) {
		vectors[i] = va_arg(ptr, Vector);
	}
	func_ptr *func = malloc(sizeof(func_ptr) * count_norm);
	*size = malloc(sizeof(size_t) * count_norm);

	if (!func || !(*size)) {
		free(vectors);
		free(func);
		free(*size);
		return 1;
	}

	for (size_t i = 0; i != count_norm; ++i) {
		func[i] = va_arg(ptr, func_ptr);
	}

	va_end(ptr);

	const double epsilon = 1e-10;
	*longest_vectors = malloc(sizeof(Vector *) * count_norm);
	if (!longest_vectors) {
		return 1;
	}
	for (int i = 0; i != count_norm; ++i) {
		char *longest_vectors_mask = malloc(sizeof(char) * count_vectors);
		if (!longest_vectors_mask) {
			return 1;
		}
		Fill(count_vectors, longest_vectors_mask, 0);

		double max = 0.0;
		size_t count_max_len_vectors = 0;

		for (int j = 0; j < count_vectors; ++j) {
			double current_norm = (func[i])(&vectors[j]);
			if (isinf(current_norm) || isnan(current_norm)) {
				free(longest_vectors_mask);
				return 2;
			}
			if (current_norm - max > epsilon) {
				max = current_norm;
				Fill(count_vectors, longest_vectors_mask, 0);
				longest_vectors_mask[j] = 1;
				count_max_len_vectors = 1;
			} else if (fabs(current_norm - max) < epsilon) {
				longest_vectors_mask[j] = 1;
				count_max_len_vectors++;
			}
		}
		 (*longest_vectors)[i] = malloc(sizeof(Vector) * count_max_len_vectors);
		if ((*longest_vectors)[i] == NULL) {
			free(longest_vectors_mask);
			return 3;
		}
		(*size)[i] = count_max_len_vectors;
		int index = 0;
		for (int j = 0; j < count_vectors; ++j) {
			if (longest_vectors_mask[j]) {
				(*longest_vectors)[i][index++] = CopyVector(&vectors[j]);
			}
		}
		free(longest_vectors_mask);
	}
	free(vectors);
	free(func);
	return 0;
}
