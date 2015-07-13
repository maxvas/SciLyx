/*
 * \file configCompiler.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * This is the compilation configuration file for LyX.
 * It was generated by cmake.
 * You might want to change some of the defaults if something goes wrong
 * during the compilation.
 */

#ifndef _CONFIG_COMPILER_H
#define _CONFIG_COMPILER_H


#cmakedefine HAVE_ISTREAM 1
#cmakedefine HAVE_OSTREAM 1
#cmakedefine HAVE_IOS 1
#cmakedefine HAVE_LOCALE 1

#include "configIncludes.h"

#include "configFunctions.h"

#cmakedefine HAVE_STD_COUNT 1
#cmakedefine HAVE_ASPRINTF 1
#cmakedefine HAVE_WPRINTF 1
#cmakedefine HAVE_SNPRINTF 1
#cmakedefine HAVE_POSIX_PRINTF 1
#cmakedefine HAVE_INTMAX_T 1
#cmakedefine HAVE_INTTYPES_H_WITH_UINTMAX 1
#cmakedefine CXX_GLOBAL_CSTD 1
#cmakedefine HAVE_WPRINTF 1
#cmakedefine HAVE_LONG_DOUBLE 1
#cmakedefine HAVE_LONG_LONG 1
#cmakedefine HAVE_WCHAR_T 1
#cmakedefine HAVE_WINT_T 1
#cmakedefine HAVE_STDINT_H_WITH_UINTMAX 1
#cmakedefine HAVE_LC_MESSAGES 1    
#cmakedefine HAVE_SSTREAM 1
#cmakedefine HAVE_ARGZ_H 1
#cmakedefine HAVE_MAGIC_H 1
#cmakedefine SIZEOF_WCHAR_T_IS_2 1
#cmakedefine SIZEOF_WCHAR_T_IS_4 1

#ifdef SIZEOF_WCHAR_T_IS_2
#  define SIZEOF_WCHAR_T 2
#else
#  ifdef SIZEOF_WCHAR_T_IS_4
#    define SIZEOF_WCHAR_T 4
#  endif
#endif

#cmakedefine GETTEXT_FOUND 1

#cmakedefine HAVE_ALLOCA 1
#cmakedefine HAVE_SYMBOL_ALLOCA 1
#if defined(HAVE_SYMBOL_ALLOCA) && !defined(HAVE_ALLOCA)
#define HAVE_ALLOCA
#endif

#cmakedefine HAVE_ICONV_CONST 1
#ifdef HAVE_ICONV_CONST
#define ICONV_CONST const
#else
#define ICONV_CONST
#endif

#ifdef _MSC_VER
#undef HAVE_OPEN  // use _open instead
#define pid_t int
#define PATH_MAX 512
#endif

#ifdef _WIN32 
#undef HAVE_MKDIR // use _mkdir instead
#endif

#define BOOST_ALL_NO_LIB 1

#if defined(HAVE_OSTREAM) && defined(HAVE_LOCALE) && defined(HAVE_SSTREAM)
#  define USE_BOOST_FORMAT 1
#else
#  define USE_BOOST_FORMAT 0
#endif

#ifdef _DEBUG
#  define ENABLE_ASSERTIONS 1
#endif

#ifndef ENABLE_ASSERTIONS
#  define BOOST_DISABLE_ASSERTS 1
#endif
#define BOOST_ENABLE_ASSERT_HANDLER 1

//#define BOOST_DISABLE_THREADS 1
#define BOOST_NO_WSTRING 1

#ifdef __CYGWIN__
#  define BOOST_POSIX 1
#  define BOOST_POSIX_API 1
#  define BOOST_POSIX_PATH 1
#endif

#if defined(HAVE_WCHAR_T) && SIZEOF_WCHAR_T == 4
#  define USE_WCHAR_T
#endif

#if defined(MAKE_INTL_LIB) && defined(_MSC_VER)
#define __attribute__(x)
#define inline
#define uintmax_t UINT_MAX
#endif

#ifdef _MSC_VER
#ifdef HAVE_CHMOD
#undef HAVE_CHMOD
#endif
#endif

#ifdef HAVE_CHMOD
#define HAVE_MODE_T
#endif

#cmakedefine _GLIBCXX_DEBUG 1
#cmakedefine _GLIBCXX_DEBUG_PEDANTIC 1

#endif
