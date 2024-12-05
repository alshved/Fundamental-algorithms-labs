#pragma once

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct stNode
{
	token data;
	struct stNode *next;
} stNode;

typedef struct
{
	stNode *top;
} Stack;

void stInitialize(Stack *s);
bool stIsEmpty(Stack *s);
int stPush(Stack *s, token value);
token stPop(Stack *s);
token stTop(Stack *s);