#pragma once
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "enum.h"
#include "queue.h"
#include "stack.h"
#include "token.h"
#include "../PrefixTree.h"

sort_res sorting_station(Queue *infix, Queue *postfix);
unsigned long long Dcalculate(Queue *postfix_queue);
unsigned long long calc(trieNode* root, int base_assign, const char* str, HashTable* synonym, HashTable* rev_synonym);