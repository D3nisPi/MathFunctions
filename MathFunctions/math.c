#include "math.h"

#define POS_INFINITY *(double*)&POS_INF_ULL
#define NEG_INFINITY *(double*)&NEG_INF_ULL
#define POS_ZERO *(double*)&POS_ZERO_ULL
#define NEG_ZERO *(double*)&NEG_ZERO_ULL
#define NaN *(double*)&NaN_ULL


const double PI = 3.141592653589793238462643383279;
const double TWO_PI = 6.283185307179586231995926937088;
const double PI_OVER_TWO = 1.570796326794896557998981734272;
const double PI_OVER_THREE = 1.047197551196597631317786181171;
const double PI_OVER_FOUR = 0.785398163397448278999490867136;
const double PI_OVER_SIX = 0.523598775598298815658893090585;
const double THREE_PI_OVER_TWO = 4.712388980384689673996945202816;
const double THREE_PI_OVER_FOUR = 2.356194490192344836998472601408;

const double E = 2.718281828459045235360287471352;


const unsigned long long POS_ZERO_ULL = 0x0000000000000000ull;
const unsigned long long NEG_ZERO_ULL = 0x8000000000000000ull;
const unsigned long long POS_INF_ULL = 0x7FF0000000000000ull;
const unsigned long long NEG_INF_ULL = 0xFFF0000000000000ull;
const unsigned long long NaN_ULL = 0x7FF8000000000000ull;

static const double EPSILON = 1e-11;
static const int BIAS = 1023;
static const int MANTISSA_BITS = 52;
static const int INF_NAN_EXP = 2047;
static const unsigned long long MANTISSA_MASK = 0x000FFFFFFFFFFFFFull;
static const unsigned long long SIGN_MASK = 0x8000000000000000ull;
static const unsigned long long EXP_MASK = 0x7FF0000000000000ull;
static const unsigned long long FIRST_BIT_OF_MANTISSA_MASK = 0x0008000000000000ull;

#define getIntExponent(x) (((*(unsigned long long*) & x) & EXP_MASK) >> MANTISSA_BITS)
#define getExponent_ULL(x) ((*(unsigned long long*) & x) & EXP_MASK)
#define getMantissa(x) ((*(unsigned long long*) & x) & MANTISSA_MASK)
#define getSign(x) ((*(unsigned long long*) & x) & SIGN_MASK)


#define getIntegerMantissaPart(x, exp) ((*(unsigned long long*) & x) & (~(MANTISSA_MASK >> exp) & MANTISSA_MASK))
#define getFractionalMantissaPart(x, exp) ((*(unsigned long long*) & x) & (MANTISSA_MASK >> exp))
#define getFirstBitOfFractionalPart(x, exp) ((*(unsigned long long*) & x) & (FIRST_BIT_OF_MANTISSA_MASK >> exp))

#define _isNan(x) (getExponent_ULL(x) == EXP_MASK && getMantissa(x) != 0)

int isNan(double x) {
	int exp = getIntExponent(x);
	unsigned long long mantissa = getMantissa(x);
	return exp == INF_NAN_EXP && mantissa != 0;
}
// abs - https://en.cppreference.com/w/c/numeric/math/abs
int myAbs(register int x) {
	return x < 0 ? -x : x;
}

// fabs - https://en.cppreference.com/w/c/numeric/math/fabs
double myFabs(register double x) {	
	// checking x for +-inf/ +-0 and NaN is not necessary because
	// +-0 -> +0
	// +-inf -> +inf
	// NaN -> NaN with plus sign (still NaN)
	return x < 0 ? -x : x;
}

// ceil = https://en.cppreference.com/w/c/numeric/math/ceil
double myCeil(double x) {
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	const int BIASED_NAN_EXPONENT = 1024;
	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x);

	if (exp == BIASED_NAN_EXPONENT && mantissa != 0) return NaN;

	if (exp >= 52) return x;
	if (exp <= -1) return (x < 0) ? 0 : 1; // (-1; 1)

	mantissa = mantissa & (MANTISSA_MASK >> exp);
	if (mantissa == 0) return x;

	unsigned long long integer = *(unsigned long long*) & x ^ mantissa;
	double result = *(double*)&integer;
	return x > 0 ? result + 1 : result;
}

// floor - https://en.cppreference.com/w/c/numeric/math/floor
double myFloor(double x) {
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	const int BIASED_NAN_EXPONENT = 1024;
	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x);

	if (exp == BIASED_NAN_EXPONENT && mantissa != 0) return NaN;
	if (exp >= 52) return x;
	if (exp <= -1) return (x < 0) ? -1 : 0; // (-1; 1)

	mantissa = mantissa & (MANTISSA_MASK >> exp);
	if (mantissa == 0) return x;

	unsigned long long integer = *(unsigned long long*) & x ^ mantissa;
	double result = *(double*)&integer;
	return x > 0 ? result : result - 1;
}

// trunc - https://en.cppreference.com/w/c/numeric/math/trunc
double myTrunc(double x) {
	// myCeil and myFloor implement checks for +-0, +-inf and NaN
	return x < 0 ? myCeil(x) : myFloor(x);
}

// round - https://en.cppreference.com/w/c/numeric/math/round
double myRound(double x) {
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	const int BIASED_NAN_EXPONENT = 1024;
	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x);

	if (exp == BIASED_NAN_EXPONENT && mantissa != 0) return NaN;

	if (exp >= 52) return x;
	if (exp == -1) return (x > 0) ? 1 : -1; // (-1; -0.5] & [0.5; 1)
	if (exp <= -2) return 0; // (-0.5; 0.5)

	mantissa = mantissa & (MANTISSA_MASK >> exp);
	if (mantissa == 0) return x;

	unsigned long long integer = *(unsigned long long*) & x ^ mantissa;
	double result = *(double*)&integer;

	unsigned long long v = getFirstBitOfFractionalPart(x, exp);
	if (v == 0) return result;
	else return (x > 0) ? result + 1 : result - 1;
}

// fmod - https://en.cppreference.com/w/c/numeric/math/fmod
double myFmod(double x, double y) {
	if (_isNan(x) || _isNan(y))
		return NaN;
	if ((x == POS_ZERO || x == NEG_ZERO) && y != 0)
		return x;
	if ((x == POS_INFINITY || x == NEG_INFINITY) && !_isNan(y))
		return NaN;
	if ((y == POS_ZERO || y == NEG_ZERO) && !_isNan(x))
		return NaN;
	if ((y == POS_INFINITY || y == NEG_INFINITY) && x != POS_INFINITY && x != NEG_INFINITY)
		return x;

	return x - myTrunc(x / y) * y;
}

// modf - https://en.cppreference.com/w/c/numeric/math/modf
double myModf(double x, double* y) {
	if (x == POS_ZERO || x == NEG_ZERO) {
		*y = x;
		return x;
	}
	if (x == POS_INFINITY) {
		*y = x;
		return POS_ZERO;
	}
	if (x == NEG_INFINITY) {
		*y = x;
		return POS_ZERO;
	}

	const unsigned long long NAN_EXPONENT = 0x7FF0000000000000;

	unsigned long long sign = getSign(x);
	unsigned long long exponent = getExponent_ULL(x);
	unsigned long long mantissa = getMantissa(x);

	if (exponent == NAN_EXPONENT && mantissa != 0) {
		*y = NaN;
		return NaN;
	}
	int exp = (exponent >> MANTISSA_BITS) - BIAS;
	if (exp >= 52) {
		*y = x;
		return 0;
	}
	if (exp <= -1) {
		*y = 0;
		return x;
	}

	unsigned long long integer = mantissa & (~(MANTISSA_MASK >> exp) & MANTISSA_MASK);
	unsigned long long i_result = sign | exponent | integer;

	*y = *(double*)&i_result;
	return x - *(double*)&i_result;
}

// frexp - https://en.cppreference.com/w/c/numeric/math/frexp
double myFrexp(double x, int* y) {
	if (x == POS_ZERO || x == NEG_ZERO) {
		*y = 0;
		return x;
	}
	if (x == POS_INFINITY || x == NEG_INFINITY)
		return x;

	const unsigned long long MASK = 0x3FE0000000000000ull;
	const int BIASED_NAN_EXPONENT = 1024;

	int exp = getIntExponent(x) - BIAS;
	unsigned long long sign = getSign(x);
	unsigned long long mantissa = getMantissa(x);

	if (exp == BIASED_NAN_EXPONENT && mantissa != 0)
		return NaN;

	*y = exp + 1;
	mantissa |= sign | MASK;
	return *(double*)&mantissa;
}

// ldexp - https://en.cppreference.com/w/c/numeric/math/ldexp
// if overflow occurs, POS_INF/NEG_INF is returned
// if underflow occurs, POS_ZERO/NEG_ZERO is returned
double myLdexp(double x, int y) {
	if (_isNan(x))
		return NaN;
	if (y == 0 || x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	int exp = getIntExponent(x);
	unsigned long long sign = getSign(x);

	// overflow
	if (exp + y >= INF_NAN_EXP) {
		return sign == 0 ? POS_INFINITY : NEG_INFINITY;
	}
	// underflow
	else if (exp + y < 0) {
		return sign == 0 ? POS_ZERO : NEG_ZERO;
	}

	unsigned long long result = sign | ((unsigned long long)(exp + y) << MANTISSA_BITS) | getMantissa(x);
	return *(double*)&result;
}

// sin - https://en.cppreference.com/w/c/numeric/math/sin
double mySin(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (x == POS_INFINITY || x == NEG_INFINITY || _isNan(x))
		return NaN;

	x = x - myTrunc(x / TWO_PI) * TWO_PI;
	if (x < -PI) x += 2 * PI;
	if (x > PI) x -= 2 * PI; // x - [-PI ; PI]

	// Taylor series around pi/2
	double t = myFabs(x) - PI / 2;
	double t_squared = t * t;
	double sum = 0;
	int i = 1;
	double current = 1;

	do { 
		sum += current;
		current = -current * t_squared / ((2 * i) * (2 * i - 1));
		i++;
	} while (myFabs(current) >= EPSILON);

	return x > 0 ? sum : -sum;
}

// cos - https://en.cppreference.com/w/c/numeric/math/cos
double myCos(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (x == POS_INFINITY || x == NEG_INFINITY || _isNan(x))
		return NaN;

	x = x - myTrunc(x / TWO_PI) * TWO_PI;
	if (x < -PI) x += 2 * PI;
	if (x > PI) x -= 2 * PI; // x - [-PI ; PI]

	// Taylor series around pi/2
	double sum = 0;
	int i = 1;
	double a = myFabs(x) - PI / 2;
	double a_squared = a * a;

	double current = -a;
	do {
		sum += current;
		current = -current * a_squared / ((2 * i) * (2 * i + 1));
		i++;
	} while (myFabs(current) >= EPSILON);

	return sum;
}

// tan - https://en.cppreference.com/w/c/numeric/math/tan
double myTan(double x) {

}
// asin - https://en.cppreference.com/w/c/numeric/math/asin
double myAsin(double x) {
	double abs_x = myFabs(x);

	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (abs_x > 1 || _isNan(x))
		return NaN;


	if (x > 0.5)
		return PI_OVER_TWO - 2 * myAsin(mySqrt((1 - x) / 2));
	if (x < -0.5)
		return -PI_OVER_TWO + 2 * myAsin(mySqrt((1 - abs_x) / 2));
		

	// Taylor series
	double sum = 0;
	int i = 1;
	double x_squared = x * x;
	double current = abs_x;

	do {
		sum += current;
		current = current * (x_squared * 2 * i * (2 * i - 1) * (2 * i - 1)) / (4 * i * i * (2 * i + 1));
		i++;
	} while (myFabs(current) >= EPSILON);

	return x > 0 ? sum : -sum;
}

// acos - https://en.cppreference.com/w/c/numeric/math/acos
double myAcos(double x) {
	if (x == 1)
		return POS_ZERO;
	return PI_OVER_TWO - myAsin(x);
}

// atan - https://en.cppreference.com/w/c/numeric/math/atan
double myAtan(double x) {

}

// atan2 - https://en.cppreference.com/w/c/numeric/math/atan2
double myAtan2(double x, double y) {

}

// sinh - https://en.cppreference.com/w/c/numeric/math/sinh
double mySinh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO || x == POS_INFINITY || x == NEG_INFINITY)
		return x;
	if (_isNan(x))
		return NaN;

	double ex = myExp(x);
	return (ex - 1 / ex) / 2;
}

// cosh - https://en.cppreference.com/w/c/numeric/math/cosh
double myCosh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return 1;
	if (x == POS_INFINITY || x == NEG_INFINITY)
		return POS_INFINITY;
	if (_isNan(x))
		return NaN;

	double ex = myExp(x);
	return (ex + 1 / ex) / 2;
}

// tanh - https://en.cppreference.com/w/c/numeric/math/tanh
double myTanh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (x == POS_INFINITY)		
		return 1;
	if (x == NEG_INFINITY)
		return - 1;
	if (_isNan(x))
		return NaN;

	double ex = myExp(x);
	double oneOverEx = 1 / ex;
	return (ex - oneOverEx) / (ex + oneOverEx);
}

// exp - https://en.cppreference.com/w/c/numeric/math/exp
double myExp(double x) {

}

// log - https://en.cppreference.com/w/c/numeric/math/log
double myLog(double x) {

}

// log10 - https://en.cppreference.com/w/c/numeric/math/log10
double myLog10(double x) {

}

// pow - https://en.cppreference.com/w/c/numeric/math/pow
double myPow(double x, double y) {

}

// sqrt - https://en.cppreference.com/w/c/numeric/math/sqrt
double mySqrt(double x) {

}