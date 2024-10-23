#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "func.h"

int main(int argc, char* argv[]) {
    overfprintf(stdout, "%mi\n%mu\n%md\n%mf", (int)10, (unsigned int)12, (double)2.2, (double)2.3);
    overfprintf(stdout, "\n%lld\n", 123);
    overfprintf(stdout, "\n\n%Ro, %Ro, %Ro, %Cv\n", 5435, 0, -15, 1234, 16);
    overfprintf(stdout, "%1.1f", 123.0);
    overfprintf(stdout, "sfdsdf %to %TO %d\t%d\n", "abcdef", 16, "10", 10, 12, 12);
    overfprintf(stdout, "pobeda\t%s", "POBEDA\n");
}