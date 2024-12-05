#pragma once
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"


typedef struct QNode {
	token data;
	struct QNode *next;
} QNode;

typedef struct {
	QNode *head;
	QNode *tail;
} Queue;

void queInitialize(Queue *q);
bool isEmpty(Queue *q);
int quePush(Queue *q, token value);
token quePop(Queue *q);
token queTop(Queue *q);
void queDestroy(Queue *q);
