#include "func.h"
#include <stdlib.h>
#include <string.h>

kOpt CheckExpression(const char *expr){
	int count = 0;
	while (*expr){
		switch (*expr){
			case '(':
				count++;
				break;
			case ')':
				count--;
				break;
		}
		expr++;
	}
	if (!count){
		return OPT_SUCCESS;
	}
	return OPT_ERROR_INPUT;
}

Node* CreateNode(char value) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode){
		fprintf(stderr, "Error: Failed to allocate memory for new node\n");
		return NULL;
	}

	newNode->value = value;
	newNode->child_count = 0;
	newNode->child_capacity = 2;
	newNode->children = (Node**)malloc(newNode->child_capacity * sizeof(Node*));
	if (!newNode->children){
		fprintf(stderr, "Error: Failed to allocate memory for children array\n");
		free(newNode);
		return NULL;
	}
	return newNode;
}

kOpt AddChild(Node* parent, Node* child) {
	if (parent->child_count == parent->child_capacity) {
		parent->child_capacity *= 2;
		Node** newChildren = (Node**)realloc(parent->children, parent->child_capacity * sizeof(Node*));
		if (newChildren == NULL) {
			fprintf(stderr, "Error: Failed to reallocate memory for children array\n");
			return OPT_ERROR_MEMORY;
		}
		parent->children = newChildren;
	}
	parent->children[parent->child_count++] = child;
	return OPT_SUCCESS;
}

void FreeTree(Node* root) {
	if (root == NULL) return;
	for (int i = 0; i < root->child_count; i++) {
		FreeTree(root->children[i]);
	}
	free(root->children);
	free(root);
}

Node* ParseExpression(const char** expr) {
	if (**expr == '\0' || **expr == ')') return NULL;

	Node* root = CreateNode(**expr);
	if (!root) return NULL;

	(*expr)++;

	if (**expr == '(') {
		(*expr)++;
		while (**expr != '\0' && **expr != ')') {
			Node* child = ParseExpression(expr);
			if (child) {
				if (AddChild(root, child) != OPT_SUCCESS){
					FreeTree(root);
					return NULL;
				}
			}else{
				FreeTree(root);
				return NULL;
			}
			if (**expr == ',') (*expr)++;
		}
		(*expr)++;
	}
	return root;
}


void PrintTree(Node* root, FILE* file, int depth) {
	if (root == NULL) return;

	for (int i = 0; i < depth; i++) {
		fprintf(file, "  ");
	}

	fprintf(file, "%c\n", root->value);

	for (int i = 0; i < root->child_count; i++) {
		PrintTree(root->children[i], file, depth + 1);
	}
}

kOpt ValidateInput(int argc, char **argv){
	char full_in[BUFSIZ], full_out[BUFSIZ];
	if (argc != 3) return OPT_ERROR_INPUT;

	if (!realpath(argv[1], full_in) || !realpath(argv[2], full_out)) return OPT_ERROR_INPUT;
	if (!strcmp(full_in, full_out)) return OPT_ERROR_INPUT;

	return OPT_SUCCESS;
}