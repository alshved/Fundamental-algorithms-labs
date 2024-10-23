#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

typedef enum Result {
    SUCCESS,
    INVALID_PARAMETR,
    MEMORY_ALLOCATING_ERROR,
    OVERFLOW
} Result;

Result toRoman(int number, char** r_value);
Result convertFromDecimal(long long int decimal_number, int base, int flag,  char** result);
Result convertToDecimal(char* number_str, int base, long long int * dec_number, int flag);
bool isLower(char* str, int size);
bool isUpper(char* str, int size);
int overfprintf(FILE * stream, char * format, ...);
int oversprintf(char* buf, char * format, ...);
