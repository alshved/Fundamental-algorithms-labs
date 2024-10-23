#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

typedef struct {
    int line;
    int index;
} MatchResult;

void computeLPSArray(const char* pat, int M, int* lps);
KMPErrors KMPSearch(MatchResult** result, const char* pat, const char* txt, int* count);