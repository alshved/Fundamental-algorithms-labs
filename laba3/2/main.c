#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "func.h"

int main(){
    Vector* maxs = NULL;
    Vector vector1, vector2, vector3, vector4, vector5;
    int dimension = 3;
    vector1.coordinates = (double*)malloc(dimension * sizeof(double));
    vector1.coordinates[0] = 1.0;
    vector1.coordinates[1] = 2.0;
    vector1.coordinates[2] = 3.0;

    vector2.coordinates = (double*)malloc(dimension * sizeof(double));
    vector2.coordinates[0] = 24.0;
    vector2.coordinates[1] = 0.0;
    vector2.coordinates[2] = 1.0;

    vector3.coordinates = (double*)malloc(dimension * sizeof(double));
    vector3.coordinates[0] = 0.0;
    vector3.coordinates[1] = 0.0;
    vector3.coordinates[2] = 0.0;

    vector4.coordinates = (double*)malloc(dimension * sizeof(double));
    vector4.coordinates[0] = 24.0;
    vector4.coordinates[1] = 3.0;
    vector4.coordinates[2] = 3.0;

    vector5.coordinates = (double*)malloc(dimension * sizeof(double));
    vector5.coordinates[0] = 15.0;
    vector5.coordinates[1] = -7.0;
    vector5.coordinates[2] = 23.0;
    
    int size;
    int st_find = FindLongestVectors(&maxs, &size, NormInfinity, dimension, 5, vector1, vector2, vector3, vector4, vector5);
    if (st_find != 0) {
        printf("Invalid parameter!\n");
        return -2;
    }

    for (int i = 0; i != size; ++i) {
        PrintVector(&maxs[i]);
    }

    free(maxs);
    free(vector1.coordinates);
    free(vector2.coordinates);
    free(vector3.coordinates);
    free(vector4.coordinates);
    free(vector5.coordinates);
    return 0;
}