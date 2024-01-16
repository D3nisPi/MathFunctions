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

#define pow2(x) (1 - 0.15639e-16 + (0.69314718055995154416 + (0.24022650695869054994 + (0.55504108675285271942e-1 + (0.96181289721472527028e-2 + (0.13333568212100120656e-2 + (0.15403075872034576440e-3 + (0.15265399313676342402e-4 + (0.13003468358428470167e-5 + 0.12113766044841794408e-6 * (x)) * (x)) * (x)) * (x)) * (x)) * (x)) * (x)) * (x)) * (x))
#define ln(u, u2) (u * (2 + u2 * (0.666666666666666 + u2 * (0.4 + u2 * (0.285714285714285 + u2 * (0.222222222222222 + u2 * (0.181818181818181 + u2 * (0.153846153846153 + u2 * 0.133333333333333))))))))


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
	return x <= NEG_ZERO ? -x : x;
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
	if (isNan(x) || isNan(y))
		return NaN;
	if ((x == POS_ZERO || x == NEG_ZERO) && y != 0)
		return x;
	if ((x == POS_INFINITY || x == NEG_INFINITY) && !isNan(y))
		return NaN;
	if ((y == POS_ZERO || y == NEG_ZERO) && !isNan(x))
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
		return NEG_ZERO;
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
	if (isNan(x))
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
	if (x == POS_INFINITY || x == NEG_INFINITY || isNan(x))
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
	if (x == POS_INFINITY || x == NEG_INFINITY || isNan(x))
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
	if (abs_x > 1 || isNan(x))
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
	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (x == POS_INFINITY)
		return PI_OVER_TWO;
	if (x == NEG_INFINITY)
		return -PI_OVER_TWO;
	if (isNan(x))
		return NaN;

	if (x > 1)
		return PI_OVER_TWO - myAtan(1 / x);
	if (x < -1)
		return -PI_OVER_TWO - myAtan(1 / x);

	// Taylor series around 0
	double ax = myFabs(x);
	double sum = 0;
	int i = 1;
	double x2 = x * x;

	double current = ax / (1 + x2);
	do {
		sum += current;
		current *= x2 / (1 + x2) * (2 * i) / (2 * i + 1);
		i++;
	} while (current >= EPSILON);

	return x > 0 ? sum : -sum;
}

// atan2 - https://en.cppreference.com/w/c/numeric/math/atan2
double myAtan2(double y, double x) {
	if (isPosZero(y)) {
		if (x > POS_ZERO || isPosZero(x))
			return POS_ZERO;
		else
			return PI;
	}
	if (isNegZero(y)) {
		if (x > POS_ZERO || isPosZero(x))
			return NEG_ZERO;
		else
			return -PI;
	}
	if (y == POS_INFINITY) {
		if (x == POS_INFINITY)
			return PI_OVER_FOUR;
		else if (x == NEG_INFINITY)
			return THREE_PI_OVER_FOUR;
		else
			return PI_OVER_TWO;
	}
	if (y == NEG_INFINITY) {
		if (x == POS_INFINITY)
			return -PI_OVER_FOUR;
		else if (x == NEG_INFINITY)
			return -THREE_PI_OVER_FOUR;
		else
			return -PI_OVER_TWO;
	}
	if (x == POS_ZERO || x == NEG_ZERO) {
		if (y < 0)
			return -PI_OVER_TWO;
		else if (y > 0)
			return PI_OVER_TWO;
	}
	if (x == NEG_INFINITY) {
		if (y > 0 && y != POS_INFINITY)
			return PI;
		else if (y < 0 && y != NEG_INFINITY)
			return -PI;
	}
	if (x == POS_INFINITY) {
		if (y > 0 && y != POS_INFINITY)
			return POS_ZERO;
		else if (y < 0 && y != NEG_INFINITY)
			return NEG_ZERO;
	}
	if (isNan(x) || isNan(y))
		return NaN;

	if (x < 0 && y < 0) return -PI + myAtan(y / x);
	if (x < 0 && y > 0) return PI + myAtan(y / x);
	return myAtan(y / x);
}

// sinh - https://en.cppreference.com/w/c/numeric/math/sinh
double mySinh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO || x == POS_INFINITY || x == NEG_INFINITY)
		return x;
	if (isNan(x))
		return NaN;

	const double LN_ONE_OVER_TWO = -0.693147180559945286226763982995;
	
	// e^x / 2 = e^(x + ln(0.5))
	// e^(-x) = 1 / e^(x + ln(0.5)) * 2ln(0.5) = 0.25 * e^(x + ln(0.5))
	double ex = myExp(x + LN_ONE_OVER_TWO);
	double res = ex - 0.25 / ex ;
	return res;
}

// cosh - https://en.cppreference.com/w/c/numeric/math/cosh
double myCosh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return 1;
	if (x == POS_INFINITY || x == NEG_INFINITY)
		return POS_INFINITY;
	if (isNan(x))
		return NaN;

	const double LN_ONE_OVER_TWO = -0.693147180559945286226763982995;

	// e^x / 2 = e^(x + ln(0.5))
	// e^(-x) = 1 / e^(x + ln(0.5)) * 2ln(0.5) = 0.25 * e^(x + ln(0.5))
	double ex = myExp(x + LN_ONE_OVER_TWO);
	double res = ex + 0.25 / ex;
	return res;
}

// tanh - https://en.cppreference.com/w/c/numeric/math/tanh
double myTanh(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return x;
	if (x == POS_INFINITY)		
		return 1;
	if (x == NEG_INFINITY)
		return - 1;
	if (isNan(x))
		return NaN;

	double ex = myExp(x);
	double oneOverEx = 1 / ex;
	return (ex - oneOverEx) / (ex + oneOverEx);
}

// exp - https://en.cppreference.com/w/c/numeric/math/exp
double myExp(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return 1;
	if (x == NEG_INFINITY)
		return POS_ZERO;	
	if (x == POS_INFINITY)
		return POS_INFINITY;
	if (isNan(x))
		return NaN;

	const double SQRT_OF_TWO = 1.414213562373095145474621858739;
	const double LOG2_E = 1.442695040888963387004650940071;

	// e^x = 2 ^ (x * log_2(e))
	double y = x * LOG2_E;

	if (y <= -1074) return POS_ZERO;
	if (y >= 1024) return POS_INFINITY;

	double integer;
	double fractional;

	// modf
	unsigned long long sign = getSign(y);
	unsigned long long exponent = getExponent_ULL(y);
	unsigned long long mantissa = getMantissa(y);

	int exp = (exponent >> MANTISSA_BITS) - BIAS;

	if (exp <= -1) {
		integer = 0;
		fractional = y;
	}
	else {
		unsigned long long i_result = sign | exponent | (mantissa & (~(MANTISSA_MASK >> exp) & MANTISSA_MASK));
		integer = *(double*)&i_result;
		fractional = y - integer;
	}


	// fractional part should be non-negative
	if (fractional < 0) {
		integer--;
		fractional++;
	}

	double fracPowered = fractional <= 0.5 ? pow2(fractional) : SQRT_OF_TWO * pow2(fractional - 0.5);
	unsigned long long intPowered;

	// denormalize the number if underflow occurs
	if (integer <= -1023) intPowered = 0x0010000000000000ull >> (-(long long)integer - 1022);
	else intPowered = ((long long)integer + 1023) << 52;

	return  *(double*)&intPowered * fracPowered;
}

// log - https://en.cppreference.com/w/c/numeric/math/log
double myLog(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return NEG_INFINITY;
	if (x == 1)
		return POS_ZERO;
	if (x < 0)
		return NaN;
	if (x == POS_INFINITY)
		return POS_INFINITY;
	if (isNan(x))
		return NaN;

	const double LN_2 = 0.693147180559945286226763982995;
	const double ONE_OVER_QBRT_E = 0.778800783071404878477039801510;
	const unsigned long long MASK = 0x3FF0000000000000ull;

	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x) | MASK; // [1; 2)
	double m = (*(double*)&mantissa) * ONE_OVER_QBRT_E; // [0.78; 1.56)

	double u = (m - 1) / (m + 1);
	double u2 = u * u;
	return ln(u, u2) + 0.25 + exp * LN_2;
}

// log10 - https://en.cppreference.com/w/c/numeric/math/log10
double myLog10(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return NEG_INFINITY;
	if (x == 1)
		return POS_ZERO;
	if (x < 0)
		return NaN;
	if (x == POS_INFINITY)
		return POS_INFINITY;
	if (isNan(x))
		return NaN;

	// log10(x) = ln(x) / ln(10) = ln(x) * log10(e)

	const double LOG10_E = 0.434294481903251816667932416749;
	const double LN_2 = 0.693147180559945286226763982995;
	const double ONE_OVER_QBRT_E = 0.778800783071404878477039801510;
	const unsigned long long MASK = 0x3FF0000000000000ull;

	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x) | MASK; // [1; 2)
	double m = (*(double*)&mantissa) * ONE_OVER_QBRT_E; // [0.78; 1.56)

	double u = (m - 1) / (m + 1);
	double u2 = u * u;

	return LOG10_E * (ln(u, u2) + 0.25 + exp * LN_2);
}

// pow - https://en.cppreference.com/w/c/numeric/math/pow
double myPow(double x, double y) {

}

// sqrt - https://en.cppreference.com/w/c/numeric/math/sqrt
double mySqrt(double x) {
	if (x < NEG_ZERO || isNan(x))
		return NaN;
	if (x == POS_ZERO || x == NEG_ZERO || x == POS_INFINITY)
		return x;


	const double SQRT_OF_TWO = 1.414213562373095145474621858739;
	const unsigned long long BIASED_ZERO_EXPONENT = 0x3FF0000000000000ull;

	int exp = getIntExponent(x) - BIAS;
	unsigned long long mantissa = getMantissa(x) | BIASED_ZERO_EXPONENT;
	double result = 1;

	if (exp & 1 != 0) {
		result = SQRT_OF_TWO;
		exp--;
	}

	exp /= 2;

	if (exp != 0) {
		unsigned long long a = ((unsigned long long)(exp + BIAS) << MANTISSA_BITS);
		result *= *(double*)&a;
	}


	double m = *(double*)&mantissa;
	double reciprocalRoot = 1;
	for (int i = 0; i < 6; i++) {
		reciprocalRoot = 0.5 * reciprocalRoot * (3 - m * reciprocalRoot * reciprocalRoot);
	}

	return result * reciprocalRoot * m;
}