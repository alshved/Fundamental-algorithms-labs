#ifndef token_h
#define token_h

typedef enum {
	TOK_OP,
	TOK_CONST,
	TOK_LBR,
	TOK_RBR,
	TOK_NONE
} tok_type;

typedef struct {
	unsigned long long const_var;
	char op;
	tok_type type;
} token;

void print_token(token t);

#endif