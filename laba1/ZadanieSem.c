#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum Opts { OPT_R, OPT_A } Opts;

int len(char* string) {
	int count = 0;
	for (int i = 0; string[i] != '\0'; ++i) {
		count++;
	}
	return count;
}

int GetName(int argc, char** argv, Opts* option) {
	if (argc == 4) {
		if (argv[1][0] != '-' && argv[1][0] != '/') {
			return 1;
		}
		if (argv[1][1] != 'a' || argv[1][2] != '\0') {
			return 2;
		}
		*option = OPT_A;
	} else if (argc == 5) {
		if (argv[1][0] != '-' && argv[1][0] != '/') {
			return 1;
		}
		if (argv[1][1] != 'r' || argv[1][2] != '\0') {
			return 3;
		}
		*option = OPT_R;
	} else {
		return 4;
	}
	return 0;
}

char* ToQuat(int n) {
	char* arr = malloc(sizeof(char) * 6);
	if (arr == NULL) {
		printf("Memmory error\n");
		return NULL;
	}
	int x = n;
	int i = 0;
	while (x != 0) {
		arr[i] = '0' + x % 4;
		x /= 4;
		++i;
	}
	arr[i] = '\0';
	return arr;
}

char* ToOct(int n) {
	char* arr = malloc(sizeof(char) * 8);
	if (arr == NULL) {
		printf("Memmory error\n");
		return NULL;
	}
	int x = n;
	int i = 0;
	while (x != 0) {
		arr[i] = '0' + x % 8;
		x /= 8;
		++i;
	}
	arr[i] = '\0';
	return arr;
}

void HandlerOptA(FILE* In, FILE* Out) {
	int newlen;
	if (fseek(In, 0L, SEEK_END) != 0) {
		printf("Error.\n");
		return;
	}
	long long bufsize = ftell(In);
	if (bufsize == -1) {
		printf("Error.\n");
		return;
	}
	if (fseek(In, 0L, SEEK_SET) != 0) {
		printf("Error.\n");
		return;
	}

	char* source = malloc(sizeof(char) * (bufsize + 1));
	if (source == NULL) {
		printf("Memmory error\n");
		return;
	}
	newlen = fread(source, sizeof(char), bufsize, In);
	char* buff = malloc(sizeof(char) * (3 * newlen));
	if (buff == NULL) {
		printf("Memmory error\n");
		free(source);
		return;
	}

	source[bufsize] = '\0';
	if (ferror(In) != 0) {
		printf("Error reading file\n");
		free(source);
		free(buff);
		return;
	}

	int it = 0;
	int k = 1;  // Нужно для количества реальных символов
	for (int i = 0; source[i] != '\0'; ++i) {
		if (source[i] == ' ' || source[i] == '\t' || source[i] == '\n' || source[i] == '\r' || source[i] == '\0') {
			continue;
		}
		if (k % 10 == 0) {
			if (source[i] >= 'A' && source[i] <= 'Z') {
				source[i] += 32;
			}
			char* el = ToQuat((int)source[i]);
			if (el == NULL) {
				printf("Memmory error\n");
				free(source);
				free(buff);
				return;
			}
			for (int j = len(el) - 1; j != -1; --j) {
				buff[it] = el[j];
				++it;
			}
			free(el);
		} else if (k % 2 == 0) {
			if (source[i] >= 'A' && source[i] <= 'Z') {
				source[i] += 32;
			}
			buff[it] = source[i];
			it++;
		} else if (k % 5 == 0) {
			char* el = ToOct((int)source[i]);
			if (el == NULL) {
				printf("Memmory error\n");
				free(source);
				free(buff);
				return;
			}
			for (int j = len(el) - 1; j != -1; --j) {
				buff[it] = el[j];
				++it;
			}
			free(el);
		} else {
			buff[it++] = source[i];
		}
		++k;
	}
	buff[it] = '\0';
	for (int i = 0; buff[i] != '\0'; ++i) {
		fputc(buff[i], Out);
		if (buff[i + 1] != '\0') {
			fputc(' ', Out);
		}
	}
	free(source);
	free(buff);
}

char* MergeArrays(char* arr1, char* arr2) {
	size_t len1 = len(arr1);
	size_t len2 = len(arr2);
	size_t max_len = len1 + len2;
	char* result = (char*)malloc(max_len + 1);
	if (result == NULL) {
		printf("Memmory error.\n");
		return NULL;
	}
	size_t i = 0, j = 0, k = 0;
	while (i < len1 && j < len2) {
		while ((arr1[i] == ' ' || arr1[i] == '\t' || arr1[i] == '\n' || arr1[i] == '\r')) {
			++i;
		}
		while ((arr2[j] == ' ' || arr2[j] == '\t' || arr2[j] == '\n' || arr2[j] == '\r')) {
			++j;
		}
		if (k % 2 == 0) {
			result[k++] = arr1[i++];
		} else {
			result[k++] = arr2[j++];
		}
	}
	while (i < len1) {
		while ((arr1[i] == ' ' || arr1[i] == '\t' || arr1[i] == '\n' || arr1[i] == '\r')) {
			++i;
		}
		result[k++] = arr1[i++];
	}
	while (j < len2) {
		while ((arr2[j] == ' ' || arr2[j] == '\t' || arr2[j] == '\n' || arr2[j] == '\r')) {
			++j;
		}
		result[k++] = arr2[j++];
	}
	result[k] = '\0';
	return result;
}

void HandlerOptR(FILE* firstIn, FILE* secIn, FILE* Out) {
	char* source1 = NULL;
	char* source2 = NULL;
	int newlen1;
	int newlen2;

	if (fseek(firstIn, 0L, SEEK_END) == 0) {
		long long bufsize = ftell(firstIn);
		if (bufsize == -1) {
			printf("Error.\n");
			return;
		}
		source1 = malloc(sizeof(char) * (bufsize + 1));
		if (source1 == NULL) {
			printf("Memmory error\n");
			return;
		}
		if (fseek(firstIn, 0L, SEEK_SET) != 0) {
			printf("Error.\n");
			free(source1);
			return;
		}
		newlen1 = fread(source1, sizeof(char), bufsize, firstIn);
		if (ferror(firstIn) != 0) {
			printf("Error reading file.\n");
			free(source1);
			return;
		}
		source1[newlen1++] = '\0';
	} else {
		printf("Error reading file.\n");
		return;
	}

	if (fseek(secIn, 0L, SEEK_END) == 0) {
		long long bufsize = ftell(secIn);
		if (bufsize == -1) {
			printf("Error.\n");
			return;
		}
		source2 = malloc(sizeof(char) * (bufsize + 1));
		if (source2 == NULL) {
			printf("Memmory error\n");
			free(source1);
			return;
		}
		if (fseek(secIn, 0L, SEEK_SET) != 0) {
			printf("Error");
			free(source1);
			free(source2);
			return;
		}
		newlen2 = fread(source2, sizeof(char), bufsize, secIn);
		if (ferror(secIn) != 0) {
			printf("Error reading file.\n");
			free(source1);
			free(source2);
			return;
		}
		source2[newlen2++] = '\0';
	} else {
		printf("Error reading file.\n");
		free(source1);
		return;
	}

	char* buff;
	if ((buff = MergeArrays(source1, source2)) == NULL) {
		printf("Error\n");
		free(buff);
		free(source1);
		free(source2);
		return;
	}
	for (int i = 0; buff[i] != '\0'; ++i) {
		fputc(buff[i], Out);
		if (buff[i + 1] != '\0') {
			fputc(' ', Out);
		}
	}
	free(source1);
	free(source2);
	free(buff);
}

int main(int argc, char** argv) {
	Opts opt = 0;
	if (GetName(argc, argv, &opt) != 0) {
		printf("Incorrect option.\n");
		return 1;
	}
	if (opt == OPT_A) {
		char* nameinput = argv[2];
		char* nameoutput = argv[3];
		FILE* In;
		FILE* Out;
		if ((In = fopen(nameinput, "r")) == NULL) {
			printf("The fisrt file could not be opened.\n");
			return 2;
		}
		if ((Out = fopen(nameoutput, "w")) == NULL) {
			printf("The second file could not be opened.\n");
			fclose(In);
			return 3;
		}
		HandlerOptA(In, Out);
		fclose(In);
		fclose(Out);
	} else if(opt == OPT_R) {
		char* nameinfirst = argv[2];
		char* nameinsecond = argv[3];
		char* nameout = argv[4];
		FILE* Firstin;
		FILE* Secin;
		FILE* Out;
		if ((Firstin = fopen(nameinfirst, "r")) == NULL) {
			printf("The fisrt file could not be opened.\n");
			return 4;
		}
		if ((Secin = fopen(nameinsecond, "r")) == NULL) {
			printf("The second file could not be opened.\n");
			fclose(Firstin);
			return 5;
		}
		if ((Out = fopen(nameout, "w")) == NULL) {
			printf("The second file could not be opened.\n");
			fclose(Firstin);
			fclose(Secin);
			return 6;
		}
		HandlerOptR(Firstin, Secin, Out);
		fclose(Firstin);
		fclose(Secin);
		fclose(Out);
	}
	return 0;
}