// -*- C++ -*-
/***************************************************************************
 *
 * cstdint - definition of integer types [tr.c99.cstdint]
 *
 * $Id: _cstdint.h 661842 2008-05-30 20:43:11Z vitek $
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 2006 Rogue Wave Software.
 * 
 **************************************************************************/

#ifndef _RWSTD_CSTDINT_H_INCLUDED
#define _RWSTD_CSTDINT_H_INCLUDED


#include <rw/_defs.h>


_RWSTD_NAMESPACE (std) {

#ifndef _RWSTD_BNO_TWOS_COMPLEMENT
#  define _RWSTD_T_MIN(tmax)   (-(tmax) - 1)
#else
#  define _RWSTD_T_MIN(tmax)   -(tmax)
#endif


/*** int8_t ***************************************************************/

#ifdef _RWSTD_INT8_T

// optional exact width types
typedef _RWSTD_INT8_T       int8_t;
typedef _RWSTD_UINT8_T      uint8_t;

#  define INT8_MAX          127
#  define UINT8_MAX         255U
#  define INT_LEAST8_MAX    INT8_MAX
#  define UINT_LEAST8_MAX   UINT8_MAX
#  define INT_FAST8_MAX     INT8_MAX
#  define UINT_FAST8_MAX    UINT8_MAX

#elif defined (_RWSTD_INT16_T)

#  define INT_LEAST8_MAX    32767
#  define UINT_LEAST8_MAX   65535U
#  define INT_FAST8_MAX     32767
#  define UINT_FAST8_MAX    65535U

#elif defined (_RWSTD_INT32_T)

#  if 4 == _RWSTD_INT_SIZE
#    define INT_LEAST8_MAX    _RWSTD_INT_MAX
#    define UINT_LEAST8_MAX   _RWSTD_UINT_MAX
#    define INT_FAST8_MAX     _RWSTD_INT_MAX
#    define UINT_FAST8_MAX    _RWSTD_UINT_MAX
#  elif 4 == _RWSTD_LONG_SIZE
#    define INT_LEAST8_MAX    _RWSTD_LONG_MAX
#    define UINT_LEAST8_MAX   _RWSTD_ULONG_MAX
#    define INT_FAST8_MAX     _RWSTD_LONG_MAX
#    define UINT_FAST8_MAX    _RWSTD_ULONG_MAX
#  endif

#elif defined (_RWSTD_INT64_T)

#  if 8 == _RWSTD_LONG_SIZE
#    define INT_LEAST8_MAX    _RWSTD_LONG_MAX
#    define UINT_LEAST8_MAX   _RWSTD_ULONG_MAX
#    define INT_FAST8_MAX     _RWSTD_LONG_MAX
#    define UINT_FAST8_MAX    _RWSTD_ULONG_MAX
#  elif 8 == _RWSTD_LLONG_SIZE
#    define INT_LEAST8_MAX    _RWSTD_LLONG_MAX
#    define UINT_LEAST8_MAX   _RWSTD_ULLONG_MAX
#    define INT_FAST8_MAX     _RWSTD_LLONG_MAX
#    define UINT_FAST8_MAX    _RWSTD_ULLONG_MAX
#  endif

#else   // fallback on int

#  define INT_LEAST8_MAX    _RWSTD_INT_MAX
#  define UINT_LEAST8_MAX   _RWSTD_UINT_MAX
#  define INT_FAST8_MAX     _RWSTD_INT_MAX
#  define UINT_FAST8_MAX    _RWSTD_UINT_MAX

#endif   // _RWSTD_INT{8,16,32,64}_T


/*** int16_t **************************************************************/

#ifdef _RWSTD_INT16_T

// optional exact width types
typedef _RWSTD_INT16_T       int16_t;
typedef _RWSTD_UINT16_T      uint16_t;

#  define INT16_MAX          32767
#  define UINT16_MAX         65535U

#  define INT_LEAST16_MAX    INT16_MAX
#  define UINT_LEAST16_MAX   UINT16_MAX
#  define INT_FAST16_MAX     INT16_MAX
#  define UINT_FAST16_MAX    UINT16_MAX

#elif defined (_RWSTD_INT32_T)

#  if 4 == _RWSTD_INT_SIZE
#    define INT_LEAST16_MAX    _RWSTD_INT_MAX
#    define UINT_LEAST16_MAX   _RWSTD_UINT_MAX
#    define INT_FAST16_MAX     _RWSTD_INT_MAX
#    define UINT_FAST16_MAX    _RWSTD_UINT_MAX
#  elif 4 == _RWSTD_LONG_SIZE
#    define INT_LEAST16_MAX    _RWSTD_LONG_MAX
#    define UINT_LEAST16_MAX   _RWSTD_ULONG_MAX
#    define INT_FAST16_MAX     _RWSTD_LONG_MAX
#    define UINT_FAST16_MAX    _RWSTD_ULONG_MAX
#  endif

#elif defined (_RWSTD_INT64_T)

#  if 8 == _RWSTD_LONG_SIZE
#    define INT_LEAST16_MAX    _RWSTD_LONG_MAX
#    define UINT_LEAST16_MAX   _RWSTD_ULONG_MAX
#    define INT_FAST16_MAX     _RWSTD_LONG_MAX
#    define UINT_FAST16_MAX    _RWSTD_ULONG_MAX
#  elif 8 == _RWSTD_LLONG_SIZE
#    define INT_LEAST16_MAX    _RWSTD_LLONG_MAX
#    define UINT_LEAST16_MAX   _RWSTD_ULLONG_MAX
#    define INT_FAST16_MAX     _RWSTD_LLONG_MAX
#    define UINT_FAST16_MAX    _RWSTD_ULLONG_MAX
#  endif

#else   // fallback on int

#  define INT_LEAST16_MAX    _RWSTD_INT_MAX
#  define UINT_LEAST16_MAX   _RWSTD_UINT_MAX
#  define INT_FAST16_MAX     _RWSTD_INT_MAX
#  define UINT_FAST16_MAX    _RWSTD_UINT_MAX

#endif   // _RWSTD_INT{16,32,64}_T


/*** int32_t **************************************************************/

#ifdef _RWSTD_INT32_T

// optional exact width types
typedef _RWSTD_INT32_T       int32_t;
typedef _RWSTD_UINT32_T      uint32_t;

#  if 4 == _RWSTD_INT_SIZE
#    define INT32_MAX          _RWSTD_INT_MAX
#    define UINT32_MAX         _RWSTD_UINT_MAX
#  elif 4 == _RWSTD_LONG_SIZE
#    define INT32_MAX          _RWSTD_LONG_MAX
#    define UINT32_MAX         _RWSTD_LONG_MAX
#  endif

#  define INT_LEAST32_MAX      INT32_MAX
#  define UINT_LEAST32_MAX     UINT32_MAX
#  define INT_FAST32_MAX       INT32_MAX
#  define UINT_FAST32_MAX      UINT32_MAX

#elif defined (_RWSTD_INT64_T)

#  if 8 == _RWSTD_LONG_SIZE
#    define INT_LEAST32_MAX    _RWSTD_LONG_MAX
#    define UINT_LEAST32_MAX   _RWSTD_ULONG_MAX
#    define INT_FAST32_MAX     _RWSTD_LONG_MAX
#    define UINT_FAST32_MAX    _RWSTD_ULONG_MAX
#  elif 8 == _RWSTD_LLONG_SIZE
#    define INT_LEAST32_MAX    _RWSTD_LLONG_MAX
#    define UINT_LEAST32_MAX   _RWSTD_ULLONG_MAX
#    define INT_FAST32_MAX     _RWSTD_LLONG_MAX
#    define UINT_FAST32_MAX    _RWSTD_ULLONG_MAX
#  endif

#else   // fallback on int

#  define INT32_MAX          _RWSTD_INT_MAX
#  define UINT32_MAX         _RWSTD_UINT_MAX
#  define INT_LEAST32_MAX    _RWSTD_INT_MAX
#  define UINT_LEAST32_MAX   _RWSTD_UINT_MAX
#  define INT_FAST32_MAX     _RWSTD_INT_MAX
#  define UINT_FAST32_MAX    _RWSTD_UINT_MAX

#endif   // _RWSTD_INT{32,64}_T


/*** int64_t **************************************************************/

#ifdef _RWSTD_INT64_T

// optional exact width types
typedef _RWSTD_INT64_T   int64_t;
typedef _RWSTD_UINT64_T  uint64_t;

// 7.18.2.5 of C99 requires that intmax_t be at least 64-bits wide
typedef _RWSTD_INT64_T   intmax_t;
typedef _RWSTD_UINT64_T  uintmax_t;

#  if 8 == _RWSTD_LONG_SIZE
#    define INT64_MAX          _RWSTD_LONG_MAX
#    define UINT64_MAX         _RWSTD_ULONG_MAX
#  elif 8 == _RWSTD_LLONG_SIZE
#    define INT64_MAX          _RWSTD_LLONG_MAX
#    define UINT64_MAX         _RWSTD_ULLONG_MAX
#  endif

#  define INT_LEAST64_MAX    INT64_MAX
#  define UINT_LEAST64_MAX   UINT64_MAX
#  define INT_FAST64_MAX     INT64_MAX
#  define UINT_FAST64_MAX    UINT64_MAX

#elif defined (_RWSTD_LONG_LONG)      // fallback on long long

// this possibly violates 7.18.2.5 of C99 which requires that intmax_t
// be at least 64-bits wide
typedef _RWSTD_LONG_LONG             intmax_t;
typedef unsigned _RWSTD_LONG_LONG    uintmax_t;

#else   // fallback on long

// this likely violates 7.18.2.5 of C99 which requires that intmax_t
// be at least 64-bits wide
typedef long           intmax_t;
typedef unsigned long  uintmax_t;

#endif   // _RWSTD_INT{64,32,16}_T

/*** intptr_t *************************************************************/

#if 8 == _RWSTD_PTR_SIZE

typedef _RWSTD_INT64_T    intptr_t;
typedef _RWSTD_UINT64_T   uintptr_t;

#  define INTPTR_MAX      INT64_MAX
#  define UINTPTR_MAX     UINT64_MAX

#elif 4 == _RWSTD_PTR_SIZE

typedef _RWSTD_INT32_T    intptr_t;
typedef _RWSTD_UINT32_T   uintptr_t;

#  define INTPTR_MAX      INT32_MAX
#  define UINTPTR_MAX     UINT32_MAX

#elif 2 == _RWSTD_PTR_SIZE

typedef _RWSTD_INT16_T    intptr_t;
typedef _RWSTD_UINT16_T   uintptr_t;

#  define INTPTR_MAX      INT16_MAX
#  define UINTPTR_MAX     UINT16_MAX

#else   // fallback on long

typedef long              intptr_t;
typedef unsigned long     uintptr_t;

#  define INTPTR_MAX      _RWSTD_LONG_MAX
#  define UINTPTR_MAX     _RWSTD_LONG_MAX

#endif   // {8,16,32,64} == _RWSTD_PTR_SIZE

/*** least and fast types *************************************************/

typedef _RWSTD_INT_LEAST8_T       int_least8_t;
typedef _RWSTD_UINT_LEAST8_T      uint_least8_t;
typedef _RWSTD_INT_LEAST8_T       int_fast8_t;
typedef _RWSTD_UINT_LEAST8_T      uint_fast8_t;

typedef _RWSTD_INT_LEAST16_T      int_least16_t;
typedef _RWSTD_UINT_LEAST16_T     uint_least16_t;
typedef _RWSTD_INT_LEAST16_T      int_fast16_t;
typedef _RWSTD_UINT_LEAST16_T     uint_fast16_t;

typedef _RWSTD_INT_LEAST32_T      int_least32_t;
typedef _RWSTD_UINT_LEAST32_T     uint_least32_t;
typedef _RWSTD_INT_LEAST32_T      int_fast32_t;
typedef _RWSTD_UINT_LEAST32_T     uint_fast32_t;

#ifdef _RWSTD_INT_LEAST64_T

typedef _RWSTD_INT_LEAST64_T      int_least64_t;
typedef _RWSTD_UINT_LEAST64_T     uint_least64_t;
typedef _RWSTD_INT_LEAST64_T      int_fast64_t;
typedef _RWSTD_UINT_LEAST64_T     uint_fast64_t;

#endif   // _RWSTD_INT_LEAST64_T

#ifdef _RWSTD_INT_LEAST128_T

typedef _RWSTD_INT_LEAST128_T     int_least128_t;
typedef _RWSTD_UINT_LEAST128_T    uint_least128_t;
typedef _RWSTD_INT_LEAST128_T     int_fast128_t;
typedef _RWSTD_UINT_LEAST128_T    uint_fast128_t;

#endif   // _RWSTD_INT_LEAST128_T

/*** other constants ******************************************************/

#define PTRDIFF_MIN         _RWSTD_PTRDIFF_MIN
#define PTRDIFF_MAX         _RWSTD_PTRDIFF_MAX

#define SIZE_MAX            _RWSTD_SIZE_MAX

#define WCHAR_MIN           _RWSTD_WCHAR_MIN
#define WCHAR_MAX           _RWSTD_WCHAR_MAX

#define WINT_MIN            _RWSTD_WINT_MIN
#define WINT_MAX            _RWSTD_WINT_MAX

#define SIG_ATOMIC_MIN      _RWSTD_SIG_ATOMIC_MIN
#define SIG_ATOMIC_MAX      _RWSTD_SIG_ATOMIC_MAX

/*** computed _MIN constants **********************************************/

#define INT8_MIN            _RWSTD_T_MIN (INT8_MAX)
#define INT16_MIN           _RWSTD_T_MIN (INT16_MAX)
#define INT32_MIN           _RWSTD_T_MIN (INT32_MAX)
#define INT64_MIN           _RWSTD_T_MIN (INT64_MAX)

#define INT_LEAST8_MIN      _RWSTD_T_MIN (INT_LEAST8_MAX)
#define INT_LEAST16_MIN     _RWSTD_T_MIN (INT_LEAST16_MAX)
#define INT_LEAST32_MIN     _RWSTD_T_MIN (INT_LEAST32_MAX)
#define INT_LEAST64_MIN     _RWSTD_T_MIN (INT_LEAST64_MAX)

#define INT_FAST8_MIN       _RWSTD_T_MIN (INT_FAST8_MAX)
#define INT_FAST16_MIN      _RWSTD_T_MIN (INT_FAST16_MAX)
#define INT_FAST32_MIN      _RWSTD_T_MIN (INT_FAST32_MAX)
#define INT_FAST64_MIN      _RWSTD_T_MIN (INT_FAST64_MAX)

}   // namespace std


#endif   // _RWSTD_CSTDINT_H_INCLUDED
