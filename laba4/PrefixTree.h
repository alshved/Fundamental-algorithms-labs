#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "Settings/hashTable.h"

#define ALPHABET_SIZE 63

typedef struct trieNode {
	struct trieNode *children[ALPHABET_SIZE];
	unsigned long long is_end_of_word;
} trieNode;

int charToInt(char x);
trieNode *createNode();
int treeInsert(trieNode *root, const char *word, unsigned long long value);
unsigned long long treeSearch(trieNode *root, const char *word);
void freeTrie(trieNode *root);
void printTrie(trieNode *root, char *prefix, int depth);