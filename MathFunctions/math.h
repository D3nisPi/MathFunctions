#pragma once
#define POS_INFINITY *(double*)&POS_INF_ULL
#define NEG_INFINITY *(double*)&NEG_INF_ULL
#define POS_ZERO *(double*)&POS_ZERO_ULL
#define NEG_ZERO *(double*)&NEG_ZERO_ULL
#define NaN *(double*)&NaN_ULL

extern const double PI;
extern const double E;

extern const unsigned long long POS_INF_ULL;
extern const unsigned long long NEG_INF_ULL;
extern const unsigned long long POS_ZERO_ULL;
extern const unsigned long long NEG_ZERO_ULL;
extern const unsigned long long NaN_ULL;


int isNan(double);

int myAbs(int x);
double myFabs(double x);

double myCeil(double x);
double myFloor(double x);
double myTrunc(double x);
double myRound(double x);

double myFmod(double x, double y);
double myModf(double x, double* y);
double myFrexp(double x, int* y);
double myLdexp(double x, int y);

double mySin(double x);
double myCos(double x);
double myTan(double x);

double myAsin(double x);
double myAcos(double x);
double myAtan(double x);
double myAtan2(double x, double y);

double mySinh(double x);
double myCosh(double x);
double myTanh(double x);

double myExp(double x);
double myLog(double x);
double myLog10(double x);
double myPow(double x, double y);
double mySqrt(double x);