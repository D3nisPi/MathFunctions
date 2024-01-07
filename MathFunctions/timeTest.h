#pragma once

typedef struct {
	double start;
	double end;
	double step;
} DoubleForInfo;
typedef struct {
	int start;
	int end;
	int step;
} IntForInfo;

void timeTestAll();

void printResult(unsigned long time1, unsigned long time2, char* name1, char* name2);

unsigned long testTime_DoubleDouble(double (*function)(double), DoubleForInfo* cycle);
unsigned long testTime_IntInt(int (*function)(int), IntForInfo* cycle);
unsigned long testTime_DoubleDoubleDouble(double (*function)(double, double), DoubleForInfo* cycle1, DoubleForInfo* cycle2);
unsigned long testTime_DoubleDoubleDoubleref(double (*function)(double, double*), DoubleForInfo* cycle1);
unsigned long testTime_DoubleDoubleIntref(double (*function)(double, int*), DoubleForInfo* cycle1);
unsigned long testTime_DoubleDoubleInt(double (*function)(double, int), DoubleForInfo* cycle1, IntForInfo* cycle2);

void compareTime_DoubleDouble(double (*myFunction)(double), double (*mathFunction)(double),
	char* name1, char* name2, DoubleForInfo* cycle);
void compareTime_IntInt(int (*myFunction)(int), int (*mathFunction)(int),
	char* name1, char* name2, IntForInfo* cycle);
void compareTime_DoubleDoubleDouble(double (*myFunction)(double, double), double (*mathFunction)(double, double),
	char* name1, char* name2, DoubleForInfo* cycle1, DoubleForInfo* cycle2);
void compareTime_DoubleDoubleDoubleref(double (*myFunction)(double, double*), double (*mathFunction)(double, double*),
	char* name1, char* name2, DoubleForInfo* cycle);
void compareTime_DoubleDoubleIntref(double (*myFunction)(double, int*), double (*mathFunction)(double, int*),
	char* name1, char* name2, DoubleForInfo* cycle);
void compareTime_DoubleDoubleInt(double (*myFunction)(double, int), double (*mathFunction)(double, int),
	char* name1, char* name2, DoubleForInfo* cycle1, IntForInfo* cycle2);