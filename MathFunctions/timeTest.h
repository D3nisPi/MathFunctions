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

typedef struct {
	double (*function) (double);
	char* functionName;
} FunctionInfo_dd;
typedef struct {
	int (*function) (int);
	char* functionName;
} FunctionInfo_ii;
typedef struct {
	double (*function) (double, double);
	char* functionName;
} FunctionInfo_ddd;
typedef struct {
	double (*function) (double, double*);
	char* functionName;
} FunctionInfo_dddp;
typedef struct {
	double (*function) (double, int*);
	char* functionName;
} FunctionInfo_ddip;
typedef struct {
	double (*function) (double, int);
	char* functionName;
} FunctionInfo_ddi;


typedef struct {
	FunctionInfo_dd* funcInfo1;
	FunctionInfo_dd* funcInfo2;
	DoubleForInfo* cycle;
} FunctionComparison_dd;
typedef struct {
	FunctionInfo_ii* funcInfo1;
	FunctionInfo_ii* funcInfo2;
	IntForInfo* cycle;
} FunctionComparison_ii;
typedef struct {
	FunctionInfo_ddd* funcInfo1;
	FunctionInfo_ddd* funcInfo2;
	DoubleForInfo* cycle1;
	DoubleForInfo* cycle2;
} FunctionComparison_ddd;
typedef struct {
	FunctionInfo_dddp* funcInfo1;
	FunctionInfo_dddp* funcInfo2;
	DoubleForInfo* cycle;
} FunctionComparison_dddp;
typedef struct {
	FunctionInfo_ddip* funcInfo1;
	FunctionInfo_ddip* funcInfo2;
	DoubleForInfo* cycle;
} FunctionComparison_ddip;
typedef struct {
	FunctionInfo_ddi* funcInfo1;
	FunctionInfo_ddi* funcInfo2;
	DoubleForInfo* cycle1;
	IntForInfo* cycle2;
} FunctionComparison_ddi;


void compareAbs(void);
void compareFabs(void);
void compareCeil(void);
void compareFloor(void);
void compareTrunc(void);
void compareRound(void);



void timeTestAll();

void printResult(unsigned long time1, unsigned long time2, char* name1, char* name2);

unsigned long testTime_DoubleDouble(double (*function)(double), DoubleForInfo* cycle);
unsigned long testTime_IntInt(int (*function)(int), IntForInfo* cycle);
unsigned long testTime_DoubleDoubleDouble(double (*function)(double, double), DoubleForInfo* cycle1, DoubleForInfo* cycle2);
unsigned long testTime_DoubleDoubleDoubleref(double (*function)(double, double*), DoubleForInfo* cycle1);
unsigned long testTime_DoubleDoubleIntref(double (*function)(double, int*), DoubleForInfo* cycle1);
unsigned long testTime_DoubleDoubleInt(double (*function)(double, int), DoubleForInfo* cycle1, IntForInfo* cycle2);

void compareTime_DoubleDouble(FunctionComparison_dd*);
void compareTime_IntInt(FunctionComparison_ii*);
void compareTime_DoubleDoubleDouble(FunctionComparison_ddd*);
void compareTime_DoubleDoubleDoubleref(FunctionComparison_dddp*);
void compareTime_DoubleDoubleIntref(FunctionComparison_ddip*);
void compareTime_DoubleDoubleInt(FunctionComparison_ddi*);