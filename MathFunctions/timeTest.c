#include <stdio.h>
#include <time.h>
#include <math.h>
#include "math.h"
#include "timeTest.h"
#include <stdint.h>

static void printError(void* arg, ...) {
	fprintf(stderr, "An error occurred when using overloaded functions");
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
	

static void printLine() {
	printf("----------------------------------------\n");
}

void timeTestAll() {
	compareAbs();
	compareFabs();

	compareCeil();
	compareFloor();
	compareTrunc();
	compareRound();

	compareFmod();
	compareModf();
	compareFrexp();
	compareLdexp();

	compareSin();
	compareCos();
	compareTan();

	compareAsin();
	compareAcos();
	compareAtan();
	compareAtan2();

	compareSinh();
	compareCosh();
	compareTanh();

	compareExp();
	compareLog();
	compareLog10();
	comparePow();
	compareSqrt();
}


void compareAbs() {
	printLine();
	FunctionInfo_ii f_myAbs = { myAbs, "myAbs" };
	FunctionInfo_ii f_abs = { abs, "abs" };
	IntForInfo cycle = { -1000000, 1000000, 1 };
	FunctionComparison_ii fc = { &f_myAbs, &f_abs, &cycle };
	compareTime(&fc);
}
void compareFabs() {
	printLine();
	FunctionInfo_dd f_myFabs = { myFabs, "myFabs" };
	FunctionInfo_dd f_fabs = { fabs, "fabs" };
	DoubleForInfo cycle = { -1000, 1000, 0.001 };
	FunctionComparison_dd fc = { &f_myFabs, &f_fabs, &cycle };
	compareTime(&fc);
}
void compareCeil() {
	printLine();
	FunctionInfo_dd f_myCeil = { myCeil, "myCeil" };
	FunctionInfo_dd f_ceil = { ceil, "ceil" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_dd fc = { &f_myCeil, &f_ceil, &cycle };
	compareTime(&fc);
}
void compareFloor() {
	printLine();
	FunctionInfo_dd f_myFloor = { myFloor, "myFloor" };
	FunctionInfo_dd f_floor = { floor, "floor" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_dd fc = { &f_myFloor, &f_floor, &cycle };
	compareTime(&fc);
}
void compareTrunc() {
	printLine();
	FunctionInfo_dd f_myTrunc = { myTrunc, "myTrunc" };
	FunctionInfo_dd f_trunc = { trunc, "trunc" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_dd fc = { &f_myTrunc, &f_trunc, &cycle };
	compareTime(&fc);
}
void compareRound() {
	printLine();
	FunctionInfo_dd f_myRound = { myRound, "myRound" };
	FunctionInfo_dd f_round = { round, "round" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_dd fc = { &f_myRound, &f_round, &cycle };
	compareTime(&fc);
}
void compareFmod() {
	printLine();
	FunctionInfo_ddd f_myFmod = { myFmod, "myFmod" };
	FunctionInfo_ddd f_fmod = { fmod, "fmod" };
	DoubleForInfo cycle1 = { -50, 50, 0.1 };
	DoubleForInfo cycle2 = { -50, 50, 0.1 };
	FunctionComparison_ddd fc = { &f_myFmod, &f_fmod, &cycle1, &cycle2 };
	compareTime(&fc);
}
void compareModf() {
	printLine();
	FunctionInfo_dddp f_myModf = { myModf, "myModf" };
	FunctionInfo_dddp f_modf = { modf, "modf" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_dddp fc = { &f_myModf, &f_modf, &cycle};
	compareTime(&fc);
}
void compareFrexp() {
	printLine();
	FunctionInfo_ddip f_myFrexp = { myFrexp, "myFrexp" };
	FunctionInfo_ddip f_frexp = { frexp, "frexp" };
	DoubleForInfo cycle = { -5000, 5000, 0.01 };
	FunctionComparison_ddip fc = { &f_myFrexp, &f_frexp, &cycle };
	compareTime(&fc);
}
void compareLdexp() {
	printLine();
	FunctionInfo_ddi f_myLdexp = { myLdexp, "myLdexp" };
	FunctionInfo_ddi f_ldexp = { ldexp, "ldexp" };
	DoubleForInfo cycle = { -50, 50, 0.01 };
	IntForInfo cycle2 = { -50, 50, 1 };
	FunctionComparison_ddi fc = { &f_myLdexp, &f_ldexp, &cycle, &cycle2 };
	compareTime(&fc);
}
void compareSin() {
	printLine();
	FunctionInfo_dd f_mySin = { mySin, "mySin" };
	FunctionInfo_dd f_sin = { sin, "sin" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_mySin, &f_sin, &cycle };
	compareTime(&fc);
}
void compareCos() {
	printLine();
	FunctionInfo_dd f_myCos = { myCos, "myCos" };
	FunctionInfo_dd f_cos = { cos, "cos" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_myCos, &f_cos, &cycle };
	compareTime(&fc);
}
void compareTan() {
	printLine();
	FunctionInfo_dd f_myTan = { myTan, "myTan" };
	FunctionInfo_dd f_tan = { tan, "tan" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_myTan, &f_tan, &cycle };
	compareTime(&fc);
}
void compareAsin() {
	printLine();
	FunctionInfo_dd f_myAsin = { myAsin, "myAsin" };
	FunctionInfo_dd f_asin = { asin, "asin" };
	DoubleForInfo cycle = { -1, 1, 0.00001 };
	FunctionComparison_dd fc = { &f_myAsin, &f_asin, &cycle };
	compareTime(&fc);
}
void compareAcos() {
	printLine();
	FunctionInfo_dd f_myAcos = { myAcos, "myAcos" };
	FunctionInfo_dd f_acos = { acos, "acos" };
	DoubleForInfo cycle = { -1, 1, 0.00001 };
	FunctionComparison_dd fc = { &f_myAcos, &f_acos, &cycle };
	compareTime(&fc);
}
void compareAtan() {
	printLine();
	FunctionInfo_dd f_myAtan = { myAtan, "myAtan" };
	FunctionInfo_dd f_atan = { atan, "atan" };
	DoubleForInfo cycle = { -250, 250, 0.001 };
	FunctionComparison_dd fc = { &f_myAtan, &f_atan, &cycle };
	compareTime(&fc);
}
void compareAtan2() {
	printLine();
	FunctionInfo_ddd f_myAtan2 = { myAtan2, "myAtan2" };
	FunctionInfo_ddd f_atan2 = { atan2, "atan2" };
	DoubleForInfo cycle1 = { -25, 25, 0.1 };
	DoubleForInfo cycle2 = { -25, 25, 0.1 };
	FunctionComparison_ddd fc = { &f_myAtan2, &f_atan2, &cycle1, &cycle2 };
	compareTime(&fc);
}
void compareSinh() {
	printLine();
	FunctionInfo_dd f_mySinh = { mySinh, "mySinh" };
	FunctionInfo_dd f_sinh = { sinh, "sinh" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_mySinh, &f_sinh, &cycle };
	compareTime(&fc);
}
void compareCosh() {
	printLine();
	FunctionInfo_dd f_myCosh = { myCosh, "myCosh" };
	FunctionInfo_dd f_cosh = { cosh, "cosh" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_myCosh, &f_cosh, &cycle };
	compareTime(&fc);
}
void compareTanh() {
	printLine();
	FunctionInfo_dd f_myTanh = { myTanh, "myTanh" };
	FunctionInfo_dd f_tanh = { tanh, "tanh" };
	DoubleForInfo cycle = { -100, 100, 0.001 };
	FunctionComparison_dd fc = { &f_myTanh, &f_tanh, &cycle };
	compareTime(&fc);
}
void compareExp() {
	printLine();
	FunctionInfo_dd f_myExp = { myExp, "myExp" };
	FunctionInfo_dd f_exp = { exp, "exp" };
	DoubleForInfo cycle = { -750, 750, 0.005 };
	FunctionComparison_dd fc = { &f_myExp, &f_exp, &cycle };
	compareTime(&fc);
}
void compareLog() {
	printLine();
	FunctionInfo_dd f_myLog = { myLog, "myLog" };
	FunctionInfo_dd f_log = { log, "log" };
	DoubleForInfo cycle = { 0, 10000, 0.01 };
	FunctionComparison_dd fc = { &f_myLog, &f_log, &cycle };
	compareTime(&fc);
}
void compareLog10() {
	printLine();
	FunctionInfo_dd f_myLog10 = { myLog10, "myLog10" };
	FunctionInfo_dd f_log10 = { log10, "log10" };
	DoubleForInfo cycle = { 0, 10000, 0.01 };
	FunctionComparison_dd fc = { &f_myLog10, &f_log10, &cycle };
	compareTime(&fc);
}
void comparePow() {
	printLine();
	FunctionInfo_ddd f_myPow = { myPow, "myPow" };
	FunctionInfo_ddd f_pow = { pow, "pow" };
	DoubleForInfo cycle1 = { -25, 25, 0.25 };
	DoubleForInfo cycle2 = { -10, 10, 0.01 };
	FunctionComparison_ddd fc = { &f_myPow, &f_pow, &cycle1, &cycle2 };
	compareTime(&fc);
}
void compareSqrt() {
	printLine();
	FunctionInfo_dd f_mySqrt = { mySqrt, "mySqrt" };
	FunctionInfo_dd f_sqrt = { sqrt, "sqrt" };
	DoubleForInfo cycle = { 0, 1000, 0.001 };
	FunctionComparison_dd fc = { &f_mySqrt, &f_sqrt, &cycle };
	compareTime(&fc);
}


uint64_t testTime_DoubleDouble(double (*function)(double), DoubleForInfo* cycle) {
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (double i = cycle->start; i <= cycle->end; i += cycle->step) {
		function(i);
	}


	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}
uint64_t testTime_IntInt(int (*function)(int), IntForInfo* cycle) {
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (int i = cycle->start; i <= cycle->end; i += cycle->step) {
		function(i);
	}
	

	if(timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}
uint64_t testTime_DoubleDoubleDouble(double (*function)(double, double), DoubleForInfo* cycle1, DoubleForInfo* cycle2) {
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		for (double j = cycle2->start; j <= cycle2->end; j += cycle2->step) {
			function(i, j);
		}
	}


	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}
uint64_t testTime_DoubleDoubleDoubleref(double (*function)(double, double*), DoubleForInfo* cycle1) {
	double out;
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		function(i, &out);
	}
	

	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}
uint64_t testTime_DoubleDoubleIntref(double (*function)(double, int*), DoubleForInfo* cycle1) {
	int out;
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		function(i, &out);
	}


	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}
uint64_t testTime_DoubleDoubleInt(double (*function)(double, int), DoubleForInfo* cycle1, IntForInfo* cycle2) {
	int out;
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	uint64_t microseconds = (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3;


	for (double i = cycle1->start; i <= cycle1->end; i += cycle1->step) {
		for (int j = cycle2->start; j <= cycle2->end; j += cycle2->step) {
			function(i, j);
		}
	}
	

	if (timespec_get(&ts, TIME_UTC) == 0) return UINT64_MAX;
	return (uint64_t)ts.tv_sec * 1e6 + (uint64_t)ts.tv_nsec / 1e3 - microseconds;
}

void compareTime_DoubleDouble(FunctionComparison_dd* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}
void compareTime_IntInt(FunctionComparison_ii* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}
void compareTime_DoubleDoubleDouble(FunctionComparison_ddd* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle1, fc->cycle2);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle1, fc->cycle2);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleDoubleref(FunctionComparison_dddp* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleIntref(FunctionComparison_ddip* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}

void compareTime_DoubleDoubleInt(FunctionComparison_ddi* fc) {
	uint64_t time1 = testTime(fc->funcInfo1->function, fc->cycle1, fc->cycle2);
	uint64_t time2 = testTime(fc->funcInfo2->function, fc->cycle1, fc->cycle2);
	printResult(time1, time2, fc->funcInfo1->functionName, fc->funcInfo2->functionName);
}



void printResult(uint64_t time1, uint64_t time2, char* name1, char* name2) {
	float d;
	if (time1 < time2) {
		d = (float)time2 / time1;
		printf("%s is \033[1;32m%.2f times faster\033[1;0m than %s\n", name1, d, name2);
		printf("\t%7s : %10llu mcs\n", name1, time1);
		printf("\t%7s : %10llu mcs\n", name2, time2);
	}
	else if (time1 > time2) {
		d = (float)time1 / time2;
		printf("%s is \033[1;31m%.2f times slower\033[1;0m than %s\n", name1, d, name2);
		printf("\t%7s : %10llu mcs\n", name1, time1);
		printf("\t%7s : %10llu mcs\n", name2, time2);
	}
	else {
		printf("%s\'s \033[1;33mtime equals\033[1;0m to %s\'s time\n", name1, name2);
		printf("\t%7s : %10llu mcs\n", name1, time1);
		printf("\t%7s : %10llu mcs\n", name2, time2);
	}
}