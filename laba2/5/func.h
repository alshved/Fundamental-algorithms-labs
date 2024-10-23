#pragma once
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

Result to_roman(int number, char** r_value);
char* print_binary_to_string(const void* data, size_t size);
char* dump_signed_int(int value);
char* dump_unsigned_int(unsigned int value);
char* dump_double(double value);
char* dump_float(float value);
void zeckendorf(unsigned int num, int* count, char** result);
Result convert_from_decimal(long long int decimal_number, int base, int flag,  char** result);
Result convert_to_decimal(char* number_str, int base, long long int * dec_number, int flag);
bool is_lower(char* str, int size);
bool is_upper(char* str, int size);
int overfprintf(FILE * stream, char * format, ...);
int oversprintf(char* buf, char * format, ...);