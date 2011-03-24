/*
  Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

  This file is part of the Holos Library.
  http://holos.googlecode.com

  the Holos Library is free software: you can redistribute it and/or modify
  it under the terms of the Holos Library License, either version 1.0
  of the License, or (at your option) any later version.

  the Holos Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See LICENSE_HOLOS for more details.

  You should have received a copy of the Holos Library License
  If not, see <http://holos.googlecode.com/>.
*/

#ifndef h_Math_included
#define h_Math_included

/*
********************************************************************************
notes:

code in this header:
  - is non-portable to hardware without FPU  
  - needs user defined types via macro
  - needs a lot of cleanup
  
todo:
  - separation between x86, x64 and other platforms

naming scheme:
  - stdlib replacements  h_Sqrt
  - integer              h_Sqrti   // i=integer, f=float
  - optimizations        h_SqrtF   // F = fast
  - approximations       h_SqrtA   // A = Approximation
  - combinations         h_SqrtFi  // fast integer
                         h_SqrtAi  // approximation, integer
                         
********************************************************************************
*/

#ifdef H_MATH_USE_INLINE
  #define H_MATH_INLINE
#else
  #define H_MATH_INLINE inline
#endif

// constants
#define DENORM      1.0000000E-37F
#define LIN2DB      8.65617024533F
#define DB2LIN      0.11552453009F
#define E           2.71828182845F
#define LOG2E       1.44269504088F
#define LOG10E      0.43429448190F
#define LN2         0.69314718055F
#define LN10        2.30258509299F
#define PI          3.14159265358F
#define PI2         6.28318530717F
#define PI4         12.5663706143F
#define PI_2        1.57079632679F
#define PI_4        0.78539816339F
#define D1_PI       0.31830988618F
#define D2_PI       0.63661977236F
#define SQRT2       1.41421356237F
#define SQRT1_2     0.70710678118F
#define EPSILON     1.19209290E-7F
#define inv360      0.00277777778F
#define inv16       0.06250000000F
#define inv127      0.00787401575F
#define inv256      0.00390625000F

#define H_PI_180    0.0174532925199432957692369076848
#define H_180_PI    57.295779513082320876798154814105

// internal IEEE floating point number classification (fast)
#include "sys/param.h"

#define H_BYTE_ORDER BYTE_ORDER

#define H_FP_INFINITE     0x01
#define H_FP_NAN          0x02
#define H_FP_NORMAL       0x04
#define H_FP_SUBNORMAL    0x08
#define H_FP_ZERO         0x10

union h_IEEE_bits_float
{
  float x;
  struct
  {
    #if H_BYTE_ORDER == LITTLE_ENDIAN
      unsigned int man  : 23;
      unsigned int exp  : 8;
      unsigned int sign : 1;
    #else
      unsigned int sign : 1;
      unsigned int exp  : 8;
      unsigned int man  : 23;
    #endif // BIG_ENDIAN
  } bits;
};

int h_Fpclassifyf(const float x)
{
  union h_IEEE_bits_float u;
  u.x = x;
  if (u.bits.exp == 0)
  {
    if (u.bits.man == 0)
      return (H_FP_ZERO);
    return (H_FP_SUBNORMAL);
  }
  if (u.bits.exp == 255)
  {
    if (u.bits.man == 0)
      return (H_FP_INFINITE);
    return (H_FP_NAN);
  }
  return (H_FP_NORMAL);
}

// isinf_float
H_MATH_INLINE
int h_Isinff(const float x)
{
  union h_IEEE_bits_float u;
  u.x = x;
  return (u.bits.exp == 255 && u.bits.man == 0);
}

// isfinite_float
H_MATH_INLINE
int h_Isfinitef(const float x)
{
  union h_IEEE_bits_float u;
  u.x = x;
  return (u.bits.exp != 255);
}

// isnan_float
H_MATH_INLINE
int h_Isnanf(const float x)
{
  union h_IEEE_bits_float u;
  u.x = x;
  return (u.bits.exp == 255 && u.bits.man != 0);
}

// isnormal_float
H_MATH_INLINE
int h_Isnormalf(const float x)
{
  union h_IEEE_bits_float u;
  u.x = x;
  return (u.bits.exp != 255 && u.bits.exp != 0);
}


// ---------------------------------------------------------------------------

// cabs
#define h_Cabs(a, b) \
  h_Sqrt((a)*(a) + (b)*(b))

// carg
#define h_Carg(a, b) \
  h_Atan2((b), (a))

// degrees to radians
#define h_Radians(x) \
  ((x)*H_PI_180)

// radians to degrees
#define h_Degrees(x) \
  ((x)*H_180_PI)

#define h_Sqr(x) ((x)*(x))
#define h_Cube(x) ((x)*(x)*(x))

// order of polynomial
#define h_PolyOrder(x) \
  (sizeof(x) / sizeof(*(x)) - 1)

// inverse of x
#define h_Inverse(x) (1/(x))

// inverse of x
H_MATH_INLINE
float h_InverseF(const float x)
{
  register union
  {
    unsigned int i;
    float j;
  } u;
  u.j = x;
  u.i = 0x7EEEEEEE - u.i;
  return u.j;
}

// returns the floor of a floating point number
H_MATH_INLINE
float h_Floor(register const float v)
{
  if (v<0.f) return (float)(int)(v - 1.f);
  else return (float)(int)(v);
}

//returns the ceil of a floating point number
H_MATH_INLINE
float h_Ceil(register const float v)
{
  if (v<0.f) return (float)(int)(v);
  else return (float)(int)(v+1.f);
}

// rounds a floating point number
H_MATH_INLINE
float h_Round(register const float v)
{
   if (v<0.f) return (float)(int)(v - 0.5f);
  else return (float)(int)(v + 0.5f);
}

// fast modf() for floating point values. 
// returns the fractional part of a floating point number
// and stores the integer part in the second argument.
H_MATH_INLINE
float h_Modf(register const float value, register float* intpart)
{
  *intpart = (float)(int)value;
  return (value - *intpart);
}

// returns the remainder of the division of two arguments
// result = numerator - (floorf(numerator/denominator) * denominator);
H_MATH_INLINE
float h_Fmod(register const float x, register const float y)
{
  register float value;
  __asm__
  (
    // gets remainder; copy floating point status word into h_ register;
    // copy the ah register into the condition code bits (cc);
    // jump if parity;
    "1: fprem;"    "fstsw %%h_;"   "sahf;"   "jp 1b;"
    : "=t" (value) : "0" (x), "u" (y) : "h_", "cc"
  );
  return value;
}

// returns the absolute value of a floating point number
H_MATH_INLINE
float h_Abs(register const float value)
{
  // alt: fpu fabs is slower
  register union
  {
    int i;
    float j;
  } u;
  u.j = value;
  u.i &= 0x7fffffff;
  return u.j;
}

// returns the negative of a floating point number
H_MATH_INLINE
float h_Neg(register float value)
{
  if (value == 0.f) return 0.f;
  __asm__ ( "xorl $0x80000000, %0;"    : "=r" (value)    : "0" (value) );
  return value;
}

// returns the sign (-1 or 1) of a floating point number
H_MATH_INLINE
float h_Sign(register const float v)
{
  register union
  {
    signed int i;
    float f;
  } u;
  u.f = v;
  return (1 | (u.i >> 31));
}

// returns the sign bit (1 or 0) of a floating point number
H_MATH_INLINE
unsigned int h_SignBit(register const float v)
{
  register union
  {
    signed int i;
    float f;
  } u;
  u.f = v;
  return (unsigned int)(-(u.i >> 31));
}

// returns the smaller of two floating point numbers
H_MATH_INLINE
float h_Min(register const float a, register const float b)
{
  return (a < b) ? a : b;
}

// returns the larger of two floating point numbers
H_MATH_INLINE
float h_Max(register const float a, register const float b)
{
  return (a > b) ? a : b;
}

// limits a floating point number to [-limit, limit]
H_MATH_INLINE
float h_Limit(register const float input,
  register const float limit)
{
  register const float _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

// returns the smaller of two integer numbers
H_MATH_INLINE
int h_Mini(register const int a, register const int b)
{
  return (a < b) ? a : b;
}

// returns the larger of two integer numbers
H_MATH_INLINE
int h_Maxi(register const int a, register const int b)
{
  return (a > b) ? a : b;
}

// limits an integer number to [-limit, limit]
H_MATH_INLINE
int h_Limiti(register const int input,
  register const int limit)
{
  register const int _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

// calculates the logarithm base 2 of a floating point number (fpu)
H_MATH_INLINE
float h_Log2(register float value)
{
  __asm__
  (
    "fld1;"   "fxch;"  "fyl2x;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the logarithm base 2 function
// based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
H_MATH_INLINE
float h_Log2F(register const float val)
{
  if (val > 0.f)
  {
    register union
    {
      int i;
      float j;
    } u;
    u.j = val;
    register const int log_2 = ((u.i >> 23) & 255) - 128;
    u.i &= ~(255 << 23);
    u.i += 127 << 23;
    return (0.05f + u.j + (float)log_2);
  }
  else return 0.f;
}


// calculates the natural logarithm (base e) of a floating point number
H_MATH_INLINE
float h_Log(register float value)
{
  __asm__
  (
    "fld %0;"    "fldln2;"    "fxch;"    "fyl2x;"
    : "=t" (value)
    : "0" (value)
  );
  return value;
}

// fast approximation of the natural logarithm function
// based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
H_MATH_INLINE
float h_LogF(register const float &val)
{
  return (h_Log2(val)*0.69314718f);
}

// calculates the logarithm base 10 of a floating point number (fpu)
H_MATH_INLINE
float h_Log10(register float value)
{
  __asm__
  (
    "fldlg2;"    "fxch;"    "fyl2x;"
    : "=t" (value): "0" (value)
  );
  return value;
}

// calculates the logarithm base 10 of a floating point number
H_MATH_INLINE
float h_Log10F(register const float x)
{
  // log10(e) = 0.4342945239647
  // also: log10(x) = log2(x) - ln(x)
  register const float y = (x - 1)/(x + 1);
  register const float y2 = y*y;
  return (2.f*y*(1 + y2*0.3333333333f + y2*y2*0.2f))*0.4342945239647f;
}

// performs fast and accurate powf(float, float) approximation (fpu)
// optimized for fractional exponent.
// for integer exponent use h_Pow()
H_MATH_INLINE
float h_Pow(register const float x, register const float y)
{
  register float value, exponent;
  __asm__
  (
    "fld1;"                       // |
    "fxch;"                       // |
    "fyl2x;"                      // log2(x)
    "fmul %%st(1);"               // y*log2(x)
    "fst %%st(1);"                // |
    "frndint;"                    // int(y*log2(x))
    "fxch;"                       // |
    "fsub %%st(1);"               // fract(y*log2(x))
    "f2xm1;"                      // 2^(fract(y*log2(x))) - 1
    "fld1;"                       // |
    "faddp;"                      // += 1
    "fscale;"                     // x*(2^exp)
    : "=t" (value), "=u" (exponent)   :"0" (x), "1" (y)
  );
  return value;
}

// performs fast pow(float, integer)
H_MATH_INLINE
float h_PowF(register float x, register unsigned long n)
{
  register float res = 1;
  while (n > 0)
  {
    if (n & 1) res *= x;
    n >>= 1;
    x *= x;
  }
  return res;
}

// pproximation of [e] to the power of a number (fpu)
H_MATH_INLINE
float h_Exp(register const float x)
{
  register float value, exponent;
  __asm__
  (
    "fldl2e;"               // e^x = 2^(x*log2(e))
    "fmul %%st(1);"         // x * log2(e)"
    "fst %%st(1);"          // |
    "frndint;"              // int(x*log2(e))
    "fxch;"                 // |
    "fsub %%st(1);"         // fract(x*log2(e))
    "f2xm1;"                // 2^(fract(x*log2(e)))
    "fld1;"                 // |
    "faddp;"                // += 1
    "fscale;"               // x*(2^exp)
    : "=t" (value), "=u" (exponent) : "0" (x)
  );
  return value;
}

// fast approximation of [e] to the power of a number 
// based on http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf 
// note: original is for double precision (has a double to float cast)
H_MATH_INLINE
float h_ExpF(register const float v)
{
  register union
  {
    double d;
    struct
    {
      #if H_BYTE_ORDER == LITTLE_ENDIAN
        int i, j;
      #else
        int j, i;
      #endif
    } s;
  } u;
  u.s.i = (int)(1512775*(double)v) + 1072632447;
  u.s.j = 0;
  return (float)u.d;
}

// returns the result of x*(2^floor(y))
// ( significand (x) multiplied by the exponent(2^y) )
H_MATH_INLINE
float h_Fscale(register const float x, register const float y)
{
  register float value;
  __asm__
  (
    "fscale;"  : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

// separates the input variable x into significand*(2^exponent)
// uses IEEE logb(x)
H_MATH_INLINE
float h_Fxtract(register float value, register float* _exp)
{
  register float sig;
  __asm__
  (
    "fxtract;"   :"=t"(sig), "=u"(*_exp)   :"0"(value)
  );
  return sig;
}

// rought approximation of a N-th root function (inaccurate)
H_MATH_INLINE
float h_Nrt(register float value,register  long root)
{
  // alt: newton-ramson is still slower than the fpu h_Powf
  // xn = (1/n)*((n-1)*xn + x/pow(xn, n-1));
  __asm__
  (
    "subl $0x3f800000, %0;"    "subl $1, %2;"
    "shrl %b2, %0;"            "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value), "c" (root)
  );
  return value;
}

// returns the square root of a floating point number (fpu)
H_MATH_INLINE
float h_Sqrt(register float value)
{
  __asm__
  (
    "fsqrt;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the squre root function 
// based on: http://www.azillionmonkeys.com/qed/sqroot.html
H_MATH_INLINE
float h_SqrtF(register const float x)
{
  register const float halfx = x*0.5;
  register union
  {
    int i;
    float j;
  } u;
  u.j = x;
  u.i = (0xbe6ec85f - u.i) >> 1;   // good initial guess
  return x*u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

// returns the invert squre root of a floating point number (fpu)
H_MATH_INLINE
float h_InvSqrt(register float value)
{
  __asm__
  (
    "fsqrt;"  "fld1;"   "fdivp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the invert squre root function
H_MATH_INLINE
float h_InvSqrtF(register const float x)
{
  // register const float halfx = 0.5f*x;
  register union
  {
    float j;
    int i;
  } u;
  u.j = x;
  u.i = 0x5f3759df - (u.i >> 1); // good initial guess
  // add one more newton iteration
  // return u.j*(1.5f - u.j*u.j*halfx) + 0.001f;
  return u.j;
}

// calculates the sine of a floating point number (fpu)
H_MATH_INLINE
float h_Sin(register float value)
{
  __asm__
  (
    "fsin;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the sine function for range [-pi, pi]
H_MATH_INLINE
float h_SinF(register float x)
{
  x *= (1.2732395447f - 0.4052847345f * h_Abs(x));
  return 0.225f * (x * h_Abs(x) - x) + x;
}

// calculates the cosine of a floating point number (fpu)
H_MATH_INLINE
float h_Cos(register float value)
{
  __asm__
  (
    "fcos;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the cosine function for range [-pi, pi]
H_MATH_INLINE
float h_CosF(register const float x)
{
  register const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

// calculates the tangens of a floating point number (fpu)
H_MATH_INLINE
float h_Tan(register float value)
{
  __asm__
  (
    "fptan;"  "fstp %1;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the tangens function for range [-pi, pi]
H_MATH_INLINE
float h_TanF(register const float x)
{
  register const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x2*(45 - x2));
}

// calculates both the sine and cosine of a floating point number (fpu)
H_MATH_INLINE
void h_SinCos(const float x, float* sin, float* cos)
{
  __asm__
  (
    "fsincos;" : "=t" (*cos), "=u" (*sin) : "0" (x)
  );
}

// calculates the arc-sine of a floating point number (fpu)
H_MATH_INLINE
float h_Asin(register float value)
{
  // asin(x)=atan(sqrt(x*x/(1-x*x)))
  register float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivp;"    "fsqrt;"    "fld1;"    "fpatan;"   "fst %0;"
    : "=t" (value)  : "0" (tmp)
  );
  return value;
}

// fast approximation of the arc-sine function for range [-1, 1]
H_MATH_INLINE
float h_AsinF(register const float x)
{
  return  PI_2 - h_SqrtF(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
          x*(0.0187293 + 0.395*x))));
}

// calculates the arc-cosine of a floating point number (fpu)
H_MATH_INLINE
float h_Acos(register float value)
{
  // acos(x) = atan(sqrt((1-x*x)/(x*x)))
  register float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivrp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=t" (value)    : "0" (tmp)
  );
  return value;
}

// fast approximation of the arc-cosine function for range [-1, 1]
H_MATH_INLINE
float h_AcosF(register const float x)
{
  register const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

// calculates the arc-tangens of a floating point number (fpu)
H_MATH_INLINE
float h_Atan(register float value)
{
  // from partial tangens
  __asm__
  (
    "fld1;"    "fpatan;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

// fast approximation of the arc-tangens function for range [-2, 2]
H_MATH_INLINE
float h_AtanF(register const float x)
{
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

// calculates the angle in radians between the positive x-h_is of a plane
H_MATH_INLINE
float h_Atan2(register const float y, register const float x)
{
  register float value;
  __asm__
  (
    "fpatan;"    "fld %%st(0);"    : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

// calculates the cotangens of a floating point number (fpu)
H_MATH_INLINE
float h_Cotan(register float value)
{
  // cotan(x) = 1/tan(x)
  __asm__
  (
    "fsincos;"    "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// calculates the cosecant of a floating point number (fpu)
H_MATH_INLINE
float h_Csc(register float value)
{
  // csc(x) = 1/sin(x)
  __asm__
  (
    "fsin;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// calculates the secant of a floating point number (fpu)
H_MATH_INLINE
float h_Sec(register float value)
{
  // sec(x) = 1/cos(x)
  __asm__
  (
    "fcos;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// calculates the arccotangent of a floating point number (fpu)
H_MATH_INLINE
float h_Acotan(register float value)
{
  // arccotan(x) = atan(1/x)
  __asm__
  (
    "fld1;"   "fxch;"   "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// calculates the arcsecant of a floating point number (fpu)
H_MATH_INLINE
float h_Asec(register float value)
{
  // asec(x) = atan(sqrt(x*x-1))
  __asm__
  (
  "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fsqrt;"
  "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// calculates the arccosecant of a floating point number (fpu)
H_MATH_INLINE
float h_Acsc(register float value)
{
  // acsc(x) = atan(sqrt(1/(x*x-1)))
  __asm__
  (
    "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fld1;"   "fdivrp;"
    "fsqrt;"    "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

// approximation of the hyperbolic-sine function for range (fpu)
H_MATH_INLINE
float h_Sinh(register const float x)
{
  if (x >= 0.f)
  {
    const float _e = h_Exp(x);
    return (_e - 1.0f/_e)*0.5f;
  }
  else
  {
    const float _eneg = h_Exp(-x);
    return (1.0f/_eneg - _eneg)*0.5f;
  }
}

// fast approximation of the hyperbolic-sine function for range [-3.5, 3.5]
H_MATH_INLINE
float h_SinhF(register const float x)
{
  register const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

// approximation of the hyperbolic-cosine function (fpu)
H_MATH_INLINE
float h_Cosh(register const float x)
{
  register const float _e = h_Exp(h_Abs(x));
  return (_e + 1.0f/_e)*0.5f;
}

// fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]
H_MATH_INLINE
float h_CoshF(register const float x)
{
  register const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

// approximation of the hyperbolic-tangens function for range [-50, 50] (fpu)
H_MATH_INLINE
float h_Tanh(register const float value)
{
  if (value > 50) return 1;
  else if (value < -50) return -1;
  else
  {
    register const float _e = h_Exp(value);
    register const float _er = 1.f/_e;
    return (_e - _er) / (_e + _er);
  }
}

// fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]
H_MATH_INLINE
float h_TanhF(register const float x)
{
  register const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

// calculate the average value of a set of floats
H_MATH_INLINE
float h_Avrg(register const unsigned int n,
  register const float* ar)
{
  register float total = 0;
  register unsigned int i=0;
  while (i<n)
  {
    total += ar[i];
    i++;
  }
  return total/n;
}

// calculate the average value of a set of integers 
H_MATH_INLINE
int h_Avrgi(register const unsigned int n, register const int* ar)
{
  register int total = 0;
  register unsigned int i=0;
  while (i<n)
  {
    total += ar[i];
    i++;
  }
  return total/n;
}

// calculate the RMS of a set (array) of float numbers
H_MATH_INLINE
float h_RMS(register const unsigned int n, register const float* ar)
{
  register float numr = 0;
  register unsigned int i=0;
  while (i<n)
  {
    numr += ar[i]*ar[i];
    i++;
  }
  return h_Sqrt(numr/n);
}

// calculate the RMS of a set (array) of int numbers
H_MATH_INLINE
int h_RMSi(register const unsigned int n, register const int* ar)
{
  register int numr = 0;
  register unsigned int i=0;
  while (i<n)
  {
    numr += ar[i]*ar[i];
    i++;
  }
  return (int)(h_Sqrt((float)(numr/n)));
}

// conversation from bandwidth (octaves) to q factor
H_MATH_INLINE
float h_Octaves2Q(const float n)
{
  const float _pow2n = h_Pow(2.f, n);
  return -h_SqrtF(_pow2n) / (1.f - _pow2n);
}

// conversation from q factor to bandwidth (octaves)
H_MATH_INLINE
float h_Q2Octaves(const float q)
{
  return 1.4426950408889634f * h_SinhF(0.5f * (q));
}

// denormalize input value
H_MATH_INLINE
float h_Denorm(register float n)
{
  register union
  {
    float f;
    unsigned int i;
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

// decibel to linear
H_MATH_INLINE
float h_Lin2Db(const float x)
{
  return LIN2DB*h_LogF(x); 
}

// linear to decibel
H_MATH_INLINE
float h_Db2Lin(const float x)
{
  return h_ExpF(DB2LIN*x); 
}

// sums a set (array) of dBFS values
H_MATH_INLINE
float h_SumDb(unsigned int n, const float* ar)
{
  float sum = 0.f;
  for (unsigned int i=0; i<n; i++) sum += h_Db2Lin(ar[i]);
  return h_LogF(sum)*LIN2DB;
}

#endif /* h_Math_included */
