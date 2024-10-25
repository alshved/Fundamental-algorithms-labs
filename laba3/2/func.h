#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>

typedef struct {
    double *coordinates;
    int dimension;
} Vector;

void PrintVector(Vector* arr);
double NormInfinity(Vector *vec);
double NormP(Vector *vec);
double NormMatrix(Vector *vec);
int FindLongestVectors(Vector **longest_vectors, int *size, double norm(Vector *vec), int dimension, int count, ...);