#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../PrefixTree.h"
#include "../Settings/help.h"
#include "enum.h"
#include "queue.h"
#include "stack.h"
#include "token.h"

sort_res sorting_station(Queue* infix, Queue* postfix) {
	Stack s;
	stInitialize(&s);
	while (!isEmpty(infix)) {
		token t = queTop(infix);
		quePop(infix);
		if (t.type == TOK_CONST) {
			quePush(postfix, t);
		} else if (t.type == TOK_OP) {
			if (stIsEmpty(&s)) {  // Add check for empty stack
				stPush(&s, t);    // Handle unary operators
			} else {
				token op2 = stTop(&s);
				while (!stIsEmpty(&s) &&
				       (((get_priority(op2.op) > get_priority(t.op)) ||
				         (get_priority(op2.op) == get_priority(t.op) && get_association(t.op) == 'l')) &&
				        stTop(&s).type == TOK_OP)) {
					quePush(postfix, stPop(&s));
					if (!stIsEmpty(&s)) op2 = stTop(&s);  // Update op2 after pop
				}
				stPush(&s, t);
			}
		} else if (t.type == TOK_LBR) {
			stPush(&s, t);
		} else if (t.type == TOK_RBR) {
			if (stIsEmpty(&s)) return SORT_BRACKET_ERROR;  // Check for matching bracket
			token cur = stPop(&s);
			while (cur.type != TOK_LBR) {
				quePush(postfix, cur);
				if (stIsEmpty(&s)) return SORT_BRACKET_ERROR;  // Check for matching bracket
				cur = stPop(&s);
			}
		}
	}
	while (!stIsEmpty(&s)) {
		if (stTop(&s).type == TOK_LBR || stTop(&s).type == TOK_RBR) {
			return SORT_BRACKET_ERROR;
		} else {
			quePush(postfix, stPop(&s));
		}
	}
	return SORT_SUCCESS;
}

unsigned long long Dcalculate(Queue* postfix_queue) {
	Stack operandStack;
	stInitialize(&operandStack);
	while (!isEmpty(postfix_queue)) {
		token current = queTop(postfix_queue);
		quePop(postfix_queue);
		if (current.type == TOK_CONST) {
			stPush(&operandStack, current);
		} else if (current.type == TOK_OP) {
			if (stIsEmpty(&operandStack)) {
				fprintf(stderr, "Error: Insufficient operands for operator %c\n", current.op);
				return ULLONG_MAX;
			}
			unsigned long long operand2 = stPop(&operandStack).const_var;
			if (stIsEmpty(&operandStack)) {
				if (current.op == '~') {
					stPush(&operandStack, (token){.type = TOK_CONST, .const_var = (unsigned long long)(~((unsigned int)(operand2)))});
				} else {
					fprintf(stderr, "Error: Insufficient operands for operator %c\n", current.op);
					return ULLONG_MAX;
				}

			} else {
				unsigned long long operand1 = stPop(&operandStack).const_var;
				unsigned long long result;
				switch (current.op) {
					case '%':
						if (operand2 == 0) {
							fprintf(stderr, "Error: Division by zero!\n");
							return ULLONG_MAX;
						}
						result = operand1 % operand2;
						break;
					case '$':
						result = operand1 ^ operand2;
						break;
					case '&':
						result = operand1 & operand2;
						break;
					case '|':
						result = operand1 | operand2;
						break;
					case '+':
						result = (operand1 + operand2) % ((unsigned long long)(UINT_MAX) + 1);
						break;
					case '-':
						result = operand1 - operand2;
						break;
					case '*':
						result = (operand1 * operand2) % ((unsigned long long)(UINT_MAX) + 1);
						break;
					case '/':
						if (operand2 == 0) {
							fprintf(stderr, "Error: Division by zero!\n");
							return ULLONG_MAX;
						}
						result = operand1 / operand2;
						break;
					case '^':
						result = fastModExp(operand1, operand2, 4294967296);
						break;
					case '~':
						stPush(&operandStack, (token){.type = TOK_CONST, .const_var = operand1});
						stPush(&operandStack, (token){.type = TOK_CONST, .const_var = (unsigned long long)(~((unsigned int)(operand2)))});
						break;
					default:
						fprintf(stderr, "Error: Unknown operator %c\n", current.op);
						return ULLONG_MAX;
				}
				stPush(&operandStack, (token){.type = TOK_CONST, .const_var = result});
			}
		}
	}

	if (stIsEmpty(&operandStack) || stTop(&operandStack).type != TOK_CONST) {
		fprintf(stderr, "Error: Invalid postfix expression.\n");
		return ULLONG_MAX;
	}

	double finalResult = stPop(&operandStack).const_var;
	return finalResult;
}

unsigned long long calc(trieNode* root, int base_assign, char* str, HashTable* synonym, HashTable* rev_synonym) {
	Queue infix_expression;
	queInitialize(&infix_expression);

	read_exp_result read_status = read_expression(&infix_expression, str, base_assign, root, synonym, rev_synonym);
	if (read_status != READ_EXP_SUCCESS) {
		queDestroy(&infix_expression);
		return ULLONG_MAX;
	}

	Queue postfix_expression;
	queInitialize(&postfix_expression);
	sort_res sort_status = sorting_station(&infix_expression, &postfix_expression);
	if (sort_status != SORT_SUCCESS) {
		return ULLONG_MAX;
	}
	unsigned long long res = Dcalculate(&postfix_expression);

	queDestroy(&infix_expression);
	queDestroy(&postfix_expression);

	return res;
}
