#include <stdio.h>
#include "test.h"

void printMenu();
int getInput();

int main() {
	int running = 1;
	printMenu();
	do {
		int input = getInput();
		switch (input) {
		case 1:
			// tests
			printf("Tests:\n");
			break;
		case 2:
			// time comparison
			printf("Time comparison:\n");
			break;
		case 3:
			printMenu();
			break;
		case 4:
			running = 0;
			break;
		default:
			printf("\033[1;31mWrong input. Try again!\033[1;0m\n");
			break;
		}
	} while (running);
	return 0;
}

void printMenu() {
	printf("Menu:\n");
	printf("1. Tests\n");
	printf("2. Comparison of execution time with functions from <math.h>\n");
	printf("3. Menu\n");
	printf("4. Exit\n");
}
int getInput() {
	int input;
	printf("\033[1;33mInput: \033[1;0m");
	scanf_s("%u", &input);
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	return input;
}