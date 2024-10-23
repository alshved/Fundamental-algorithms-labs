#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

typedef struct {
    char* data;
    int capacity;
} String;

StringResult StringInit(String* str, int init);
void StringFree(String* str);
int StringLength(const String* str) ;
StringResult StringSet(String* str, const char* text);
StringResult StringConcat(String* str, const char* text);
void StringPrint(const String* str);
int Length(const char* str);
void StringReverse(String* str);
