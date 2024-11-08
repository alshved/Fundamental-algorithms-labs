#include "students.h"

#define COUNT_EXAMS 5

int CompareById(const void *a, const void *b) { return ((Student *)a)->id - ((Student *)b)->id; }
int CompareBySurname(const void *a, const void *b) { return strcmp(((Student *)a)->surname, ((Student *)b)->surname); }
int CompareByName(const void *a, const void *b) { return strcmp(((Student *)a)->name, ((Student *)b)->name); }
int CompareByGroup(const void *a, const void *b) { return strcmp(((Student *)a)->group, ((Student *)b)->group); }

Student *FindStudentById(Student *students, int count, unsigned int id) {
	for (int i = 0; i < count; i++) {
		if (students[i].id == id) {
			return &students[i];
		}
	}
	return NULL;
}

void FreeStudents(Student *students, ssize_t count) {
	for (ssize_t i = 0; i < count; i++) {
		free(students[i].name);
		free(students[i].group);
		free(students[i].surname);
		free(students[i].grades);
	}
	free(students);
}

char *ReadString(FILE *file) {
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	ungetc(c, file);
	char *str = NULL;
	int length = 0;
	int capacity = 16;
	str = (char *)malloc(capacity * sizeof(char));
	if (!str) {
		perror("Ошибка выделения памяти для строки");
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != ' ' && ch != '\n') {
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char *)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				perror("Ошибка перераспределения памяти для строки");
				free(str);
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';  // Завершаем строку нулевым символом

	return str;
}

ssize_t ReadStudents(const char *file_path, Student **students) {
	FILE *file = fopen(file_path, "r");
	if (!file) {
		printf("Не удалось открыть файл");
		return -1;
	}
	size_t capacity = 2;
	*students = (Student *)malloc(capacity * sizeof(Student));
	if (!*students) {
		printf("Ошибка выделения памяти для массива студентов");
		fclose(file);
		return -1;
	}

	ssize_t count = 0;

	while (1) {
		if (count >= capacity) {
			capacity *= 2;
			Student *tmp = (Student *)realloc(*students, capacity * sizeof(Student));
			if (!tmp) {
				printf("Ошибка перераспределения памяти для массива студентов");
				free(*students);
				fclose(file);
				return -1;
			}
			*students = tmp;
		}

		Student *current_student = &(*students)[count];
		if (fscanf(file, "%u", &current_student->id) != 1) {
			break;
		}

		current_student->grades = (unsigned char *)malloc(sizeof(unsigned char) * COUNT_EXAMS);
		if (!current_student->grades) {
			perror("Ошибка выделения памяти для оценок");
			fclose(file);
			return -1;
		}
		
		current_student->name = ReadString(file);
        if (!current_student->name) {
			fprintf(stderr, "Ошибка при чтении данных о студенте с ID %u\n", current_student->id);
            free(current_student->grades);
            break;
        }

        current_student->surname = ReadString(file);
        if (!current_student->surname) {
			fprintf(stderr, "Ошибка при чтении данных о студенте с ID %u\n", current_student->id);
            free(current_student->name);
            free(current_student->grades);
            break;
        }

        current_student->group = ReadString(file);
        if (!current_student->group) {
			fprintf(stderr, "Ошибка при чтении данных о студенте с ID %u\n", current_student->id);
            free(current_student->name);
            free(current_student->surname);
            free(current_student->grades);
            break;
        }

		for (int j = 0; j < COUNT_EXAMS; j++) {
			fscanf(file, "%hhu", &current_student->grades[j]);
		}
		count++;
	}
	fclose(file);
	return count;
}

float CalculateAverage(Student *student) {
	float sum = 0;
	for (int i = 0; i < COUNT_EXAMS; i++) {
		sum += student->grades[i];
	}
	return sum / COUNT_EXAMS;
}

void LogStudentData(FILE *logFile, Student *student) {
	if (student) {
		fprintf(logFile, "Студент: %s %s, Группа: %s, Средний балл: %.2f\n", student->name, student->surname,
		        student->group, CalculateAverage(student));
	}
}

float CalculateAverageGrade(Student *students, int count) {
	float total = 0.0;
	for (int i = 0; i < count; i++) {
		total += CalculateAverage(&students[i]);
	}
	return total / count;
}

void LogStudentsWithHighAverage(FILE *out, Student *students, int count) {
	float average = CalculateAverageGrade(students, count);
	for (int i = 0; i < count; i++) {
		if (CalculateAverage(&students[i]) > average) {
			fprintf(out, "%s %s\n", students[i].name, students[i].surname);
		}
	}
}

char *ReadStringForSpace(FILE *file) {
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	ungetc(c, file);
	char *str = NULL;
	int length = 0;
	int capacity = 16;
	str = (char *)malloc(capacity * sizeof(char));
	if (!str) {
		perror("Ошибка выделения памяти для строки");
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != '\n') {
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char *)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				perror("Ошибка перераспределения памяти для строки");
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';  // Завершаем строку нулевым символом

	return str;
}