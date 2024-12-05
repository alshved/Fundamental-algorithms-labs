#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"

typedef enum ResultSettings { ERROR_READING_FILE, WRONG_INPUT_DATA, MEMORY_ALLOC_ERROR, SUCCESS } ResultSettings;

typedef struct {
	char original[7];
	char* synonym;
} CommandSynonym;

typedef enum DirectionOfSaving { DLEFT = 0, DRIGHT = 1 } DirectionOfSaving;

typedef enum DirectionOfExecution { ERIGHT = 0, ELEFT = 1, EMIDDLE = 2 } DirectionOfExecution;

void initHashTable(HashTable* table);
ResultSettings loadSettings(FILE* file, HashTable* table1, HashTable* table, DirectionOfSaving* direction_of_saving,
                            DirectionOfExecution* direction_of_execution);