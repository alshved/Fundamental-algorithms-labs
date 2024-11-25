#include "hashTable.h"

// Функция вычисления хэша
unsigned int hash(char *str) {
    unsigned int hash = 0;
    unsigned int power = 1;
    while (*str) {
        hash = (hash + (*str) * power) % TABLE_SIZE;
        power = (power * PRIME) % TABLE_SIZE;
        str++;
    }
    return hash;
}

// Создание хэш таблицы
HashTable *createHashTable() {
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return NULL;
    }
    table->table = malloc(TABLE_SIZE * sizeof(Node *));
    if (table->table == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->table[i] = NULL;
    }
    return table;
}


// Вставка в хэш таблицу
int insert(HashTable *table, char *key, char *value) {
    unsigned int index = hash(key);
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return 1;
    }
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = table->table[index];
    table->table[index] = newNode;
    return 0;
}


// Поиск по хэш таблице
char *search(HashTable *table, char *key) {
    if (key == NULL){
        return NULL;
    }
    unsigned int index = hash(key);
    Node *current = table->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void destroyHashTable(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table->table[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(table->table);
    free(table);
}