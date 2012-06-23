#ifndef DEV_MATH_H
#define DEV_MATH_H

#include <math.h>

namespace dev
{
  #define PI      3.14159265358979323846f
  #define PI_2    1.57079632679489661923f
  #define PI_4    0.78539816339744830962f
  #define Radian  57.2957795130823208768f
  #define Degree  0.01745329251994329577f

  #define ToRadian( degree ) ((degree) * (0.01745329251994329577f))
  #define ToDegree( radian ) ((radian) * (57.2957795130823208768f))

  #define DBL_DIG         15                      /* # of decimal digits of precision */
  #define DBL_EPSILON     2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */
  #define DBL_MANT_DIG    53                      /* # of bits in mantissa */
  #define DBL_MAX         1.7976931348623158e+308 /* max value */
  #define DBL_MAX_10_EXP  308                     /* max decimal exponent */
  #define DBL_MAX_EXP     1024                    /* max binary exponent */
  #define DBL_MIN         2.2250738585072014e-308 /* min positive value */
  #define DBL_MIN_10_EXP  (-307)                  /* min decimal exponent */
  #define DBL_MIN_EXP     (-1021)                 /* min binary exponent */
  #define _DBL_RADIX      2                       /* exponent radix */
  #define _DBL_ROUNDS     1                       /* addition rounding: near */

  #define FLT_DIG         6                       /* # of decimal digits of precision */
  #define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
  #define FLT_GUARD       0
  #define FLT_MANT_DIG    24                      /* # of bits in mantissa */
  #define FLT_MAX         3.402823466e+38F        /* max value */
  #define FLT_MAX_10_EXP  38                      /* max decimal exponent */
  #define FLT_MAX_EXP     128                     /* max binary exponent */
  #define FLT_MIN         1.175494351e-38F        /* min positive value */
  #define FLT_MIN_10_EXP  (-37)                   /* min decimal exponent */
  #define FLT_MIN_EXP     (-125)                  /* min binary exponent */
  #define FLT_NORMALIZE   0
  #define FLT_RADIX       2                       /* exponent radix */
  #define FLT_ROUNDS      1                       /* addition rounding: near */

  #define LDBL_DIG        DBL_DIG                 /* # of decimal digits of precision */
  #define LDBL_EPSILON    DBL_EPSILON             /* smallest such that 1.0+LDBL_EPSILON != 1.0 */
  #define LDBL_MANT_DIG   DBL_MANT_DIG            /* # of bits in mantissa */
  #define LDBL_MAX        DBL_MAX                 /* max value */
  #define LDBL_MAX_10_EXP DBL_MAX_10_EXP          /* max decimal exponent */
  #define LDBL_MAX_EXP    DBL_MAX_EXP             /* max binary exponent */
  #define LDBL_MIN        DBL_MIN                 /* min positive value */
  #define LDBL_MIN_10_EXP DBL_MIN_10_EXP          /* min decimal exponent */
  #define LDBL_MIN_EXP    DBL_MIN_EXP             /* min binary exponent */
  #define _LDBL_RADIX     DBL_RADIX               /* exponent radix */
  #define _LDBL_ROUNDS    DBL_ROUNDS              /* addition rounding: near */
}

#endif