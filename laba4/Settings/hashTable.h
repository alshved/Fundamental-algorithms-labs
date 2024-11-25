#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100
#define PRIME 97

typedef struct Node {
	char *key;
	char *value;
	struct Node *next;
} Node;

typedef struct HashTable {
	Node **table;
} HashTable;

unsigned int hash(char *str);
HashTable *createHashTable();
int insert(HashTable *table, char *key, char *value);
char *search(HashTable *table, char *key);
void destroyHashTable(HashTable *table);