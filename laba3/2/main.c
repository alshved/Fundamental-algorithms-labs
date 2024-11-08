#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "func.h"

int main() {
	Vector** maxs = NULL;
	Vector vector1, vector2, vector3, vector4, vector5;
	int dimension = 3;
    size_t* size = NULL;
	vector1.coordinates = (double*)malloc(dimension * sizeof(double));
	vector1.coordinates[0] = 1.0;
	vector1.coordinates[1] = 2.0;
	vector1.coordinates[2] = 3.0;
	vector1.dimension = dimension;

	vector2.coordinates = (double*)malloc(dimension * sizeof(double));
	vector2.coordinates[0] = 24.0;
	vector2.coordinates[1] = 0.0;
	vector2.coordinates[2] = 1.0;
	vector2.dimension = dimension;

	vector3.coordinates = (double*)malloc(dimension * sizeof(double));
	vector3.coordinates[0] = 0.0;
	vector3.coordinates[1] = 0.0;
	vector3.coordinates[2] = 0.0;
	vector3.dimension = dimension;

	vector4.coordinates = (double*)malloc(dimension * sizeof(double));
	vector4.coordinates[0] = 24.0;
	vector4.coordinates[1] = 3.0;
	vector4.coordinates[2] = 3.0;
	vector4.dimension = dimension;

	vector5.coordinates = (double*)malloc(dimension * sizeof(double));
	vector5.coordinates[0] = 15.0;
	vector5.coordinates[1] = -7.0;
	vector5.coordinates[2] = 23.0;
	vector5.dimension = dimension;

	int st_find = FindAllLongestVectors(&maxs, &size, 5, 3, vector1, vector2, vector3, vector4, vector5,
	                                    NormInfinity, NormMatrix, NormP);
	if (st_find != 0) {
		printf("Invalid parameter!\n");
		return -2;
	}
	free(vector1.coordinates);
	free(vector2.coordinates);
	free(vector3.coordinates);
	free(vector4.coordinates);
	free(vector5.coordinates);

	for (size_t i = 0; i < 3; ++i) {
		printf("Norm number %ld\n", i + 1);
		for (size_t j = 0; j < size[i]; ++j) {
			PrintVector(&maxs[i][j]);
		}
	}

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < size[i]; ++j) {
			free(maxs[i][j].coordinates);
		}
        free(maxs[i]);
	}
	free(size);
	free(maxs);
	return 0;
}