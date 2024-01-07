#include <stdio.h>
#include <math.h>
#include "math.h"
#include "test.h"

static const double EPSILON = 1e-7;

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
}
int testAbs() {
	for (int i = -1000; i <= 1000; i++) {
		if (myAbs(i) != abs(i))
			return -1;
	}
	return 0;
}
int testFabs() {
	for (double i = -1000; i <= 1000; i += 0.01) {
		if (myFabs(i) != fabs(i))
			return -1;
	}
	return 0;

	if (myFabs(POS_ZERO) != POS_ZERO)
		return -1;
	if (myFabs(NEG_ZERO) != POS_ZERO)
		return -1;
	if (myFabs(POS_INFINITY) != POS_INFINITY)
		return -1;
	if (myFabs(NEG_INFINITY) != POS_INFINITY)
		return -1;
	if (!isNan(myFabs(NEG_INFINITY)))
		return -1;
}

int testCeil() {
	double x;
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
	if (!isNan(myCeil(x)))
		return -1;

	return 0;
}
int testFloor() {
	double x;
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
	if (!isNan(myFloor(x)))
		return -1;

	return 0;
}
int testTrunc() {
	double x;
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
	if (!isNan(myTrunc(x)))
		return -1;

	return 0;
}
int testRound() {
	double x;
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
	if (!isNan(myRound(x)))
		return -1;

	return 0;
}
int testFmod() {
	double x, y;
	for (double i = -100; i <= 100; i += 0.1) {
		for (double j = -100; j <= 100; j += 0.1) {
			if (abs(myFmod(i, j) - fmod(i, j) > EPSILON))
				return -1;
		}
	}

	x = NaN;
	y = 1;
	if (!isNan(myFmod(x, y)))
		return -1;

	x = 1;
	y = NaN;
	if (!isNan(myFmod(x, y)))
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
	if (!isNan(myFmod(x, y)))
		return -1;

	x = NEG_INFINITY;
	y = 1;
	if (!isNan(myFmod(x, y)))
		return -1;

	x = 1;
	y = POS_ZERO;
	if (!isNan(myFmod(x, y)))
		return -1;

	x = 1;
	y = NEG_ZERO;
	if (!isNan(myFmod(x, y)))
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
	for (double i = -1000; i <= 1000; i += 0.01) {
		frac1 = myModf(i, &int1);
		frac2 = modf(i, &int2);
		if (int1 != int2 || frac1 != frac2)
			return -1;
	}

	x = POS_ZERO;
	frac1 = myModf(x, &int1);
	if (int1 != POS_ZERO || frac1 != POS_ZERO)
		return -1;

	x = NEG_ZERO;
	frac1 = myModf(x, &int1);
	if (int1 != NEG_ZERO || frac1 != NEG_ZERO)
		return -1;

	x = POS_INFINITY;
	frac1 = myModf(x, &int1);
	if (int1 != POS_INFINITY || frac1 != POS_ZERO)
		return -1;

	x = NEG_INFINITY;
	frac1 = myModf(x, &int1);
	if (int1 != NEG_INFINITY || frac1 != NEG_ZERO)
		return -1;

	x = NaN;
	frac1 = myModf(x, &int1);
	if (!isNan(int1) || !isNan(frac1))
		return -1;

	return 0;
}
