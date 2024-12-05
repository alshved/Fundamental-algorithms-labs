#ifndef enum_h
#define enum_h

#include <stdio.h>
#include <string.h>
#include "token.h"
#include "queue.h"
#include "stack.h"
#include "token.h"
#include "../PrefixTree.h"

typedef enum read_tok_result{
	READ_TOK_SUCCESS = 0,
	READ_TOK_EOF = -1,
	READ_TOK_EOE = -2,
	READ_TOK_ERROR_UNEXP_CHAR = -3
} read_tok_result;

typedef enum read_exp_result{
	READ_EXP_SUCCESS = 0,
	READ_EXP_EOF = -1,
	READ_EXP_EOE = -2,
	READ_ERROR_UNEXP_TOKEN = -3,
	READ_ERROR_INVALID_CHAR = -4,
    READ_ERROR_WITH_RBR = -5
} read_exp_result;

typedef enum sort_res{
	SORT_SUCCESS = 0,
	SORT_BRACKET_ERROR,
	SORT_WRONG_OP
} sort_res;

read_tok_result read_token(token *out, tok_type prev, char *str, int base_assign, trieNode *root, HashTable* synonym);
int get_priority(char op);
char get_association(char op);
read_exp_result read_expression(Queue *q, char *str, int base_assign, trieNode *root, HashTable* synonym, HashTable* rev_synonym);
char *getNextWord(char *str);

#endif