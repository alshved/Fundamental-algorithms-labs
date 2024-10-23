#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ARRAY_SIZE 20

bool parse_int_is_possible(char* str) {
	int i = 0;
	if (str[0] == '-') {
		i++;
	}
	for (; str[i] != '\0'; ++i) {
		if (str[i] < 48 || str[i] > 57) {
			return false;
		}
	}
	return true;
}

int parse_int(char* str) {
	int result = 0.0;
	int sign = 1;
	if (*str == '-') {
		sign = -1;
		str++;
	}
	while (*str) {
		result = result * 10.0 + (*str - '0');
		str++;
	}
	return result;
}

void fill(int* arr, int size, int a, int b) {
    for (int i = 0; i != size; ++i) {
        arr[i] = rand() % (b - a + 1) + a;
    }
}

void swap_max_min(int* ar, int size){
	int index_max = 0;
	int index_min = 0;
	for (int i = 0; i != size; ++i){
		if (ar[i] < ar[index_min]){
			index_min = i;
		}
		if (ar[i] > ar[index_max]){
			index_max = i;
		}
	}
	int tmp = ar[index_max];
	ar[index_max] = ar[index_min];
	ar[index_min] = tmp;
}

int main(int argc, char** argv) {
	if (argc != 3){
		printf("Wrong arguments\n");
		return 1;
	}
	int a, b;
	if (parse_int_is_possible(argv[1])){
		a = parse_int(argv[1]);
	} else {
		printf("Wrong first argument\n");
		return 2;
	}
	if (parse_int_is_possible(argv[2])){
		b = parse_int(argv[2]);
	} else {
		printf("Wrong second argument\n");
		return 3;
	}
	if (a > b){
		printf("Wrong arguments\n");
		return 4;
	}
	int array[ARRAY_SIZE];
	fill(array, ARRAY_SIZE, a, b);
	printf("Before swap:\n");
	for (int i = 0; i != ARRAY_SIZE; ++i){
		printf("%d ", array[i]);
	}
	swap_max_min(array, ARRAY_SIZE);
	printf("\nAfter swap:\n");
	for (int i = 0; i != ARRAY_SIZE; ++i){
		printf("%d ", array[i]);
	}
}