/* µnit Testing Framework
 * Copyright (c) 2013-2016 Evan Nemerson <evan@nemerson.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !defined(MUNIT_H)
#define MUNIT_H

#include <stdarg.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__GNUC__)
#  define MUNIT_LIKELY(expr) (__builtin_expect ((expr), 1))
#  define MUNIT_UNLIKELY(expr) (__builtin_expect ((expr), 0))
#  define MUNIT_UNUSED __attribute__((__unused__))
#else
#  define MUNIT_LIKELY(expr) (expr)
#  define MUNIT_UNLIKELY(expr) (expr)
#  define MUNIT_UNUSED
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#  define MUNIT_ARRAY_PARAM(name) name
#else
#  define MUNIT_ARRAY_PARAM(name)
#endif

#if !defined(_WIN32)
#  define MUNIT_SIZE_MODIFIER "z"
#  define MUNIT_CHAR_MODIFIER "hh"
#  define MUNIT_SHORT_MODIFIER "h"
#else
#  if defined(_M_X64) || defined(__amd64__)
#    define MUNIT_SIZE_MODIFIER "I64"
#  else
#    define MUNIT_SIZE_MODIFIER ""
#  endif
#  define MUNIT_CHAR_MODIFIER ""
#  define MUNIT_SHORT_MODIFIER ""
#endif

typedef enum {
  MUNIT_LOG_DEBUG,
  MUNIT_LOG_INFO,
  MUNIT_LOG_WARNING,
  MUNIT_LOG_ERROR
} MunitLogLevel;

#if defined(__GNUC__) && !defined(__MINGW32__)
__attribute__((format (printf, 4, 5)))
#endif
void munit_log_ex(MunitLogLevel level, const char* filename, int line, const char* format, ...);

#define munit_logf(level, format, ...) \
  munit_log_ex(level, __FILE__, __LINE__, format, __VA_ARGS__)

#define munit_log(level, msg) \
  munit_logf(level, "%s", msg)

#define munit_errorf(format, ...) \
  munit_logf(MUNIT_LOG_ERROR, format, __VA_ARGS__)

#define munit_error(msg) \
  munit_errorf("%s", msg)

#define munit_assert(expr) \
  do { \
    if (!MUNIT_LIKELY(expr)) { \
      munit_error("assertion failed: " #expr); \
    } \
  } while (0)
#define munit_assert_true(expr) \
  munit_assert (!!(expr))
#define munit_assert_false(expr) \
  munit_assert (!(expr))

#define munit_assert_cmp_type_full(prefix, suffix, T, fmt, a, op, b)   \
  do { \
    T munit_tmp_a_ = (a); \
    T munit_tmp_b_ = (b); \
    if (!(munit_tmp_a_ op munit_tmp_b_)) {                               \
      munit_errorf("assertion failed: " #a " " #op " " #b " (" prefix "%" fmt suffix " " #op " " prefix "%" fmt suffix ")", \
                   munit_tmp_a_, munit_tmp_b_); \
    } \
  } while (0)

#define munit_assert_cmp_type(T, fmt, a, op, b) \
  munit_assert_cmp_type_full("", "", T, fmt, a, op, b)

#define munit_assert_cmp_char(a, op, b) \
  munit_assert_cmp_type_full("'\\x", "'", char, "02" MUNIT_CHAR_MODIFIER "x", a, op, b)
#define munit_assert_cmp_uchar(a, op, b) \
  munit_assert_cmp_type_full("'\\x", "'", unsigned char, "02" MUNIT_CHAR_MODIFIER "x", a, op, b)
#define munit_assert_cmp_short(a, op, b) \
  munit_assert_cmp_type(short, MUNIT_SHORT_MODIFIER "d", a, op, b)
#define munit_assert_cmp_ushort(a, op, b) \
  munit_assert_cmp_type(unsigned short, MUNIT_SHORT_MODIFIER "u", a, op, b)
#define munit_assert_cmp_int(a, op, b) \
  munit_assert_cmp_type(int, "d", a, op, b)
#define munit_assert_cmp_uint(a, op, b) \
  munit_assert_cmp_type(unsigned int, "u", a, op, b)
#define munit_assert_cmp_long(a, op, b) \
  munit_assert_cmp_type(long int, "ld", a, op, b)
#define munit_assert_cmp_ulong(a, op, b) \
  munit_assert_cmp_type(unsigned long int, "lu", a, op, b)
#define munit_assert_cmp_llong(a, op, b) \
  munit_assert_cmp_type(long long int, "lld", a, op, b)
#define munit_assert_cmp_ullong(a, op, b) \
  munit_assert_cmp_type(unsigned long long int, "u", a, op, b)

#define munit_assert_cmp_size(a, op, b) \
  munit_assert_cmp_type(size_t, MUNIT_SIZE_MODIFIER "u", a, op, b)

#define munit_assert_cmp_float(a, op, b) \
  munit_assert_cmp_type(float, "f", a, op, b)
#define munit_assert_cmp_double(a, op, b) \
  munit_assert_cmp_type(double, "g", a, op, b)
#define munit_assert_cmp_ptr(a, op, b) \
  munit_assert_cmp_type(const void*, "p", a, op, b)

#include <inttypes.h>
#define munit_assert_cmp_int8(a, op, b) \
  munit_assert_cmp_type(int8_t, PRIi8, a, op, b)
#define munit_assert_cmp_uint8(a, op, b) \
  munit_assert_cmp_type(uint8_t, PRIu8, a, op, b)
#define munit_assert_cmp_int16(a, op, b) \
  munit_assert_cmp_type(int16_t, PRIi16, a, op, b)
#define munit_assert_cmp_uint16(a, op, b) \
  munit_assert_cmp_type(uint16_t, PRIu16, a, op, b)
#define munit_assert_cmp_int32(a, op, b) \
  munit_assert_cmp_type(int32_t, PRIi32, a, op, b)
#define munit_assert_cmp_uint32(a, op, b) \
  munit_assert_cmp_type(uint32_t, PRIu32, a, op, b)
#define munit_assert_cmp_int64(a, op, b) \
  munit_assert_cmp_type(int64_t, PRIi64, a, op, b)
#define munit_assert_cmp_uint64(a, op, b) \
  munit_assert_cmp_type(uint64_t, PRIu64, a, op, b)

#define munit_assert_double_equal(a, b, precision) \
  do { \
    const double munit_tmp_a_ = (a); \
    const double munit_tmp_b_ = (b); \
    const double munit_tmp_diff_ = ((munit_tmp_a_ - munit_tmp_b_) < 0) ? \
      -(munit_tmp_a_ - munit_tmp_b_) : \
      (munit_tmp_a_ - munit_tmp_b_); \
    if (MUNIT_UNLIKELY(munit_tmp_diff_ > 1e-##precision)) { \
      munit_errorf("assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
		   munit_tmp_a_, munit_tmp_b_); \
    } \
  } while (0)

#include <string.h>
#define munit_assert_string_equal(a, b) \
  do { \
    const char* munit_tmp_a_ = a; \
    const char* munit_tmp_b_ = b; \
    if (MUNIT_UNLIKELY(strcmp (munit_tmp_a_, munit_tmp_b_) != 0)) { \
      munit_errorf("assertion failed: string " #a " == " #b " (\"%s\" == \"%s\")", \
                   munit_tmp_a_, munit_tmp_b_); \
    } \
  } while (0)

#define munit_assert_string_nequal(a, b) \
  do { \
    const char* munit_tmp_a_ = a; \
    const char* munit_tmp_b_ = b; \
    if (MUNIT_UNLIKELY(strcmp (munit_tmp_a_, munit_tmp_b_) == 0)) { \
      munit_errorf("assertion failed: string " #a " != " #b " (\"%s\" == \"%s\")", \
                   munit_tmp_a_, munit_tmp_b_); \
    } \
  } while (0)

#define munit_assert_memory_equal(size, a, b) \
  do { \
    const uint8_t* munit_tmp_a_ = (void*) (a); \
    const uint8_t* munit_tmp_b_ = (void*) (b); \
    const size_t munit_tmp_size_ = (size); \
    size_t munit_tmp_pos_; \
    for (munit_tmp_pos_ = 0 ; munit_tmp_pos_ < munit_tmp_size_ ; munit_tmp_pos_++) { \
      if (MUNIT_UNLIKELY(munit_tmp_a_[munit_tmp_pos_] != munit_tmp_b_[munit_tmp_pos_])) { \
        munit_errorf("assertion failed: memory " #a " == " #b ", at offset %" MUNIT_SIZE_MODIFIER "u", munit_tmp_pos_); \
        break; \
      } \
    } \
  } while (0)

#define munit_assert_ptr_equal(a, b) \
  munit_assert_cmp_ptr(a, ==, b)
#define munit_assert_null(ptr) \
  munit_assert_cmp_ptr(ptr, ==, NULL)
#define munit_assert_non_null(ptr) \
  munit_assert_cmp_ptr(ptr, !=, NULL)

/*** Memory allocation ***/

void* munit_malloc_ex(const char* filename, int line, size_t size);

#define munit_malloc(size) \
  munit_malloc_ex(__FILE__, __LINE__, (size))

#define munit_new(type) \
  ((type*) munit_malloc(sizeof(type)))

#define munit_calloc(nmemb, size) \
  munit_malloc((nmemb) * (size))

#define munit_newa(type, nmemb) \
  ((type*) munit_calloc((nmemb), sizeof(type)))

/*** Random number generation ***/

void munit_rand_seed(uint32_t seed);

int munit_rand_int(void);
int munit_rand_int_range(int min, int max);
double munit_rand_double(void);
void munit_rand_memory (size_t size, uint8_t buffer[MUNIT_ARRAY_PARAM(size)]);

/*** Tests and Suites ***/

typedef enum {
  /* Test successful */
  MUNIT_OK,
  /* Test failed */
  MUNIT_FAIL,
  /* Test was skipped */
  MUNIT_SKIP,
  /* Test failed due to circumstances not intended to be tested
   * (things like network errors, invalid parameter value, failure to
   * allocate memory in the test harness, etc.). */
  MUNIT_ERROR
} MunitResult;

typedef struct {
  char*  name;
  char** values;
} MunitParameterEnum;

typedef struct {
  char* name;
  char* value;
} MunitParameter;

const char* munit_parameters_get(const MunitParameter params[], const char* key);

typedef enum {
  MUNIT_TEST_OPTION_NONE = 0,
  MUNIT_TEST_OPTION_SINGLE_ITERATION = 1 << 0
} MunitTestOptions;

typedef MunitResult (* MunitTestFunc)(const MunitParameter params[], void* user_data_or_fixture);
typedef void*       (* MunitTestSetup)(const MunitParameter params[], void* user_data);
typedef void        (* MunitTestTearDown)(void* fixture);

typedef struct {
  char*               name;
  MunitTestFunc       test;
  MunitTestSetup      setup;
  MunitTestTearDown   tear_down;
  MunitTestOptions    options;
  MunitParameterEnum* parameters;
} MunitTest;

typedef enum {
  MUNIT_SUITE_OPTION_NONE = 0
} MunitSuiteOptions;

typedef struct MunitSuite_ MunitSuite;

struct MunitSuite_ {
  char*             prefix;
  MunitTest*        tests;
  MunitSuite*       suites;
  unsigned int      iterations;
  MunitSuiteOptions options;
};

int munit_suite_main(const MunitSuite* suite, void* user_data, int argc, const char* argv[MUNIT_ARRAY_PARAM(argc + 1)]);

/* Note: I'm very happy with this API; it's likely to change if I
 * figure out something better.  Suggestions welcome. */

typedef struct MunitArgument_ MunitArgument;

struct MunitArgument_ {
  char* name;
  _Bool (* parse_argument)(const MunitSuite* suite, void* user_data, int* arg, int argc, const char* argv[MUNIT_ARRAY_PARAM(argc + 1)]);
  void (* write_help)(const MunitArgument* argument, void* user_data);
};

int munit_suite_main_custom(const MunitSuite* suite,
                            void* user_data,
                            int argc, const char* argv[MUNIT_ARRAY_PARAM(argc + 1)],
                            const MunitArgument arguments[]);

#if defined(__cplusplus)
}
#endif

#endif /* !defined(MUNIT_H) */
