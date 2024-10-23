#include <stdbool.h>
#include <stdio.h>

typedef enum Opts { OPT_H, OPT_P, OPT_S, OPT_E, OPT_A, OPT_F } Opts;

int stoi(char *str, int *number) {
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] < 48 || str[i] > 57) {
			return 1;
		}
	}
	int pred;
	for (int i = 0; str[i] != '\0'; ++i) {
		pred = *number;
		*number = *number * 10 + (str[i] - '0');
		if (*number < pred){
			return 2;
		}
	}
	return 0;
}  // Перевод строки в инт

int get_opts(int argc, char **argv, Opts *option, int *number) {
	if (argc != 3) {
		return 1;
	}
	int stoi_res = stoi(argv[1], number);
	if (stoi_res == 1){
		return 100;
	}
	if (stoi_res == 2){
		return 3;
	}
	if (stoi_res != 0){
		return 2;
	}
	if (argv[2][0] != '-' && argv[2][0] != '/') {
		return 4;
	}
	if (argv[2][2] != '\0') {
		return 5;
	}
	switch (argv[2][1]) {
		case 'h':
			*option = OPT_H;
			break;

		case 'p':
			*option = OPT_P;
			break;

		case 's':
			*option = OPT_S;
			break;
		case 'e':
			*option = OPT_E;
			if (*number > 10) {
				return 6;
			}
			break;
		case 'a':
			*option = OPT_A;
			break;
		case 'f':
			*option = OPT_F;
			break;

		default:
			return 7;
			break;
	}
	return 0;
}

void handler_opt_h(int number) {
	int multiple = number * 2;
	bool flag = false;
	while (multiple <= 100) {
		if (multiple % number == 0) {
			printf("%d ", multiple);
			flag = true;
		}
		multiple += number;
	}
	if (!flag) {
		printf("%s ", "There are no such numbers");
	}
}

void handler_opt_p(int number) {
	if (number == 1) {
		printf("No prime, no composite.\n");
		return;
	}
	bool is_prime = true;
	if (number < 2) {
		is_prime = false;
	}
	for (int i = 2; (i * i) <= number; ++i) {
		if (number % i == 0) {
			is_prime = false;
		}
	}
	if (is_prime) {
		printf("%s ", "A prime number.\n");
	} else {
		printf("%s ", "Composite number.\n");
	}
}

char hex_digit(int code) {
	return (code < 10) ? '0' + code : 'A' + code - 10;
}  // Перевод инта в чар в 16 ричной системе

void handler_opt_s(int number) {
	char ar[12];
	for (int i = 0; i != 9; ++i) {
		ar[i] = '\0';
	}
	int i = 0;
	while (number != 0) {
		ar[i] = hex_digit(number % 16);
		number /= 16;
		++i;
	}
	for (int i = 9; i != -1; --i) {
		if (ar[i] != '\0') {
			printf("%c ", ar[i]);
		}
	}
}

void handler_opt_e(int number) {
	for (int i = 1; i != number + 1; ++i) {
		long long pow = i;
		for (int j = 0; j != 10; ++j) {
			printf("%11lld ", pow);
			pow *= i;
		}
		printf("%c", '\n');
	}
}

void handler_opt_a(int number) {
	long long answer = number * (number + 1) / 2;
	if (answer < 0 || answer < number){
		printf("Overflow long long.\n");
		return;
	}
	printf("%lld ", answer);
}

unsigned long long factorial(int n) {
	unsigned long long res = 1;
	unsigned long long pred;
	for (int i = 2; i <= n; i++) {
		pred = res;
		res *= i;
		if (res <= pred){
			return 0;
		}
	}
	return res;
}

void handler_opt_f(int number) {
	unsigned long long ans = factorial(number);
	if (ans == 0){
		printf("Overflow unsigned long long.\n");
		return;
	}
	printf("%llu ", ans);
}

int main(int argc, char **argv) {
	Opts opt = 0;
	int procceed_number = 0;
	void (*handlers[6])(int) = {handler_opt_h, handler_opt_p, handler_opt_s,
	                            handler_opt_e, handler_opt_a, handler_opt_f};
	int res = get_opts(argc, argv, &opt, &procceed_number);
	if (res == 3){
		printf("Overflow int.\n");
		return 1;
	}
	if (res != 0) {
		printf("%s", "Incorrect option.\n");
		return 2;
	}
	if (procceed_number == 0) {
		printf("You entered 0.\n");
		return 3;
	}
	handlers[opt](procceed_number);
	return 0;
}