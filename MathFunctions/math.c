#define POS_INFINITY *(double*)&POS_INF_ULL
#define NEG_INFINITY *(double*)&NEG_INF_ULL
#define POS_ZERO *(double*)&POS_ZERO_ULL
#define NEG_ZERO *(double*)&NEG_ZERO_ULL
#define NaN *(double*)&NaN_ULL


const double PI = 3.141592653589793238462643383279;
const double E = 2.718281828459045235360287471352;


const unsigned long long POS_ZERO_ULL = 0x0000000000000000ull;
const unsigned long long NEG_ZERO_ULL = 0x8000000000000000ull;
const unsigned long long POS_INF_ULL = 0x7FF0000000000000ull;
const unsigned long long NEG_INF_ULL = 0xFFF0000000000000ull;
const unsigned long long NaN_ULL = 0x7FF8000000000000ull;


static const double EPSILON = 1e-12;
static const int BIAS = 1023;
static const int MANTISSA_BITS = 52;
static const int INF_NAN_EXP = 2047;
static const unsigned long long MANTISSA_MASK = 0x000FFFFFFFFFFFFFull;
static const unsigned long long SIGN_MASK = 0x8000000000000000ull;
static const unsigned long long EXP_MASK = 0x7FF0000000000000ull;
static const unsigned long long FIRST_BIT_OF_MANTISSA_MASK = 0x0008000000000000ull;


static int getIntExponent(double x) {
	return ((*(unsigned long long*) & x) & EXP_MASK) >> MANTISSA_BITS;
}
static unsigned long long getExponentULL(double x) {
	return (*(unsigned long long*) & x) & EXP_MASK;
}
static unsigned long long getMantissa(double x) {
	return (*(unsigned long long*) & x) & MANTISSA_MASK;
}
static unsigned long long getSign(double x) {
	return (*(unsigned long long*) & x) & SIGN_MASK;
}


static unsigned long long getIntegerMantissaPart(double x, int exp) {
	unsigned long long mask = ~(MANTISSA_MASK >> exp) & MANTISSA_MASK;
	return (*(unsigned long long*) & x) & mask;
}
static unsigned long long getFractionalMantissaPart(double x, int exp) {
	unsigned long long mask = MANTISSA_MASK >> exp;
	return (*(unsigned long long*) & x) & mask;
}
static unsigned long long getFirstBitOfFractionalPart(double x, int exp) {
	unsigned long long mask = FIRST_BIT_OF_MANTISSA_MASK >> exp;
	return (*(unsigned long long*) & x) & mask;
}


const int isNan_(unsigned long long mantissa, int exp) {
	return exp == INF_NAN_EXP && mantissa != 0;
}
const int isPosNegZero(unsigned long long mantissa, int exp) {
	return exp == 0 && mantissa == 0;
}
const int isPosNegInfinity(unsigned long long mantissa, int exp) {
	return exp == INF_NAN_EXP && mantissa == 0;
}


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
	if (isNan(x))
		return NaN;
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	int exp = getIntExponent(x) - BIAS;

	if (exp >= 52) return x;
	if (exp <= -1) return (x < 0) ? 0 : 1; // (-1; 1)

	unsigned long long fractional = getFractionalMantissaPart(x, exp);
	if (fractional == 0) return x;

	unsigned long long result = *(unsigned long long*)&x ^ fractional;
	double r = x > 0 ? *(double*)&result + 1 : *(double*)&result;
	return r;
}

// floor - https://en.cppreference.com/w/c/numeric/math/floor
double myFloor(double x) {
	if (isNan(x))
		return NaN;
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;


	int exp = getIntExponent(x) - BIAS;
	if (exp >= 52) return x;
	if (exp <= -1) return (x < 0) ? -1 : 0; // (-1; 1)

	unsigned long long fractional = getFractionalMantissaPart(x, exp);
	if (fractional == 0) return x;

	unsigned long long result = *(unsigned long long*) & x ^ fractional;
	return x > 0 ? *(double*)&result : *(double*)&result - 1;
}

// trunc - https://en.cppreference.com/w/c/numeric/math/trunc
double myTrunc(double x) {
	// myCeil and myFloor implement checks for +-0, +-inf and NaN
	return x < 0 ? myCeil(x) : myFloor(x);
}

// round - https://en.cppreference.com/w/c/numeric/math/round
double myRound(double x) {
	if (isNan(x))
		return NaN;
	if (x == POS_INFINITY || x == NEG_INFINITY || x == POS_ZERO || x == NEG_ZERO)
		return x;

	int exp = getIntExponent(x) - BIAS;
	if (exp >= 52) return x;
	if (exp == -1) return (x > 0) ? 1 : -1; // (-1; -0.5] & [0.5; 1)
	if (exp <= -2) return 0; // (-0.5; 0.5)

	unsigned long long fractional = getFractionalMantissaPart(x, exp);
	if (fractional == 0) return x;

	unsigned long long v = getFirstBitOfFractionalPart(x, exp);
	unsigned long long result = *(unsigned long long*) & x ^ fractional;

	if (v == 0) return *(double*)&result;
	else return (x > 0) ? *(double*)&result + 1 : *(double*)&result - 1;
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
		return POS_ZERO;
	}
	if (isNan(x)) {
		*y = NaN;
		return NaN;
	}

	unsigned long long sign = getSign(x);
	unsigned long long exponent = getExponentULL(x);

	int exp = (exponent >> MANTISSA_BITS) - BIAS;

	if (exp >= 52) {
		*y = x;
		return 0;
	}
	if (exp <= -1) {
		*y = 0;
		return x;
	}

	unsigned long long integer = getIntegerMantissaPart(x, exp);
	unsigned long long i_result = sign | exponent | integer;

	*y = *(double*)&i_result;
	return x - *(double*)&i_result;
}

// frexp - https://en.cppreference.com/w/c/numeric/math/frexp
double myFrexp(double x, int* y) {
	const unsigned long long MASK = 0x3FE0000000000000ull;

	int exp = getIntExponent(x);
	unsigned long long sign = getSign(x);
	unsigned long long mantissa = getMantissa(x);

	if (isPosNegZero(mantissa, exp)) { // +- 0
		*y = 0;
		return x;
	}
	if (isPosNegInfinity(mantissa, exp)) return x; // +- inf
	if (isNan_(mantissa, exp)) return NaN; // NaN
	 
	*y = exp - BIAS + 1;
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

	unsigned long long e = exp + y;
	return sign | (e << MANTISSA_BITS) | getMantissa(x);
}