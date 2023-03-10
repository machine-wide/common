#pragma once
#ifndef DBJ_COMMON_HEADER_
#define DBJ_COMMON_HEADER_
/*
   (c) 2019-2022 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/

   vcruntime.h

   Architectural decision: depend on the platform API, and the platform is Windows 10
   Thus feel free to peruse macros and things from vcruntime.h
*/

// C programer? Stop. You must understand:
// https://man7.org/linux/man-pages/man7/feature_test_macros.7.html

#ifdef __clang__
#pragma clang system_header
#else // ! __clang__
#error please use clang[-cl].exe as packed with Visual Studio
#endif

/// -------------------------------------------------------------------------------
/// stolen from vcruntime.h
#define _DBJ_STRINGIZE_(x) #x
#define _DBJ_STRINGIZE(x) _DBJ_STRINGIZE_(x)

#define _DBJ_WIDE_(s) L##s
#define _DBJ_WIDE(s) _DBJ_WIDE_(s)

#define _DBJ_CONCATENATE_(a, b) a##b
#define _DBJ_CONCATENATE(a, b) _DBJ_CONCATENATE_(a, b)

#define _DBJ_EXPAND_(s) s
#define _DBJ_EXPAND(s) _DBJ_EXPAND_(s)

#undef MIN
#define MIN(a, b) (((b) < (a)) ? (b) : (a))

#define DBJ_CAPI_MAJOR 0
#define DBJ_CAPI_MINOR 1
#define DBJ_CAPI_PATCH 0
// SEMVER + TIMESTAMP
#define DBJ_VERSION_(M, R, P) #M "." #R "." #P " [" __DATE__ "] "
#define DBJ_CAPI_VERSION_(M, R, P) DBJ_VERSION_(M, R, P)
#define DBJ_CAPI_VERSION() DBJ_CAPI_VERSION_(DBJ_CAPI_MAJOR, DBJ_CAPI_MINOR, DBJ_CAPI_PATCH)

#ifdef __cplusplus
#else // ! __cplusplus

#if __STDC_VERSION__ < 201112L
#error __STDC_VERSION__ has to exist and it has to be at least 201112L, aka C11
#endif

#ifdef __STDC_NO_VLA__
#error Sorry this code requires VL Types
#endif

// NOTE: C++ standard does not allow re-defining the name static_assert
// it should be part of assert.h by C11 std or better
// #ifndef static_assert
// #define static_assert _Static_assert
// #endif // static_assert

#endif // ! __cplusplus

#undef DBJ_EXTERN_C_BEGIN
#undef DBJ_EXTERN_C_END

#ifdef __cplusplus
#define DBJ_EXTERN_C_BEGIN \
  extern "C"               \
  {
#define DBJ_EXTERN_C_END }
#else // ! __cplusplus
#define DBJ_EXTERN_C_BEGIN
#define DBJ_EXTERN_C_END
#endif // !__cplusplus

/*
-----------------------------------------------------------------------------------------
NOTE: C++ compiler will not magically turn into C compiler inside extern "C" { } blocks
thus C header only libs in C++ projects might not compile always
*/
DBJ_EXTERN_C_BEGIN

// requires non void return type
#if defined(__clang__)
#define DBJ_PURE_FUNCTION __attribute__((const))
#else
#define DBJ_PURE_FUNCTION
#endif

/*
*
* POSIX is not deprecated ;)
*
 ... warning C4996 : 'strdup' : The POSIX name for this item is deprecated.Instead,
   use the ISO Cand C++ conformant name : _strdup.See online help for details.
   Thus:
   /wd4996
   must be added as a compiler CLI switch
*/
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
// please STOP here; you must understand:
// https://man7.org/linux/man-pages/man7/feature_test_macros.7.html
#define _POSIX_C_SOURCE 200809L
#endif

#define __STDC_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1

// this is comfortable for users but not 100% a good practice
// never include more than you need
#include <assert.h> // NDEBUG used here
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h> /* MSFT UCRT only */
#include <stddef.h> /* size_t */
#include <errno.h>

#undef DBJ_ASSERT
// this required MSVC dll's
// #define DBJ_ASSERT _ASSERTE
// #include <crtdbg.h>
// so we shall simply do this
#define DBJ_ASSERT assert

/// -------------------------------------------------------------------------------
/// https://stackoverflow.com/a/29253284/10870835

// #if (!defined(_DEBUG)) && (!defined(NDEBUG))
// #error NDEBUG *is* a standard macro and has to exist in RELEASE builds
// #endif

// https://stackoverflow.com/a/19453814/10870835
// it raises an error when the argument is not an array (or a pointer)
// error: subscripted value is neither array nor pointer nor vector
#undef DBJ_IS_INDEXABLE
#define DBJ_IS_INDEXABLE(arg) (sizeof(arg[0]))

// The legend says this is from some GOOGLE project, perhaps called "Chromium"?
// basically use any macro you fancy, as long as it works better
// in any case COUNT_OF should not compile
// id A and B are not array literals
#undef DBJ_COUNT_OF
#define DBJ_COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#undef DBJ_EMPTY_STMNT
#define DBJ_EMPTY_STMNT \
  do                    \
  {                     \
  } while (0)

#undef DBJ_REQUIRE
// DBJ_REQUIRE(n, "Argument n is required not to be null");
#define DBJ_REQUIRE(expression, message) _ASSERTE((expression) && (message))

/// -------------------------------------------------------------------------------
/// be advised: static_assert is C11 macro
/// https://en.cppreference.com/w/c/error/static_assert
#undef DBJ_UNUSED
/// #define DBJ_UNUSED(...) static_assert((__VA_ARGS__, true), #__VA_ARGS__)
#define DBJ_UNUSED(...) DBJ_GULP(__VA_ARGS__)

// genius?
#undef DBJ_GULP
#define DBJ_GULP(...) (void)(0)

/// -------------------------------------------------------------------------------

#ifdef _MSVC_LANG
// https://developercommunity.visualstudio.com/content/problem/195665/-line-cannot-be-used-as-an-argument-for-.html
#define DBJ_CONSTEXPR_LINE long(_DBJ_CONCATENATE(__LINE__, U))
#else
#define DBJ_CONSTEXPR_LINE __LINE__
#endif

/*
we use the macros bellow to create ever needed location info always
associated with the offending expression
timestamp included
*/
#undef DBJ_FILE_LINE
#define DBJ_FILE_LINE __FILE__ "(" _CRT_STRINGIZE(__LINE__) ")"

#undef DBJ_FILE_LINE_TSTAMP
#define DBJ_FILE_LINE_TSTAMP __FILE__ "(" _CRT_STRINGIZE(__LINE__) ")[" __TIMESTAMP__ "] "

#undef DBJ_FLT_PROMPT
#define DBJ_FLT_PROMPT(x) DBJ_FILE_LINE_TSTAMP _CRT_STRINGIZE(x)

/* will not compile if MSG_ is not string literal */
#undef DBJ_ERR_PROMPT
#define DBJ_ERR_PROMPT(MSG_) DBJ_FILE_LINE_TSTAMP MSG_

/*
  there is no `repeat` in C or C++

    DBJ_REPEAT(50){ printf("\n%d", dbj_repeat_counter_ ); }
*/
#undef DBJ_REPEAT
#define DBJ_REPEAT(N) for (size_t dbj_repeat_counter_ = 0; dbj_repeat_counter_ < (size_t)(N); ++dbj_repeat_counter_)

///	-----------------------------------------------------------------------------------------
// WARNING: looks like a hack but it is not
// repeats char without a loop
// requires no free
// also alloca() should work on every compiler
#define DBJ_CHAR_LINE(CHAR_, LEN_) (assert(LEN_ <= 0xFF), \
                                    (char const *const)memset(memset(alloca(LEN_ + 1), 0, LEN_ + 1), CHAR_, LEN_))

// wchar_t version is a bit more involved
// because size of wide char is not 1
enum
{
  DBJ_WSZ = sizeof(wchar_t)
};

#define DBJ_WCHAR_LINE(CHAR_, LEN_) (assert(LEN_ <= 0xFF), \
                                     (wchar_t const *const)wmemset(wmemset(alloca((DBJ_WSZ * LEN_) + DBJ_WSZ), L'\0', LEN_ + 1), CHAR_, LEN_))

///	-----------------------------------------------------------------------------------------
/// compile time extremely precise PI approximation
#define DBJ_PI 355 / 113

// the fallacy of the zstring leads to this
// we have no pointer and size, just the possibility of
// existence of '\0'
#define dbjcapi_is_empty(text) (text == 0 || *text == '\0')

#define dbjcapi_wis_empty(text) (text == 0 || *text == L'\0')

// void __fastfail(unsigned int code);
// FAST_FAIL_<description> symbolic constant from winnt.h or wdm.h that indicates the reason for process termination.
// #include <winnt.h>
#undef DBJ_FAST_FAIL
#ifdef _WIN32
void __fastfail(unsigned int);
#define DBJ_FAST_FAIL __fastfail(7)
#else
#define DBJ_FAST_FAIL exit(-1)
#endif

// https://nullprogram.com/blog/2021/07/30/
static inline char *dbj_strdup_(const char *s)
{
  const size_t len = strlen(s) + 1;
  char *c = calloc(len, sizeof(char));
  if (c)
  {
    memcpy(c, s, len); // GOOD
  }
  return c;
}

// AFAIK above is pure C ?
DBJ_EXTERN_C_END

#endif // DBJ_COMMON_HEADER_
