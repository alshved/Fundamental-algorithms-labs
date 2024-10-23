#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void fill(int* arr, int size, int a, int b) {
	for (int i = 0; i != size; ++i) {
		arr[i] = rand() % (b - a + 1) + a;
	}
}

void swap(int* arr, int i, int j) {
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void bubble_sort(int* arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) swap(arr, j, j + 1);
		}
	}
}

int binary_search(int* data, int left, int right, int key) {
	int best_index = 0;
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (data[mid] == key) {
			return data[mid];
		}
		if (data[mid] < key) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
		if (abs(data[mid] - key) <= abs(data[best_index] - key)) {
			best_index = mid;
		}
	}
	return data[best_index];
}

void combinate_arrays(int* a, int* b, int* c, int size_a, int size_b) {
	for (int i = 0; i != size_a; ++i) {
        c[i] = a[i] + binary_search(b, 0, size_b + 1, a[i]);
	}
}

int main(int argc, char** argv) {
	if (argc != 1) {
		printf("Wrong arguments\n");
		return 1;
	}
	int size_a = rand() % 9991 + 10;
	int size_b = rand() % 9991 + 10;
	int* a = malloc(size_a * sizeof(int));
	if (a == NULL) {
		printf("Memmory error\n");
	}
	int* b = malloc(size_b * sizeof(int));
	if (b == NULL) {
		printf("Memmory error\n");
		free(a);
	}
	fill(a, size_a, -1000, 1000);
	fill(b, size_b, -1000, 1000);
	int* c = malloc((size_a) * sizeof(int));
	if (c == NULL) {
		printf("Memmory error\n");
		free(a);
		free(b);
	}
	bubble_sort(b, size_b);
	combinate_arrays(a, b, c, size_a, size_b);
    for (int i = 0; i != size_a; ++i){
        printf("%d ", c[i]);
    }
	free(a);
	free(b);
	free(c);
}