#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void stInitialize(Stack *s) { s->top = NULL; }

// Проверка на пустоту
bool stIsEmpty(Stack *s) { return s->top == NULL; }

// Добавление элемента в стек (push)
int stPush(Stack *s, token value) {
	stNode *newNode = (stNode *)malloc(sizeof(stNode));
	if (newNode == NULL) {
		printf("Memory allocation failed!\n");
		return 1;
	}
	newNode->data = value;
	newNode->next = s->top;
	s->top = newNode;
	return 0;
}

// Удаление элемента из стека (pop)
token stPop(Stack *s) {
	token value = s->top->data;
	stNode *temp = s->top;
	s->top = s->top->next;
	free(temp);
	return value;
}

// Получение верхнего элемента стека (peek)
token stTop(Stack *s) {
	return s->top->data;
}