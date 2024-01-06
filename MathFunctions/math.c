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


int isNan(double x) {
	int exp = getIntExponent(x);
	unsigned long long mantissa = getMantissa(x);
	return exp == INF_NAN_EXP && mantissa != 0;
}
// abs - https://en.cppreference.com/w/c/numeric/math/abs
int myAbs(int x) {
	return x < 0 ? -x : x;
}

// fabs - https://en.cppreference.com/w/c/numeric/math/fabs
double myFabs(double x) {
	if (x == POS_ZERO || x == NEG_ZERO)
		return POS_ZERO;
	if (x == POS_INFINITY || x == NEG_INFINITY)
		return POS_INFINITY;
	if (isNan(x))
		return NaN;

	return x < 0 ? -x : x;
}