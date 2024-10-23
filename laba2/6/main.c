#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"

int main() {
	double t;
	int romanNum1;
	unsigned int zeckNum;
	int romanNum2;
	unsigned int zeckNum2;
	int baseNum;
	char *str = "  112312.00000   IV  thisa 101011 XVII  001011  1a";
	char sstr[6];
	if (overSscanf(str, "%lf %Ro %5s %Zr %Ro %Zr %Cv", &t, &romanNum1, sstr, &zeckNum, &romanNum2, &zeckNum2, &baseNum, 16) == -1){
		printf("Ошибка.\n");
		return 1;
	}
    printf("%lf\n", t);
	printf("%d\n", romanNum1);
	printf("%s\n", sstr);
	printf("%u\n", zeckNum);
	printf("%u\n", zeckNum2);
	printf("%d\n", romanNum2);
	printf("%d\n", baseNum);

	FILE *file = fopen("input.txt", "r");
	if (!file) {
		printf("Не удалось открыть файл.\n");
		return 1;
	}
    int p;

	char ssstr[6];
	if (overFscanf(file, "%Ro %5s %Zr %Zr %Ro %Cv %d", &romanNum1, ssstr, &zeckNum, &zeckNum2, &romanNum2, &baseNum, 16, &p) == -1){
		printf("Ошибка.\n");
		return 1;
	}
	printf("%s\n", ssstr);
	printf("%d\n", romanNum1);
	printf("%u\n", zeckNum);
	printf("%u\n", zeckNum2);
	printf("%d\n", romanNum2);
	printf("%d\n", baseNum);
    printf("%d\n", p);

	fclose(file);
	return 0;
}
