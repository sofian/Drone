#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <algorithm>

//! Basic definitions

#ifndef HALF_PI
#define HALF_PI 0.5*M_PI
#endif

#ifndef TWICE_PI
#define TWICE_PI 2*M_PI
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Use template MIN/MAX (safer).

#ifdef MIN
#undef MIN
#endif

#ifdef MAX
#undef MAX
#endif

//! Min.
template <typename T>
T MIN(T x, T y)
{ return std::min(x,y); }

//! Max.
template <typename T>
T MAX(T x, T y)
{ return std::max(x,y); }

// Begin LIBGIMP

// LIBGIMP - The GIMP Library
// Copyright (c) 2004 Jean-S�bastien Sen�cal
// Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball

//! Clamp x at lower = l and upper = u.
template <typename T>
inline T CLAMP(T x, T l, T u)
{ return ( x < l ? l : ( x > u ? u : x ) ); }

//! Round.
template <typename T>
inline int ROUND(T x)
{ return (int)(x+0.5); }

//! Square.
template <typename T>
inline T SQR(T x)
{ return (x*x); }

//! Limit a (0->511) int to 255.
inline int MAX255(int a)
{ return (a | ((a & 256) - ((a & 256) >> 8))); }

//! Clamp a int32-range int between 0 and 255 inclusive
#ifndef CLAMP0255
#define CLAMP0255(a)  CLAMP(a,0,255)
#endif


// End LIBGIMP


#define PI_DIV_180 M_PI / 180
inline double DEG2RAD(double d)
{ return (d * PI_DIV_180); }


//! Return whether is outside boundaries (x<l or x>u)
template <typename T>
inline bool IS_OUTSIDE(T x, T l, T u)
{ return ( (x < l) || (x > u) );}

// Fast array operations.

// Additions.

//! Adds #n# elements of #src1# to #src2# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void addVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = *src1++ + *src2++;
}

//! Adds #n# elements of #src# to #dst#.
template <typename T, typename U>
inline void addAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ += *src++;
}

//! Adds #val# to #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void addVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = *src++ + val;
}

//! Adds #val# to #n# elements of #src#.
template <typename T, typename U>
inline void addAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ += val;
}


// Subtractions.

//! Subtracts #n# elements of #src2# from #src1# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void subtractVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = *src1++ - *src2++;
}

//! Subtracts #n# elements of #src# from #dst#.
template <typename T, typename U>
inline void subtractAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ -= *src++;
}

//! Subtracts #val# from #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void subtractVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = *src++ - val;
}

//! Subtracts #val# to #n# elements of #src#.
template <typename T, typename U>
inline void subtractAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ -= val;
}


// Multiplications.

//! Multiplies #n# elements of #src1# to #src2# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void multiplyVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = *src1++ * *src2++;
}

//! Multiplies #n# elements of #dst# by #src#.
template <typename T, typename U>
inline void multiplyAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ *= *src++;
}

//! Subtracts #val# to #n# elements of #src# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void mutliplyVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = *src++ * val;
}

//! Multiplies #val# with #n# elements of #src#.
template <typename T, typename U>
inline void multiplyAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ *= val;
}


// Divisions.

//! Divides #n# elements of #src2# from #src1# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void divideVecVec(T *dst, const U *src1, const V *src2, size_t n)
{
  while (n--)
    *dst++ = *src1++ / *src2++;
}

//! Divides #n# elements of #dst# by #src#.
template <typename T, typename U>
inline void divideAccVecVec(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ /= *src++;
}

//! Divides #n# elements of #src# by #val# and puts the result in #dst#. 
template <typename T, typename U, typename V>
inline void divideVecVal(T *dst, const U *src, const V val, size_t n)
{
  while (n--)
    *dst++ = *src++ / val;
}

//! Divides #n# elements of #src# by #val#.
template <typename T, typename U>
inline void divideAccVecVal(T *src, const U val, size_t n)
{
  while (n--)
    *src++ /= val;
}


// Other utility template functions.

//! Copies #n# values of #src# to #dst#. If both are of the same type, better use memcpy(3).
template <typename T, typename U>
inline void copy(T *dst, const U *src, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(*src++);
}

//! Fills #n# values of #src# with value #val#.
template <typename T, typename U>
inline void fill(T *dst, const U val, size_t n)
{
  while (n--)
    *dst++ = static_cast<T>(val);
}

//! Returns the sum of #n# elements of #src#.
template <typename T>
inline T sum(const T *src, size_t n)
{
  if (!n)
    return 0;
  T ret = *src++;
  while (--n)
    ret += *src++;
  return ret;
}

//! Returns the min element of #n# elements of #src#.
template <typename T>
inline T min(const T *src, size_t n)
{
  if (!n)
    return 0;
  T min = *src++;
  while (--n)
    min = MIN(*src++,min);
  return min;
}

//! Returns the max element of #n# elements of #src#.
template <typename T>
inline T max(const T *src, size_t n)
{
  if (!n)
    return 0;
  T max = *src++;
  while (--n)
    max = MAX(*src++,max);
  return max;
}

//! Fast trigonometric functions approximations

//! Begin WML

// Magic Software, Inc.
// http://www.magic-software.com
// http://www.wild-magic.com
// Copyright (c) 2004.  All Rights Reserved
//
// The Wild Magic Library (WML) source code is supplied under the terms of
// the license agreement http://www.magic-software.com/License/WildMagic.pdf
// and may not be copied or disclosed except in accordance with the terms of
// that agreement.

#define FAST_MATH_PRECISION 1 // XXX to be changed ultimately (how??)


// WARNING : these function are only valid in some interval
// TODO: which interval.. Should be something like -2PI..2PI
inline float fastsin (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = -2.39e-08f;
  ret *= x2;
  ret += 2.7526e-06f;
  ret *= x2;
  ret -= 1.98409e-04f;
  ret *= x2;
  ret += 8.3333315e-03f;
  ret *= x2;
  ret -= 1.666666664e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 7.61e-03f;
  ret *= x2;
  ret -= 1.6605e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#endif
}

inline float fastcos (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = -2.605e-07f;
  ret *= x2;
  ret += 2.47609e-05f;
  ret *= x2;
  ret -= 1.3888397e-03f;
  ret *= x2;
  ret += 4.16666418e-02f;
  ret *= x2;
  ret -= 4.999999963e-01f;
  ret *= x2;
  ret += 1.0f;
  return ret;
#else
  float x2 = x*x;
  float ret = 3.705e-02f;
  ret *= x2;
  ret -= 4.967e-01f;
  ret *= x2;
  ret += 1.0f;
  return ret;
#endif
}

inline float fasttan (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = 9.5168091e-03f;
  ret *= x2;
  ret += 2.900525e-03f;
  ret *= x2;
  ret += 2.45650893e-02f;
  ret *= x2;
  ret += 5.33740603e-02f;
  ret *= x2;
  ret += 1.333923995e-01f;
  ret *= x2;
  ret += 3.333314036e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 2.033e-01f;
  ret *= x2;
  ret += 3.1755e-01f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#endif
}

inline float fastasin (float x)
{
  float xRoot = sqrt(1.0f-x);
  float ret = -0.0187293f;
  ret *= x;
  ret += 0.0742610f;
  ret *= x;
  ret -= 0.2121144f;
  ret *= x;
  ret += 1.5707288f;
  ret = HALF_PI - xRoot*ret;
  return ret;
}

inline float fastacos (float x)
{
  float xRoot = sqrt(1.0f-x);
  float ret = -0.0187293f;
  ret *= x;
  ret += 0.0742610f;
  ret *= x;
  ret -= 0.2121144f;
  ret *= x;
  ret += 1.5707288f;
  ret *= xRoot;
  return ret;
}

inline float fastatan (float x)
{
#if FAST_MATH_PRECISION
  float x2 = x*x;
  float ret = 0.0028662257f;
  ret *= x2;
  ret -= 0.0161657367f;
  ret *= x2;
  ret += 0.0429096138f;
  ret *= x2;
  ret -= 0.0752896400f;
  ret *= x2;
  ret += 0.1065626393f;
  ret *= x2;
  ret -= 0.1420889944f;
  ret *= x2;
  ret += 0.1999355085f;
  ret *= x2;
  ret -= 0.3333314528f;
  ret *= x2;
  ret += 1.0f;
  ret *= x;
  return ret;
#else
  float x2 = x*x;
  float ret = 0.0208351f;
  ret *= x2;
  ret -= 0.085133f;
  ret *= x2;
  ret += 0.180141f;
  ret *= x2;
  ret -= 0.3302995f;
  ret *= x2;
  ret += 0.999866f;
  ret *= x;
  return ret;
#endif
}

//! End WML

//
inline float fastDist(float x, float y)
{
  // well... not that fast yet :)
  return sqrt( x*x + y*y );
}

inline float fastAngle(float x, float y)
{
  // well... not that fast yet :)
  return atan2f(y,x);
}

// warning : theta must be clamped inside some range 0..2PI (see fastcos)
inline float fastPolarToX(float rho, float theta)
{
  return fastcos(theta)*rho;
}

// warning : theta must be clamped inside some range 0..2PI (see fastsin)
inline float fastPolarToY(float rho, float theta)
{
  return fastsin(theta)*rho;
}

//! clamps the value in a mirror fashion :
//! It is like the interval was infinite, but repeating himself 'mirrorly'
//! e.g: MIRROR_CLAMP(x,0,2) for x={-3,-2,-1,0,1,2,3} gives :
//! {1,2,1,0,1,2,1}
//! WARNING : not thread safe because of static variables

template<class T>
T MIRROR_CLAMP(T t,T low,T high)
{
  if(t>=low && t<=high)
    return t;
  else
  {
    static T range,howmuch;
    static int howmuchtimes,flip;
    range=high-low;
    if(t<low)
    {
      howmuch=low-t;
      flip=0;
    }
    else
    {
      howmuch=t-high;
      flip=1;
    }
    howmuchtimes = (int)(howmuch/range);
    if((howmuchtimes+flip)%2)
      return high-howmuch+howmuchtimes*range;
    else
      return low+howmuch-howmuchtimes*range;
  }
}


#endif
