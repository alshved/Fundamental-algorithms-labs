#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Opts { ASCENDING, DESCENDING } Opts;

typedef struct Employee {
	unsigned int id;
	char* name;
	char* surname;
	double salary;
} Employee;

int ParseInput(int argc, char** argv, Opts* opt, char** file_name1, char** file_name2) {
	if (argc != 4) {
		return 1;
	}
	if ((argv[2][0] == '/' || argv[2][0] == '-') && argv[2][2] == '\0') {
		switch (argv[2][1]) {
			case 'a':
				*opt = ASCENDING;
				break;
			case 'd':
				*opt = DESCENDING;
				break;
			default:
				return 3;
		}
	} else {
		return 3;
	}
	*file_name1 = argv[1];
	*file_name2 = argv[3];
	return 0;
}

void ClearEmployees(Employee* employees, int size) {
	for (int i = 0; i < size; i++) {
		free(employees[i].surname);
		free(employees[i].name);
	}
	free(employees);
}

bool CheckName(char* name) {
	int n = strlen(name);
	if (n == 0) {
		return false;
	}
	for (int i = 0; i != n; ++i) {
		if (!isalpha(name[i])) {
			return false;
		}
	}
	return true;
}

int GetInfo(Employee** employees, FILE* file, int* size) {
	int buffer_size = 10;
	*employees = (Employee*)malloc(sizeof(Employee) * buffer_size);
	if (*employees == NULL) {
		return 1;
	}

	int count = 0;
	Employee cur;

	while (true) {
		cur.surname = (char*)malloc(1024 * sizeof(char));
		cur.name = (char*)malloc(1024 * sizeof(char));
		if (fscanf(file, "%u %s %s %lf", &(cur.id), cur.name, cur.surname, &(cur.salary)) != 4) {
			free(cur.surname);
			free(cur.name);
			break;
		}

		if (!CheckName(cur.surname) || !CheckName(cur.name)) {
			free(cur.surname);
			free(cur.name);
			ClearEmployees(*employees, count);
			return 2;
		}

		if (count >= buffer_size) {
			buffer_size *= 2;
			Employee* temp = realloc(*employees, sizeof(Employee) * buffer_size);
			if (temp == NULL) {
				free(cur.surname);
				free(cur.name);
				ClearEmployees(*employees, count);
				return 3;
			}
			*employees = temp;
		}
		(*employees)[count] = cur;
		count++;
	}
	*size = count;
	return 0;
}

int comparator(const void* a, const void* b) {
	Employee* e1 = (Employee*)a;
	Employee* e2 = (Employee*)b;
	if (e1->salary != e2->salary) {
		return e1->salary < e2->salary ? -1 : 1;
	}
	if (strcmp(e1->surname, e2->surname) != 0) {
		return strcmp(e1->surname, e2->surname);
	}
	if (strcmp(e1->name, e2->name) != 0) {
		return strcmp(e1->name, e2->name);
	}
	if (e1->id != e2->id) {
		return e1->id < e2->id ? -1 : 1;
	}
	return 0;
}

void ReverseArray(Employee* arr, int n) {
	for (int i = 0; i != n / 2; ++i) {
		Employee temp = arr[i];
		arr[i] = arr[n - i - 1];
		arr[n - i - 1] = temp;
	}
}

int main(int argc, char** argv) {
	int capacity = 1;
	Opts opt;
	char* inp_file;
	char* out_file;
	int status = ParseInput(argc, argv, &opt, &inp_file, &out_file);
	if (status != 0) {
		printf("Incorect input.\n");
		return 1;
	}

	Employee* employees;
	int size;
	FILE* file_in = fopen(inp_file, "r");
	if (file_in == NULL) {
		return 1;
	}
	status = GetInfo(&employees, file_in, &size);
	fclose(file_in);
	if (status != 0) {
        printf("Wrong data.\n");
		return 1;
	}
	FILE* file_out = fopen(out_file, "w");
	if (file_out == NULL) {
		ClearEmployees(employees, size);
        printf("Error opening file.\n");
		return 1;
	}

	qsort(employees, size, sizeof(Employee), comparator);

	if (opt == DESCENDING) {
		ReverseArray(employees, size);
	}

	for (int i = 0; i < size; i++) {
		fprintf(file_out, "%u %s %s %lf\n", employees[i].id, employees[i].name, employees[i].surname,
		        employees[i].salary);
	}
	fclose(file_out);
	ClearEmployees(employees, size);
}