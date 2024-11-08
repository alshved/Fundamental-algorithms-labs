#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main(int argc, char* argv[]) {
	if (ValidateInput(argc, argv)){
		fprintf(stderr, "Error. Usage: %s <input file> <output file>\n", argv[0]);
	}

	FILE* input = fopen(argv[1], "r");
	if (!input) {
		fprintf(stderr, "Error opening input file.\n");
		return 1;
	}

	FILE* output = fopen(argv[2], "w");
	if (!output) {
		fprintf(stderr, "Error opening output file.\n");
		fclose(input);
		return 1;
	}

	char *line = NULL;
	size_t line_size = 0;
	int count = 1;
	while (getline(&line, &line_size, input) != -1) {
		const char* expr = line;
		if (CheckExpression(expr) == OPT_ERROR_INPUT){
			fprintf(stderr, "Incorrect expression.\n");
			continue;
		}
		Node* root = ParseExpression(&expr);
		if (!root){
			continue;
		}
		fprintf(output,"Tree number %d:\n", count++);
		PrintTree(root, output, 0);
		fprintf(output, "\n");
		FreeTree(root);
	}

	free(line);
	fclose(input);
	fclose(output);
	return 0;
}