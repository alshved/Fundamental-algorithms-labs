#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
	double x, y;
} Point;

typedef enum IsConvex { YES, NO, ERROR, ON_STRAIGHT } IsConvex;

double vecProduct(Point p1, Point p2, Point p3) {
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

IsConvex CheckConvex(int count, ...) {
	bool onOneStraightLine = true;
	if (count < 3) {
		return ERROR;
	}

	va_list points;
	va_start(points, count);

	Point *vertices = malloc(count * sizeof(Point));
	if (vertices == NULL) {
		va_end(points);
		return ERROR;
	}

	for (int i = 0; i < count; ++i) {
		vertices[i] = va_arg(points, Point);
	}
	va_end(points);

	int sign = 0;
	for (int i = 0; i < count; ++i) {
		Point p1 = vertices[i];
		Point p2 = vertices[(i + 1) % count];
		Point p3 = vertices[(i + 2) % count];

		double cp = vecProduct(p1, p2, p3);
		if (cp != 0) {
			onOneStraightLine = false;
			int current_sign = (cp > 0) ? 1 : -1;
			if (sign == 0) {
				sign = current_sign;
			} else if (sign != current_sign) {
				free(vertices);
				return NO;
			}
		}
	}
	if (onOneStraightLine) {
		return ON_STRAIGHT;
	}
	free(vertices);
	return YES;
}

void CheckRes(IsConvex res) {
	if (res == YES) {
		printf("Многоугольник выпуклый.\n");
	} else if (res == NO) {
		printf("Многоугольник невыпуклый.\n");
	} else if (res == ON_STRAIGHT) {
		printf("Точки лежат на одной прямой.\n");
	} else {
		printf("Ошибка во входных данных.\n");
	}
}

int main() {
	Point p1 = {0, 1};
	Point p2 = {1, 3};
	Point p3 = {3, 3};
	Point p4 = {6, 2};
	Point p5 = {5, 0};
	Point p6 = {2, 0};
	Point p7 = {0, 0};
	IsConvex res = CheckConvex(7, p1, p2, p3, p4, p5, p6, p7);
	printf("1: ");
	CheckRes(res);

	p1 = (Point){1, 1};
	p2 = (Point){3, 0};
	p3 = (Point){5, 1};
	p4 = (Point){3, 2};
	p5 = (Point){2, 5};
	p6 = (Point){0, 3};
	res = CheckConvex(6, p1, p2, p3, p4, p5, p6);
	printf("2: ");
	CheckRes(res);

	p1 = (Point){0, 0};
	p2 = (Point){0, 2};
	p3 = (Point){2, 2};
	p4 = (Point){2, 0};
	res = CheckConvex(4, p1, p2, p3, p4);
	printf("3: ");
	CheckRes(res);

	p1 = (Point){0, 0};
	p2 = (Point){0, 2};
	p3 = (Point){0, 4};
	res = CheckConvex(3, p1, p2, p3);
	printf("4: ");
	CheckRes(res);

	p1 = (Point){0, 0};
	p2 = (Point){0, 2};
	p3 = (Point){1, 1};
	p4 = (Point){2, 2};
	p5 = (Point) {2, 0};
	res = CheckConvex(5, p1, p2, p3, p4, p4);
	printf("5: ");
	CheckRes(res);

	return 0;
}