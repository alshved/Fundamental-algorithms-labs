#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "students.h"

bool CorrectID(char* str) {
	char* token = strtok(str, " ");
	if (token == NULL) {
		return false;
	}
	for (int i = 0; token[i] != '\0'; i++) {
		if (!isdigit(token[i])) {
			return false;
		}
	}
	token = strtok(NULL, " ");
	if (token != NULL) {
		return false;
	}
	return true;
}

bool CorrertStrForName(char* str) {
	char* token = strtok(str, " ");
	if (token == NULL) {
		return false;
	}
	token = strtok(NULL, " ");
	if (token != NULL) {
		return false;
	}
	return true;
}

void ClearStdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	ungetc(c, stdin);
}

void InteractiveMenu(Student* students, int count, FILE* out) {
	int choice;
	char input[10];
	printf("\nMenu:\n");
	printf("1. Find student by ID\n");
	printf("2. Find students by surname\n");
	printf("3. Find students by name\n");
	printf("4. Find students by group\n");
	printf("5. Sort students by ID\n");
	printf("6. Sort students by surname\n");
	printf("7. Sort students by name\n");
	printf("8. Sort students by group\n");
	printf("9. Log students with above average grades\n");
	printf("0. Exit\n");

	do {
		printf("Choose an option: ");
		char t;
		if ((t = getchar()) == EOF) {
			printf("Exiting...\n");
			break;
		}
		ungetc(t, stdin);

		scanf("%9s", input);

		if (strlen(input) != 1 || sscanf(input, "%d", &choice) != 1) {
			ClearStdin();
			printf("Invalid input, please try again.\n");
			continue;
		}
		if ((t = getchar()) != '\n') {
			printf("Invalid input, please try again.\n");
			ClearStdin();
			continue;
		}
		ungetc(t, stdin);

		switch (choice) {
			case 1: {
				unsigned int id;
				printf("Enter student ID: ");
				char* in = ReadStringForSpace(stdin);

				if (!CorrectID(in)) {
					printf("Invalid input, please try again.\n");
					free(in);
					continue;
				}
				id = atoi(in);
				Student* student = FindStudentById(students, count, id);
				if (student) {
					printf("Student found: %s %s, Group: %s\n", student->name, student->surname, student->group);
					LogStudentData(out, student);
				} else {
					printf("Student with ID %u not found.\n", id);
				}
				free(in);
				break;
			}
			case 2: {
				char* surname;
				printf("Enter student surname: ");
				surname = ReadStringForSpace(stdin);
				if (!CorrertStrForName(surname)) {
					printf("Invalid input, please try again.\n");
					free(surname);
					continue;
				}
				Student* student = FindStudentsBySurname(students, count, surname);
				if (student) {
					printf("Student found: %s %s, Group: %s\n", student->name, student->surname, student->group);
				} else {
					printf("Student with surname %s not found.\n", surname);
				}
				free(surname);
				break;
			}
			case 3: {
				char* name;
				printf("Enter student name: ");
				name = ReadStringForSpace(stdin);
				if (!CorrertStrForName(name)) {
					printf("Invalid input, please try again.\n");
					free(name);
					continue;
				}
				Student* student = FindStudentsByName(students, count, name);
				if (student) {
					printf("Student found: %s %s, Group: %s\n", student->name, student->surname, student->group);
				} else {
					printf("Student with name %s not found.\n", name);
				}
				free(name);
				break;
			}
			case 4: {
				char* group;
				printf("Enter student group: ");
				group = ReadStringForSpace(stdin);
				if (!CorrertStrForName(group)) {
					printf("Invalid input, please try again.\n");
					free(group);
					continue;
				}
				Student* student = FindStudentsByGroup(students, count, group);
				if (student) {
					printf("Student found: %s %s, Group: %s\n", student->name, student->surname, student->group);
				} else {
					printf("Student with group %s not found.\n", group);
				}
				free(group);
				break;
			}
			case 5: {
				qsort(students, count, sizeof(Student), CompareById);
				printf("Students sorted by ID.\n");
				break;
			}
			case 6: {
				qsort(students, count, sizeof(Student), CompareBySurname);
				printf("Students sorted by surname.\n");
				break;
			}
			case 7: {
				qsort(students, count, sizeof(Student), CompareByName);
				printf("Students sorted by name.\n");
				break;
			}
			case 8: {
				qsort(students, count, sizeof(Student), CompareByGroup);
				printf("Students sorted by group.\n");
				break;
			}
			case 9: {
				LogStudentsWithHighAverage(out, students, count);
				printf("Students with above average grades logged.\n");
				break;
			}
			case 0:
				printf("Exiting...\n");
				break;

			default:
				printf("Invalid option, please try again.\n");
				break;
		}
	} while (choice != 0);
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Wrong arguments.\n");
		return 1;
	}
	Student* students;
	ssize_t count = ReadStudents(argv[1], &students);
	if (count == -1) {
		printf("Error reading from file.\n");
		return 2;
	}
	FILE* out = fopen(argv[2], "a");
	if (!out) {
		printf("Error opening file.\n");
		FreeStudents(students, count);
		return 3;
	}
	InteractiveMenu(students, count, out);
	FreeStudents(students, count);
	fclose(out);
}