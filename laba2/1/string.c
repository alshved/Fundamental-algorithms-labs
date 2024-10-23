#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "string.h"

StringResult StringInit(String* str, int init) {
    str->data = (char*)malloc(init * sizeof(char));
    if (str->data == NULL) {
        return MEMORY_ALLOCATING_ERROR;
    }
    str->data[0] = '\0';
    str->capacity = init;
    return SUCCESS;
}

void StringFree(String* str) {
    free(str->data);
    str->data = NULL;
    str->capacity = 0;
}

int StringLength(const String* str) {
    int len = 0;
    while (str->data[len] != '\0') {
        len++;
    }
    return len;
}

int Length(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void StringReverse(String* str) {
    int len = StringLength(str);
    int left = 0;
    int right = len - 1;
    while (left < right) {
        char temp = str->data[left];
        str->data[left] = str->data[right];
        str->data[right] = temp;
        left++;
        right--;
    }
}

StringResult StringSet(String* str, const char* text) {
    if (str == NULL || text == NULL) {
        return MEMORY_ALLOCATING_ERROR;
    }

    int textLen = Length(text);
    
    if (textLen + 1 > str->capacity) {
        char* tmp = (char*)realloc(str->data, (textLen + 1) * 2 * sizeof(char));
        if (tmp == NULL) {
            return MEMORY_ALLOCATING_ERROR;
        }
        str->data = tmp;
        str->capacity = (textLen + 1) * 2;
    }

    for (int i = 0; i < textLen; i++) {
        str->data[i] = text[i];
    }
    str->data[textLen] = '\0';

    return SUCCESS;
}


StringResult StringConcat(String* str, const char* text) {
    int currentLen = StringLength(str);
    int textLen = Length(text);
    if (currentLen + textLen + 1 > str->capacity) {
        char* tmp = (char*)realloc(str->data, (currentLen + textLen + 1) * 2 * sizeof(char));
        if (tmp == NULL) {
            StringFree(str);
            return MEMORY_ALLOCATING_ERROR;
        }
        str->data= tmp;
        str->capacity = currentLen + textLen + 1;
    }
    for (int i = 0; i < textLen; i++) {
        str->data[currentLen + i] = text[i];
    }
    str->data[currentLen + textLen] = '\0';
    return SUCCESS;
}

void StringPrint(const String* str) {
    printf("%s\n", str->data);
}