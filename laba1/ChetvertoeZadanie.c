#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Opts { OPT_D, OPT_I, OPT_S, OPT_A } Opts;

int len(char* string) {
	int count = 0;
	for (int i = 0; string[i] != '\0'; ++i) {
		count++;
	}
	return count;
}

char* NameOutFile(char* InputFile) {
	int strlen = len(InputFile);
	char* nameOut = malloc((5 + strlen) * sizeof(char));
	if (nameOut == NULL) {
		return NULL;
	}
	nameOut[0] = 'o';
	nameOut[1] = 'u';
	nameOut[2] = 't';
	nameOut[3] = '_';
	for (int i = 4; i != strlen + 4; ++i) {
		nameOut[i] = InputFile[i - 4];
	}
	nameOut[strlen + 4] = '\0';
	return nameOut;
}

int GetName(int argc, char** argv, Opts* option, int* neednames) {
	if (argc == 3) {
		if (argv[1][0] != '-' && argv[1][0] != '/') {
			return 1;
		}
		if (argv[1][2] != '\0') {
			return 2;
		}
		switch (argv[1][1]) {
			case 'd':
				*option = OPT_D;
				break;

			case 'i':
				*option = OPT_I;
				break;

			case 's':
				*option = OPT_S;
				break;

			case 'a':
				*option = OPT_A;
				break;

			default:
				return 3;
				break;
		}
		*neednames = 1;
	} else if (argc == 4) {
		if (argv[1][0] != '-' && argv[1][0] != '/') {
			return 1;
		}
		if (argv[1][1] != 'n') {
			return 4;
		}
		if (argv[1][3] != '\0') {
			return 5;
		}
		switch (argv[1][2]) {
			case 'd':
				*option = OPT_D;
				break;

			case 'i':
				*option = OPT_I;
				break;

			case 's':
				*option = OPT_S;
				break;

			case 'a':
				*option = OPT_A;
				break;

			default:
				return 6;
				break;
		}
		*neednames = 2;
	} else {
		return 7;
	}
	return 0;
}

void HandlerOptD(FILE* In, FILE* Out) {
	char el;
	while ((el = getc(In)) != EOF) {
		if (el < '0' || el > '9') {
			fprintf(Out, "%c", el);
		}
	}
}
void HandlerOptI(FILE* In, FILE* Out) {
	char el;
	int count = 0;
	while ((el = getc(In)) != EOF) {
		if (el == '\n') {
			fprintf(Out, "%d\n", count);
			count = 0;
		}
		if ((el >= 'A' && el <= 'Z') || (el >= 'a' && el <= 'z')) {
			count++;
		}
	}
	fprintf(Out, "%d\n", count);
}

void HandlerOptS(FILE* In, FILE* Out) {
	char el = 0;
	int count = 0;
	while ((el = getc(In)) != EOF) {
		if (el == '\n') {
			fprintf(Out, "%d\n", count);
			count = 0;
		} else {
			if (!((el >= 'A' && el <= 'Z') || (el >= 'a' && el <= 'z')) && !(el >= '0' && el <= '9') && !(el == ' ') &&
			    !(el == '\r')) {
				++count;
			}
		}
	}
	fprintf(Out, "%d\n", count);
}

void HandlerOptA(FILE* In, FILE* Out) {
	char el = 0;
	while ((el = getc(In)) != EOF) {
		if (!(el >= '0' && el <= '9') && !(el == '\r') && !(el == '\n')) {
			int n = el;
			char hexnumber[16];
			int i = 0;
			while (n != 0) {
				if (n % 16 < 10) {
					hexnumber[i++] = (char)(n % 16 + '0');
				} else {
					hexnumber[i++] = (char)(n % 16 + 'A' - 10);
				}
				n = n / 16;
			}
			for (int j = i - 1; j != -1; --j) {
				fprintf(Out, "%c", hexnumber[j]);
			}
		} else {
			fprintf(Out, "%c", el);
		}
	}
}

int main(int argc, char** argv) {
	char* nameIn;
	char* nameOut;
	int neednames = 0;
	Opts opt = 0;
	void (*handlers[4])(FILE* In, FILE* out) = {HandlerOptD, HandlerOptI, HandlerOptS, HandlerOptA};
	if (GetName(argc, argv, &opt, &neednames) != 0) {
		printf("Incorrect option.\n");
		return 1;
	}
	nameIn = argv[2];
	if (neednames == 2) {
		nameOut = argv[3];
		if (strcmp(nameIn, nameOut) == 0) {
			printf("The input file is the same as the output file.\n");
			return 1;
		}
	} else {
		nameOut = NameOutFile(nameIn);
		if (nameOut == NULL) {
			printf("Memory allocation error.\n");
			free(nameOut);
			return 2;
		}
	}
	FILE* In;
	FILE* Out;
	if ((In = fopen(nameIn, "r")) == NULL) {
		printf("The fisrt file could not be opened.\n");
		if (neednames == 1) {
			free(nameOut);
		}
		return 3;
	}
	if ((Out = fopen(nameOut, "w")) == NULL) {
		printf("The second file could not be opened.\n");
		fclose(In);
		return 4;
	}
	if (neednames == 1) {
		free(nameOut);
	}
	handlers[opt](In, Out);
	fclose(In);
	fclose(Out);
	return 0;
}