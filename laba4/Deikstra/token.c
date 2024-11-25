#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
#include "stack.h"


void print_token(token t) {
	switch (t.type) {
	case TOK_CONST:
		printf("%llu ", t.const_var);
		return;
		
	case TOK_OP:
	case TOK_LBR:
	case TOK_RBR:
		printf("%c ", t.op);
		return;

	case TOK_NONE:
		printf("Print none token.\n");
		return;
	}
}