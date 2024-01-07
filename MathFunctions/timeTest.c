#include <stdio.h>
#include <time.h>
#include <math.h>
#include "math.h"
#include "timeTest.h"




void timeTestAll() {
	IntForInfo intFor = { -10000000, 10000000, 1 };
	DoubleForInfo doubleFor = { -10000, 10000, 0.001 };

	compareTime_IntInt(myAbs, abs, "myAbs", "abs", &intFor);
	compareTime_DoubleDouble(myFabs, fabs, "myFabs", "fabs", &doubleFor);
}


unsigned long testTime_DoubleDouble(double (*function)(double), DoubleForInfo* cycle) {
	clock_t before = clock();
	for (double i = cycle->start; i <= cycle->end; i += cycle->step) {
		function(i);
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}
unsigned long testTime_IntInt(int (*function)(int), IntForInfo* cycle) {
	clock_t before = clock();
	for (int i = cycle->start; i <= cycle->end; i += cycle->step) {
		function(i);
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}
unsigned long testTime_DoubleDoubleDouble(double (*function)(double, double), DoubleForInfo* cycle1, DoubleForInfo* cycle2) {
	clock_t before = clock();
	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		for (double j = cycle2->start; j <= cycle2->end; j += cycle2->step) {
			function(i, j);
		}
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}
unsigned long testTime_DoubleDoubleDoubleref(double (*function)(double, double*), DoubleForInfo* cycle1) {
	double out;
	clock_t before = clock();
	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		function(i, &out);
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}
unsigned long testTime_DoubleDoubleIntref(double (*function)(double, int*), DoubleForInfo* cycle1) {
	int out;
	clock_t before = clock();
	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		function(i, &out);
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}
unsigned long testTime_DoubleDoubleInt(double (*function)(double, int), DoubleForInfo* cycle1, IntForInfo* cycle2) {
	int out;
	clock_t before = clock();
	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		for (int j = cycle2->start; j <= cycle2->end; j += cycle2->step) {
			function(i, j);
		}
	}
	return (clock() - before) * 1000 / CLOCKS_PER_SEC;
}

void compareTime_DoubleDouble(double (*myFunction)(double), double (*mathFunction)(double),
	char* name1, char* name2, DoubleForInfo* cycle) {

	unsigned long time1 = testTime_DoubleDouble(myFunction, cycle);
	unsigned long time2 = testTime_DoubleDouble(mathFunction, cycle);
	printResult(time1, time2, name1, name2);
}
void compareTime_IntInt(int (*myFunction)(int), int (*mathFunction)(int), 
	char* name1, char* name2, IntForInfo* cycle) {

	unsigned long time1 = testTime_IntInt(myFunction, cycle);
	unsigned long time2 = testTime_IntInt(mathFunction, cycle);
	printResult(time1, time2, name1, name2);
}
void compareTime_DoubleDoubleDouble(double (*myFunction)(double, double), double (*mathFunction)(double, double),
	char* name1, char* name2,
	DoubleForInfo* cycle1, DoubleForInfo* cycle2) {

	unsigned long time1 = testTime_DoubleDoubleDouble(myFunction, cycle1, cycle2);
	unsigned long time2 = testTime_DoubleDoubleDouble(mathFunction, cycle1, cycle2);
	printResult(time1, time2, name1, name2);
}

void compareTime_DoubleDoubleDoubleref(double (*myFunction)(double, double*), double (*mathFunction)(double, double*),
	char* name1, char* name2, DoubleForInfo* cycle) {

	unsigned long time1 = testTime_DoubleDoubleDoubleref(myFunction, cycle);
	unsigned long time2 = testTime_DoubleDoubleDoubleref(mathFunction, cycle);
	printResult(time1, time2, name1, name2);
}

void compareTime_DoubleDoubleIntref(double (*myFunction)(double, int*), double (*mathFunction)(double, int*),
	char* name1, char* name2, DoubleForInfo* cycle) {

	unsigned long time1 = testTime_DoubleDoubleIntref(myFunction, cycle);
	unsigned long time2 = testTime_DoubleDoubleIntref(mathFunction, cycle);
	printResult(time1, time2, name1, name2);
}

void compareTime_DoubleDoubleInt(double (*myFunction)(double, int), double (*mathFunction)(double, int),
	char* name1, char* name2, 
	DoubleForInfo* cycle1, IntForInfo* cycle2) {

	unsigned long time1 = testTime_DoubleDoubleInt(myFunction, cycle1, cycle2);
	unsigned long time2 = testTime_DoubleDoubleInt(mathFunction, cycle1, cycle2);
	printResult(time1, time2, name1, name2);
}



void printResult(unsigned long time1, unsigned long time2, char* name1, char* name2) {
	float d;
	if (time1 < time2) {
		d = (float)time2 / time1;
		printf("\033[1;32m%s is %.2f times faster than %s\033[1;0m\n", name1, d, name2);
		printf("\033[1;32m\t%s : %lu ms\033[1;0m\n", name1, time1);
		printf("\033[1;32m\t%s : %lu ms\033[1;0m\n", name2, time2);
	}
	else if (time1 > time2) {
		d = (float)time1 / time2;
		printf("\033[1;31m%s is %.2f times slower than %s\033[1;0m\n", name1, d, name2);
		printf("\033[1;31m\t%s : %lu ms\033[1;0m\n", name1, time1);
		printf("\033[1;31m\t%s : %lu ms\033[1;0m\n", name2, time2);
	}
	else {
		printf("\033[1;33m%s\'s time equals to %s\'s time\033[1;0m\n", name1, name2);
		printf("\033[1;33m\t%s : %lu ms\033[1;0m\n", name1, time1);
		printf("\033[1;33m\t%s : %lu ms\033[1;0m\n", name2, time2);
	}
}