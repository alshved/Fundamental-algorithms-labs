#pragma once

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readStringWithSpace(FILE *file);
char *removeComment(char *str);
char *readStringToSemicolon(FILE *file, int* error);
bool isValidVariableName(const char *name);
char *trimTrailingSpaces(char *str);
char **splitIntoBrackets(char *string);
bool isValidNumber(const char *str, int base_assign);
char **splitString(const char *str, const char *delimiter);
char *removeFirstAndLast(char *str);
char **splitStringComma(char *str);
unsigned long long fastModExp(unsigned long long base, unsigned long long exponent, unsigned long long modulus);
void reverseString(char *str);
char **splitIntoBracketsRev(char *string);