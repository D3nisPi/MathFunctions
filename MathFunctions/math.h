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
