#include <stdio.h>
#include <math.h>
#include "math.h"
#include "test.h"

static const double EPSILON = 1e-10;

static int compareDoubles(double x, double y, double epsilon) {
	static const unsigned long long MANTISSA_MASK = 0x000FFFFFFFFFFFFFull;
	static const unsigned long long SIGN_MASK = 0x8000000000000000ull;
	static const unsigned long long EXP_MASK = 0x7FF0000000000000ull;

	unsigned long long ux = *(unsigned long long*) & x;
	unsigned long long uy = *(unsigned long long*) & y;

	unsigned long long ex = (ux & EXP_MASK) >> 52;
	unsigned long long ey = (uy & EXP_MASK) >> 52;

	// for small values
	if (ex < 1023 && ey  < 1023) {
		double diff = fabs(x - y);
		return diff <= epsilon;
	}
		
	// for large values set exponent to 0 for both numbers and shift mantissa (if required)
	unsigned long long sx = ux & SIGN_MASK;
	unsigned long long sy = uy & SIGN_MASK;

	unsigned long long min = ex <= ey ? ex : ey;

	ex -= min;
	ey -= min;

	unsigned long long mx = ux & (0x000FFFFFFFFFFFFFull >> ex) | 0x3FF0000000000000ull | sx;
	unsigned long long my = uy & (0x000FFFFFFFFFFFFFull >> ey) | 0x3FF0000000000000ull | sy;
	double dx = *(double*)&mx;
	double dy = *(double*)&my;
	return fabs(dx - dy) <= epsilon;
}

void test(int (*testFunction)(void), char* name) {
	int result = testFunction();
	if (result == 0) {
		printf("\033[1;32m[X] - %s passed\033[1;0m\n", name);
	} 
	else {
		printf("\033[1;31m[~] - %s failed\033[1;0m\n", name);
	}
}
void testAll() {
	test(testAbs, "abs");
	test(testFabs, "fabs");

	test(testCeil, "ceil");
	test(testFloor, "floor");
	test(testTrunc, "trunc");
	test(testRound, "round");

	test(testFmod, "fmod");
	test(testModf, "modf");
	test(testFrexp, "frexp");
	test(testLdexp, "ldexp");

	test(testSin, "sin");
	test(testCos, "cos");
	test(testTan, "tan");

	test(testAsin, "asin");
	test(testAcos, "acos");
	test(testAtan, "atan");
	test(testAtan2, "atan2");

	test(testSinh, "sinh");
	test(testCosh, "cosh");
	test(testTanh, "tanh");

	test(testExp, "exp");
	test(testLog, "log");
	test(testLog10, "log10");
	test(testPow, "pow");
	test(testSqrt, "sqrt");

}
int testAbs() {
	for (int i = -1000; i <= 1000; i++) {
		if (myAbs(i) != abs(i))
			return -1;
	}
	return 0;
}
int testFabs() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (myFabs(i) != fabs(i))
			return -1;
	}

	x = POS_ZERO;
	res = myFabs(x);
	if (!isPosZero(x))
		return -1;

	x = NEG_ZERO;
	res = myFabs(x);
	if (!isPosZero(res))
		return -1;

	x = POS_INFINITY;
	if (myFabs(x) != POS_INFINITY)
		return -1;

	x = NEG_INFINITY;
	if (myFabs(x) != POS_INFINITY)
		return -1;

	x = NaN;
	res = myFabs(x);
	if (!isNan(res))
		return -1;

	return 0;
}

int testCeil() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (myCeil(i) != ceil(i))
			return -1;
	}

	x = POS_ZERO;
	if (myCeil(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myCeil(x) != x)
		return -1;

	x = POS_INFINITY;
	if (myCeil(x) != x)
		return -1;

	x = NEG_INFINITY;
	if (myCeil(x) != x)
		return -1;

	x = NaN;
	res = myCeil(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testFloor() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (myFloor(i) != floor(i))
			return -1;
	}

	x = POS_ZERO;
	if (myFloor(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myFloor(x) != x)
		return -1;

	x = POS_INFINITY;
	if (myFloor(x) != x)
		return -1;

	x = NEG_INFINITY;
	if (myFloor(x) != x)
		return -1;

	x = NaN;
	res = myFloor(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testTrunc() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.1) {
		if (myTrunc(i) != trunc(i))
			return -1;
	}

	x = POS_ZERO;
	if (myTrunc(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myTrunc(x) != x)
		return -1;

	x = POS_INFINITY;
	if (myTrunc(x) != x)
		return -1;

	x = NEG_INFINITY;
	if (myTrunc(x) != x)
		return -1;

	x = NaN;
	res = myTrunc(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testRound() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.1) {
		if (myRound(i) != round(i))
			return -1;
	}

	x = POS_ZERO;
	if (myRound(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myRound(x) != x)
		return -1;

	x = POS_INFINITY;
	if (myRound(x) != x)
		return -1;

	x = NEG_INFINITY;
	if (myRound(x) != x)
		return -1;

	x = NaN;
	res = myRound(x);
	if (!isNan(res))
		return -1;

	return 0;
}

int testFmod() {
	double x, y, res;
	for (double i = -100; i <= 100; i += 0.1) {
		for (double j = -100; j <= 100; j += 0.1) {
			if (fabs(myFmod(i, j) - fmod(i, j)) > EPSILON)
				return -1;
		}
	}

	x = NaN;
	y = 1;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = 1;
	y = NaN;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = POS_ZERO;
	y = 1;
	if (myFmod(x, y) != x)
		return -1;

	x = NEG_ZERO;
	y = 1;
	if (myFmod(x, y) != x)
		return -1;

	x = POS_INFINITY;
	y = 1;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = NEG_INFINITY;
	y = 1;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = 1;
	y = POS_ZERO;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = 1;
	y = NEG_ZERO;
	res = myFmod(x, y);
	if (!isNan(res))
		return -1;

	x = 1;
	y = POS_INFINITY;
	if (myFmod(x, y) != x)
		return -1;

	x = 1;
	y = NEG_INFINITY;
	if (myFmod(x, y) != x)
		return -1;

	return 0;
}
int testModf() {
	double int1, int2, frac1, frac2, x;
	for (double i = -500; i <= 500; i += 0.01) {
		frac1 = myModf(i, &int1);
		frac2 = modf(i, &int2);
		if (int1 != int2 || frac1 != frac2)
			return -1;
	}

	x = POS_ZERO;
	frac1 = myModf(x, &int1);
	if (!isPosZero(int1) || !isPosZero(frac1))
		return -1;

	x = NEG_ZERO;
	frac1 = myModf(x, &int1);
	if (!isNegZero(int1) || !isNegZero(frac1))
		return -1;

	x = POS_INFINITY;
	frac1 = myModf(x, &int1);
	if (int1 != POS_INFINITY || !isPosZero(frac1))
		return -1;

	x = NEG_INFINITY;
	frac1 = myModf(x, &int1);
	if (int1 != NEG_INFINITY || !isNegZero(frac1))
		return -1;

	x = NaN;
	frac1 = myModf(x, &int1);
	if (!isNan(int1) || !isNan(frac1))
		return -1;

	return 0;
}
int testFrexp() {
	int exp1, exp2;
	double res1, res2, x;
	for (double i = -1000; i <= 1000; i += 0.01) {
		res1 = myFrexp(i, &exp1);
		res2 = frexp(i, &exp2);
		if (res1 != res2 || exp1 != exp2)
			return -1;
	}

	x = POS_ZERO; exp1 = 10;
	res1 = myFrexp(x, &exp1);
	if (res1 != x || exp1 != 0)
		return -1;

	x = NEG_ZERO; exp1 = 10;
	res1 = myFrexp(x, &exp1);
	if (res1 != x || exp1 != 0)
		return -1;

	x = POS_INFINITY; exp1 = 10;
	res1 = myFrexp(x, &exp1);
	if (res1 != x)
		return -1;

	x = NEG_INFINITY; exp1 = 10;
	res1 = myFrexp(x, &exp1);
	if (res1 != x)
		return -1;

	x = NaN; exp1 = 10;
	res1 = myFrexp(x, &exp1);
	if (!isNan(res1))
		return -1;

	return 0;
}
int testLdexp() {
	double x, res;
	int exp;
	for (double i = -100; i <= 100; i += 0.01) {
		for (int j = -30; j <= 31; j++) {
			if (fabs(myLdexp(i, j) - ldexp(i, j)) > EPSILON)
				return -1;
		}
	}

	x = 1;
	exp = 0;
	if (myLdexp(x, exp) != x)
		return -1;

	x = POS_ZERO;
	exp = 1;
	if (myLdexp(x, exp) != x)
		return -1;

	x = NEG_ZERO;
	exp = 1;
	if (myLdexp(x, exp) != x)
		return -1;

	x = POS_INFINITY;
	exp = 1;
	if (myLdexp(x, exp) != x)
		return -1;

	x = NEG_INFINITY;
	exp = 1;
	if (myLdexp(x, exp) != x)
		return -1;

	x = NaN;
	exp = 1;
	res = myLdexp(x, exp);
	if (!isNan(res))
		return -1;

	return 0;
}

int testSin() {
	double x, res;
	for (double i = -100; i <= 100; i += 0.01) {
		if (fabs(mySin(i) - sin(i)) > EPSILON)
			return -1;
	}

	x = POS_ZERO;
	if (mySin(x) != x)
		return -1;

	x = NEG_ZERO;
	if (mySin(x) != x)
		return -1;

	x = POS_INFINITY;
	res = mySin(x);
	if (!isNan(res))
		return -1;

	x = NEG_INFINITY;
	res = mySin(x);
	if (!isNan(res))
		return -1;

	x = NaN;
	res = mySin(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testCos() {
	double x, res;
	for (double i = -100; i <= 100; i += 0.01) {
		if (fabs(myCos(i) - cos(i)) > EPSILON)
			return -1;
	}

	x = POS_ZERO;
	if (myCos(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myCos(x) != x)
		return -1;

	x = POS_INFINITY;
	res = myCos(x);
	if (!isNan(res))
		return -1;

	x = NEG_INFINITY;
	res = myCos(x);
	if (!isNan(res))
		return -1;

	x = NaN;
	res = myCos(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testTan() {

}

int testAsin() {
	double x, res;
	for (double i = -1; i <= 1; i += 0.001) {
		if (fabs(myAsin(i) - asin(i)) > EPSILON)
			return -1;	
	}

	x = POS_ZERO;
	if (myAsin(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myAsin(x) != x)
		return -1;

	x = 10;
	res = myAsin(x);
	if (!isNan(res))
		return -1;

	x = -10;
	res = myAsin(x);
	if (!isNan(res))
		return -1;

	x = NaN;
	res = myAsin(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testAcos() {
	double x, res;
	for (double i = -1; i <= 1; i += 0.001) {
		if (fabs(myAcos(i) - acos(i)) > EPSILON)
			return -1;
	}

	x = 1;
	res = myAcos(x);
	if (!isPosZero(res))
		return -1;

	x = 10;
	res = myAcos(x);
	if (!isNan(res))
		return -1;

	x = -10;
	res = myAcos(x);
	if (!isNan(res))
		return -1;

	x = NaN;
	res = myAcos(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testAtan() {
	double x, res;
	for (double i = -100; i <= 100; i += 0.001) {
		if (fabs(myAtan(i) - atan(i)) > EPSILON)
			return -1;
	}

	x = POS_ZERO;
	if (myAtan(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myAtan(x) != x)
		return -1;

	x = POS_INFINITY;
	if (myAtan(x) != PI_OVER_TWO)
		return -1;

	x = NEG_INFINITY;
	if (myAtan(x) != -PI_OVER_TWO)
		return -1;

	x = NaN;
	res = myAtan(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testAtan2() {
	double y, x, res;
	for (double i = -10; i <= 10; i += 0.01) {
		for (double j = -10; j <= 10; j += 0.1) {
			if (fabs(myAtan2(i, j) - atan2(i, j)) > EPSILON)
				return -1;
		}		
	}

	y = POS_ZERO;
	x = -1;
	if (myAtan2(y, x) != PI)
		return -1;

	y = NEG_ZERO;
	x = -1;
	if (myAtan2(y, x) != -PI)
		return -1;


	y = POS_ZERO;
	x = 1;
	res = myAtan2(y, x);
	if (!isPosZero(res))
		return -1;

	y = NEG_ZERO;
	x = 1;
	res = myAtan2(y, x);
	if (!isNegZero(res))
		return -1;

	y = POS_INFINITY;
	x = 1;
	if (myAtan2(y, x) != PI_OVER_TWO)
		return -1;

	y = NEG_INFINITY;
	x = 1;
	if (myAtan2(y, x) != -PI_OVER_TWO)
		return -1;

	y = POS_INFINITY;
	x = NEG_INFINITY;
	if (myAtan2(y, x) != THREE_PI_OVER_FOUR)
		return -1;

	y = NEG_INFINITY;
	x = NEG_INFINITY;
	if (myAtan2(y, x) != -THREE_PI_OVER_FOUR)
		return -1;

	y = POS_INFINITY;
	x = POS_INFINITY;
	if (myAtan2(y, x) != PI_OVER_FOUR)
		return -1;


	y = -1;
	x = POS_ZERO;
	if (myAtan2(y, x) != -PI_OVER_TWO)
		return -1;

	y = -1;
	x = NEG_ZERO;
	if (myAtan2(y, x) != -PI_OVER_TWO)
		return -1;

	y = 1;
	x = POS_ZERO;
	if (myAtan2(y, x) != PI_OVER_TWO)
		return -1;

	y = 1;
	x = NEG_ZERO;
	if (myAtan2(y, x) != PI_OVER_TWO)
		return -1;

	y = 1;
	x = NEG_INFINITY;
	if (myAtan2(y, x) != PI)
		return -1;

	y = -1;
	x = NEG_INFINITY;
	if (myAtan2(y, x) != -PI)
		return -1;

	y = 1;
	x = POS_INFINITY;
	res = myAtan2(y, x);
	if (!isPosZero(res))
		return -1;

	y = -1;
	x = POS_INFINITY;
	res = myAtan2(y, x);
	if (!isNegZero(res))
		return -1;

	y = NaN;
	x = 1;
	res = myAtan2(y, x);
	if (!isNan(res))
		return -1;

	y = 1;
	x = NaN;
	res = myAtan2(y, x);
	if (!isNan(res))
		return -1;


	y = NaN;
	x = NaN;
	res = myAtan2(y, x);
	if (!isNan(res))
		return -1;

	return 0;
}

int testSinh() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.05) {
		if (!compareDoubles(mySinh(i), sinh(i), EPSILON))
			return -1;	
	}

	x = POS_ZERO;
	if (mySinh(x) != x)
		return -1;

	x = NEG_ZERO;
	if (mySinh(x) != x)
		return -1;

	x = POS_INFINITY;
	if (mySinh(x) != x)
		return -1;

	x = NEG_INFINITY;
	if (mySinh(x) != x)
		return -1;

	x = NaN;
	res = mySinh(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testCosh() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.001) {
		if (!compareDoubles(myCosh(i), cosh(i), EPSILON))
			return -1;
	}

	x = POS_ZERO;
	if (myCosh(x) != 1)
		return -1;

	x = NEG_ZERO;
	if (myCosh(x) != 1)
		return -1;

	x = POS_INFINITY;
	if (myCosh(x) != POS_INFINITY)
		return -1;

	x = NEG_INFINITY;
	if (myCosh(x) != POS_INFINITY)
		return -1;

	x = NaN;
	res = myCosh(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testTanh() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (fabs(myTanh(i) - tanh(i)) > EPSILON)
			return -1;
	}

	x = POS_ZERO;
	if (myTanh(x) != x)
		return -1;

	x = NEG_ZERO;
	if (myTanh(x) != x)
		return -1;
	x = POS_INFINITY;
	if (myTanh(x) != 1)
		return -1;

	x = NEG_INFINITY;
	if (myTanh(x) != -1)
		return -1;

	x = NaN;
	res = myTanh(x);
	if (!isNan(res))
		return -1;

	return 0;
}

int testExp() {
	double x, res;
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (!compareDoubles(myExp(i), exp(i), EPSILON))
			return -1;
	}

	x = POS_ZERO;
	if (myExp(x) != 1)
		return -1;

	x = NEG_ZERO;
	if (myExp(x) != 1)
		return -1;

	x = NEG_INFINITY;
	res = myExp(x);
	if (!isPosZero(res))
		return -1;

	x = POS_INFINITY;
	if (myExp(x) != POS_INFINITY)
		return -1;

	x = NaN;
	res = myExp(x);
	if (!isNan(res))
		return -1;

	return 0;
}
int testLog() {

}
int testLog10() {

}
int testPow() {

}
int testSqrt() {
	double x, res;
	for (double i = 0; i <= 1000; i += 0.001) {
		if (fabs(mySqrt(i) - sqrt(i)) > EPSILON) {
			return -1;
		}
	}

	x = -1;
	res = mySqrt(x);
	if (!isNan(res))
		return -1;

	x = POS_INFINITY;
	if (mySqrt(x) != x)
		return -1;

	x = POS_ZERO;
	if (mySqrt(x) != x)
		return -1;

	x = NEG_ZERO;
	if (mySqrt(x) != x)
		return -1;

	x = NaN;
	res = mySqrt(x);
	if (!isNan(res))
		return -1;

	return 0;
}