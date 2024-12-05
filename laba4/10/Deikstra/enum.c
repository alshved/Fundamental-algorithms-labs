#include "enum.h"

#include "../PrefixTree.h"
#include "../Settings/help.h"

char *getNextWord(char *str) {
	static char *token = NULL;  // static to maintain state between calls
	if (str == NULL) {
		return NULL;  // handle initial NULL
	}
	if (token == NULL) {
		token = strtok(str, " \t\n");  // first call
	} else {
		token = strtok(NULL, " \t\n");  // subsequent calls
	}
	return token;
}

read_tok_result read_token(token *out, tok_type prev, char *str, int base_assign, trieNode *root, HashTable *synonym) {
	if (str == NULL || *str == '\0') {
		return READ_TOK_EOF;
	}
	char *op;
	op = getNextWord(str);
	if (op == NULL) {
		return READ_TOK_EOF;
	}

	if (strcmp(op, "(") == 0) {
		out->type = TOK_LBR;
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, ")") == 0) {
		out->type = TOK_RBR;
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "add")) == 0) {
		out->type = TOK_OP;
		out->op = '+';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "div")) == 0) {
		out->type = TOK_OP;
		out->op = '/';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "mult")) == 0) {
		out->type = TOK_OP;
		out->op = '*';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "pow")) == 0) {
		out->type = TOK_OP;
		out->op = '^';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "not")) == 0) {
		out->type = TOK_OP;
		out->op = '~';
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "sub")) == 0) {
		out->type = TOK_OP;
		out->op = '-';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "rem")) == 0) {
		out->type = TOK_OP;
		out->op = '%';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "xor")) == 0) {
		out->type = TOK_OP;
		out->op = '$';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "and")) == 0) {
		out->type = TOK_OP;
		out->op = '&';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}
	if (strcmp(op, search(synonym, "or")) == 0) {
		out->type = TOK_OP;
		out->op = '|';
		if (prev == TOK_OP) {
			return READ_ERROR_INVALID_CHAR;
		}
		return READ_TOK_SUCCESS;
	}

	if (isValidNumber(op, base_assign)) {
		unsigned long long value = strtoull(op, NULL, base_assign);
		out->type = TOK_CONST;
		out->const_var = value;
		return READ_TOK_SUCCESS;
	}
	if (treeSearch(root, op) != ULLONG_MAX) {
		out->type = TOK_CONST;
		out->const_var = treeSearch(root, op);
		return READ_TOK_SUCCESS;
	}
	return READ_ERROR_UNEXP_TOKEN;
}

int get_priority(char op) {
	switch (op) {
		case '%':
			return 5;
		case '$':
			return 2;
		case '&':
			return 3;
		case '|':
			return 1;
		case '~':
			return 7;
		case '^':
			return 6;
		case '*':
			return 5;
		case '/':
			return 5;
		case '+':
			return 4;
		case '-':
			return 4;
	}
}

char get_association(char op) {
	if (op == '^' || op == '~') {
		return 'r';
	}
	return 'l';
}

read_exp_result read_expression(Queue *q, char *str, int base_assign, trieNode *root, HashTable *synonym,
                                HashTable *rev_synonym) {
	tok_type prev = TOK_NONE;
	read_tok_result status = READ_TOK_SUCCESS;
	while (status != READ_TOK_EOF && status != READ_TOK_EOE) {
		token a;
		a.type = TOK_NONE;
		status = read_token(&a, prev, str, base_assign, root, synonym);
		if (status == READ_TOK_SUCCESS) {
			quePush(q, a);
			prev = a.type;
		} else if (status == READ_TOK_EOE || status == READ_TOK_EOF) {
			return READ_EXP_SUCCESS;
		} else {
			return READ_ERROR_INVALID_CHAR;
		}
	}
	return READ_EXP_SUCCESS;
}