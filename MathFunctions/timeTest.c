#include <stdio.h>
#include <time.h>
#include <math.h>
#include "math.h"
#include "timeTest.h"
#include <stdint.h>

static void printError(void* arg, ...) {
	printf(stderr, "An error occurred when using overloaded functions");
}

// functions overloading

#define testTime_0(func, outer_cycle) _Generic((func),\
	double (*)(double): _Generic((outer_cycle),\
		DoubleForInfo*: testTime_DoubleDouble,\
		default: printError),\
	int (*)(int): _Generic((outer_cycle),\
		IntForInfo*: testTime_IntInt,\
		default: printError),\
	double (*)(double, double*): _Generic((outer_cycle),\
		DoubleForInfo*: testTime_DoubleDoubleDoubleref,\
		default: printError),\
	double (*)(double, int*): _Generic((outer_cycle),\
		DoubleForInfo*: testTime_DoubleDoubleIntref,\
		default: printError),\
	default: printError)(func, outer_cycle)

#define testTime_1(func, outer_cycle, inner_cycle) _Generic((func),\
	double (*)(double, double): _Generic((outer_cycle),\
		DoubleForInfo*: _Generic((inner_cycle),\
			DoubleForInfo*: testTime_DoubleDoubleDouble,\
			default: printError),\
		default: printError),\
	double (*)(double, int):_Generic((outer_cycle),\
		DoubleForInfo*: _Generic((inner_cycle),\
			IntForInfo*: testTime_DoubleDoubleInt,\
			default: printError),\
		default: printError),\
	default: printError)(func, outer_cycle, inner_cycle)

#define testTime_N(_0, _1, N,...) testTime_##N
#define testTime(func, outer_cycle, ...) \
    testTime_N(-1, ##__VA_ARGS__, 1, 0)(func, outer_cycle, ##__VA_ARGS__)



#define compareTime(fc) _Generic((fc),\
	FunctionComparison_dd*: compareTime_DoubleDouble,\
	FunctionComparison_ii*: compareTime_IntInt,\
	FunctionComparison_ddd*: compareTime_DoubleDoubleDouble,\
	FunctionComparison_dddp*: compareTime_DoubleDoubleDoubleref,\
	FunctionComparison_ddip*: compareTime_DoubleDoubleIntref,\
	FunctionComparison_ddi*: compareTime_DoubleDoubleInt,\
	default: printError) (fc)
	



void timeTestAll() {
	compareAbs();
	compareFabs();
	compareCeil();
	compareFloor();
	compareTrunc();
	compareRound();
}

void compareAbs() {
	FunctionInfo_ii f_myAbs = { myAbs, "myAbs" };
	FunctionInfo_ii f_abs = { abs, "abs" };
	IntForInfo cycle = { -10000000, 10000000, 1 };
	FunctionComparison_ii fc = { &f_myAbs, &f_abs, &cycle };
	compareTime(&fc);
}
void compareFabs() {
	FunctionInfo_dd f_myFabs = { myFabs, "myFabs" };
	FunctionInfo_dd f_fabs = { fabs, "fabs" };
	DoubleForInfo cycle = { -10000, 10000, 0.001 };
	FunctionComparison_dd fc = { &f_myFabs, &f_fabs, &cycle };
	compareTime(&fc);
}
void compareCeil() {
	FunctionInfo_dd f_myCeil = { myCeil, "myCeil" };
	FunctionInfo_dd f_ceil = { ceil, "ceil" };
	DoubleForInfo cycle = { -5000, 5000, 0.001 };
	FunctionComparison_dd fc = { &f_myCeil, &f_ceil, &cycle };
	compareTime(&fc);
}
void compareFloor() {
	FunctionInfo_dd f_myFloor = { myFloor, "myFloor" };
	FunctionInfo_dd f_floor = { floor, "floor" };
	DoubleForInfo cycle = { -5000, 5000, 0.001 };
	FunctionComparison_dd fc = { &f_myFloor, &f_floor, &cycle };
	compareTime(&fc);
}
void compareTrunc() {
	FunctionInfo_dd f_myTrunc = { myTrunc, "myTrunc" };
	FunctionInfo_dd f_trunc = { trunc, "trunc" };
	DoubleForInfo cycle = { -5000, 5000, 0.001 };
	FunctionComparison_dd fc = { &f_myTrunc, &f_trunc, &cycle };
	compareTime(&fc);
}
void compareRound() {
	FunctionInfo_dd f_myRound = { myRound, "myRound" };
	FunctionInfo_dd f_round = { round, "round" };
	DoubleForInfo cycle = { -5000, 5000, 0.001 };
	FunctionComparison_dd fc = { &f_myRound, &f_round, &cycle };
	compareTime(&fc);
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

void compareTime_DoubleDouble(FunctionComparison_dd* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}
void compareTime_IntInt(FunctionComparison_ii* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}
void compareTime_DoubleDoubleDouble(FunctionComparison_ddd* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle1, fc->cycle2);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle1, fc->cycle2);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleDoubleref(FunctionComparison_dddp* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleIntref(FunctionComparison_ddip* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleInt(FunctionComparison_ddi* fc) {
	unsigned long time1 = testTime(fc->funcInfo1->function, fc->cycle1, fc->cycle2);
	unsigned long time2 = testTime(fc->funcInfo2->function, fc->cycle1, fc->cycle2);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
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