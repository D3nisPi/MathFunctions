#include <stdio.h>
#include <math.h>
#include "math.h"
#include "test.h"


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
