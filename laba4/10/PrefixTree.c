#include "PrefixTree.h"

#define ALPHABET_SIZE 63

int charToInt(char x) {
	if (isdigit(x)) {
		return x - '0';
	} else if (islower(x)) {
		return x - 'a' + 10;
	} else if (isupper(x)) {
		return x - 'A' + 36;
	} else if (x == '_') {
		return 62;
	}
	return -1;
}

trieNode *createNode() {
	trieNode *node = (trieNode *)malloc(sizeof(trieNode));
	if (node == NULL) {
		return NULL;
	}
	node->is_end_of_word = ULLONG_MAX;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		node->children[i] = NULL;
	}
	return node;
}

int treeInsert(trieNode *root, const char *word, unsigned long long value) {
	trieNode *current = root;
	while (*word) {
		int index = charToInt(*word);
		if (index < 0 || index >= ALPHABET_SIZE) {
			return 1;
		}
		if (current->children[index] == NULL) {
			current->children[index] = createNode();
			if (current->children[index] == NULL) {
				return 1;
			}
		}
		current = current->children[index];
		word++;
	}
	current->is_end_of_word = value;
	return 0;
}

unsigned long long treeSearch(trieNode *root, const char *word) {
	trieNode *current = root;
	while (*word) {
		int index = charToInt(*word);
		if (index < 0 || index >= ALPHABET_SIZE) {
			return ULLONG_MAX;
		}
		if (current->children[index] == NULL) {
			return ULLONG_MAX;
		}
		current = current->children[index];
		word++;
	}
	return current->is_end_of_word;
}

void freeTrie(trieNode *root) {
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		if (root->children[i]) {
			freeTrie(root->children[i]);
		}
	}
	free(root);
}

void printTrie(trieNode *root, char *prefix, int depth) {
    if (root == NULL) {
        return;
    }

    // Печатаем текущий узел
    if (root->is_end_of_word != ULLONG_MAX) {
        printf("%s: %llu\n", prefix, root->is_end_of_word);
    }

    // Рекурсивно печатаем потомков
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i]) {
            char new_prefix[100];
            // Исправленный код для символа '_':
            sprintf(new_prefix, "%s%c", prefix, 
                    i < 10 ? i + '0' : 
                    i < 36 ? i - 10 + 'a' :
                    i == 62 ? '_' : i - 36 + 'A');
            printTrie(root->children[i], new_prefix, depth + 1);
        }
    }
}