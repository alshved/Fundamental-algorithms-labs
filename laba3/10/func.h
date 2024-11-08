#pragma once
#include <stdio.h>

typedef enum kOpt{
	OPT_SUCCESS,
	OPT_FAIL,
	OPT_ERROR,
	OPT_ERROR_MEMORY,
	OPT_ERROR_INPUT
} kOpt;

typedef struct Node {
	char value;
	struct Node** children;
	int child_count;
	int child_capacity;
} Node;

kOpt CheckExpression(const char *expr);
void PrintTree(Node *root, FILE *output, int x);
void FreeTree(Node *root);
Node* CreateNode(char value);
Node* ParseExpression(const char** expr);
kOpt ValidateInput(int argc, char **argv);