#pragma once
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	double *coordinates;
	size_t dimension;
} Vector;

void PrintVector(Vector *arr);
double NormInfinity(Vector *vec);
double NormP(Vector *vec);
double NormMatrix(Vector *vec);
int FindAllLongestVectors(Vector ***longest_vectors, size_t **size, size_t count_vectors,
                          size_t count_norm, ...);