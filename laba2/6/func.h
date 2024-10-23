#pragma once
#include <stdio.h>
#include <stdlib.h>

int overFscanf(FILE *stream, const char *format, ...);
int overSscanf(const char *str, const char *format, ...);
int baseToInt(const char *str, int base);
unsigned int zeckendorfToInt(const char *zeckendorf);
int romanToInt(const char *roman);
int romanCharToInt(char c);