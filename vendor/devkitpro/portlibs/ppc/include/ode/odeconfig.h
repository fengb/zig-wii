/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

#ifndef _ODE_ODECONFIG_H_
#define _ODE_ODECONFIG_H_

/* Pull in the standard headers */
#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <float.h>


#include <ode/precision.h>


#if defined(ODE_DLL) || defined(ODE_LIB)
#define __ODE__
#endif

/* Define a DLL export symbol for those platforms that need it */
#if defined(_MSC_VER) || (defined(__GNUC__) && defined(_WIN32))
  #if defined(ODE_DLL)
    #define ODE_API __declspec(dllexport)
  #else
    #define ODE_API
  #endif
#endif

#if !defined(ODE_API)
  #define ODE_API
#endif

#if defined(_MSC_VER)
#  define ODE_API_DEPRECATED __declspec(deprecated)
#elif defined (__GNUC__) && ( (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)) )
#  define ODE_API_DEPRECATED __attribute__((__deprecated__))
#else
#  define ODE_API_DEPRECATED
#endif

#define ODE_PURE_INLINE static __inline
#define ODE_INLINE __inline

#if defined(__cplusplus)
  #define ODE_EXTERN_C extern "C"
#else
  #define ODE_EXTERN_C
#endif

#if defined(__GNUC__)
#define ODE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define ODE_NORETURN __declspec(noreturn)
#else // #if !defined(_MSC_VER)
#define ODE_NORETURN
#endif // #if !defined(__GNUC__)


/* Well-defined common data types...need to be defined for 64 bit systems */
#if defined(__aarch64__) || defined(__alpha__) || defined(__ppc64__) \
    || defined(__s390__) || defined(__s390x__) || defined(__zarch__) \
    || defined(__mips__) || defined(__powerpc64__) || defined(__riscv) \
    || (defined(__sparc__) && defined(__arch64__))
    #include <stdint.h>
    typedef int64_t         dint64;
    typedef uint64_t        duint64;
    typedef int32_t         dint32;
    typedef uint32_t        duint32;
    typedef int16_t         dint16;
    typedef uint16_t        duint16;
    typedef int8_t          dint8;
    typedef uint8_t         duint8;

    typedef intptr_t        dintptr;
    typedef uintptr_t       duintptr;
    typedef ptrdiff_t       ddiffint;
    typedef size_t          dsizeint;

#elif (defined(_M_IA64) || defined(__ia64__) || defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ILP32__) && !defined(_ILP32)
  #define X86_64_SYSTEM   1
#if defined(_MSC_VER)
  typedef __int64         dint64;
  typedef unsigned __int64 duint64;
#else
#if defined(_LP64) || defined(__LP64__)
typedef long              dint64;
typedef unsigned long     duint64;
#else
  typedef long long       dint64;
  typedef unsigned long long duint64;
#endif
#endif
  typedef int             dint32;
  typedef unsigned int    duint32;
  typedef short           dint16;
  typedef unsigned short  duint16;
  typedef signed char     dint8;
  typedef unsigned char   duint8;

  typedef dint64          dintptr;
  typedef duint64         duintptr;
  typedef dint64          ddiffint;
  typedef duint64         dsizeint;

#else
#if defined(_MSC_VER)
  typedef __int64         dint64;
  typedef unsigned __int64 duint64;
#else
  typedef long long       dint64;
  typedef unsigned long long duint64;
#endif
  typedef int             dint32;
  typedef unsigned int    duint32;
  typedef short           dint16;
  typedef unsigned short  duint16;
  typedef signed char     dint8;
  typedef unsigned char   duint8;

  typedef dint32          dintptr;
  typedef duint32         duintptr;
  typedef dint32          ddiffint;
  typedef duint32         dsizeint;

#endif


/* Define the dInfinity macro */
#ifdef INFINITY
  #ifdef dSINGLE
    #define dInfinity ((float)INFINITY)
  #else
    #define dInfinity ((double)INFINITY)
  #endif
#elif defined(HUGE_VAL)
  #ifdef dSINGLE
    #ifdef HUGE_VALF
      #define dInfinity HUGE_VALF
    #else
      #define dInfinity ((float)HUGE_VAL)
    #endif
  #else
    #define dInfinity HUGE_VAL
  #endif
#else
  #ifdef dSINGLE
    #define dInfinity ((float)(1.0/0.0))
  #else
    #define dInfinity (1.0/0.0)
  #endif
#endif


/* Define the dNaN macro */
#if defined(NAN)
  #define dNaN NAN
#elif defined(__GNUC__)
  #define dNaN ({ union { duint32 m_ui; float m_f; } un; un.m_ui = 0x7FC00000; un.m_f; })
#elif defined(__cplusplus)
  union _dNaNUnion
  {
      _dNaNUnion(): m_ui(0x7FC00000) {}
      duint32 m_ui; 
      float m_f;
  };
  #define dNaN (_dNaNUnion().m_f)
#else
  #ifdef dSINGLE
    #define dNaN ((float)(dInfinity - dInfinity))
  #else
    #define dNaN (dInfinity - dInfinity)
  #endif
#endif


  /* Visual C does not define these functions */
#if defined(_MSC_VER)
  #define _ode_copysignf(x, y) ((float)_copysign(x, y))
  #define _ode_copysign(x, y) _copysign(x, y)
  #define _ode_nextafterf(x, y) _nextafterf(x, y)
  #define _ode_nextafter(x, y) _nextafter(x, y)
  #if !defined(_WIN64) && defined(dSINGLE)
    #define _ODE__NEXTAFTERF_REQUIRED
    ODE_EXTERN_C float _nextafterf(float x, float y);
  #endif
#else
  #define _ode_copysignf(x, y) copysignf(x, y)
  #define _ode_copysign(x, y) copysign(x, y)
  #define _ode_nextafterf(x, y) nextafterf(x, y)
  #define _ode_nextafter(x, y) nextafter(x, y)
#endif


#endif
