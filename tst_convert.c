/* Copyright 2019 Herman ten Brugge
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fast_convert.h"

/* See: https://en.wikipedia.org/wiki/Linear_congruential_generator */
#define RAND_IA         UINT64_C(0x5851F42D4C957F2D)
#define RAND_IC         UINT64_C(0x14057B7EF767814F)

static double
get_time (void)
{
#ifdef WIN
  struct timeval tv;

  gettimeofday (&tv, NULL);
  return ((double) tv.tv_sec) * 1e9 + ((double) tv.tv_usec) * 1e3;
#else
  struct timespec curtime;

  clock_gettime (CLOCK_REALTIME, &curtime);
  return ((double) curtime.tv_sec) * 1e9 + ((double) curtime.tv_nsec);
#endif
}

int
main (int argc, char **argv)
{
  unsigned int len;
  unsigned int j;
  uint64_t df;
  uint64_t dl;
  uint64_t i;
  uint64_t c;
  uint64_t cf;
  uint64_t cl;
  uint64_t nf = 0;
  uint64_t nl = 0;
  uint64_t r = 1234567890;
  uint64_t max = UINT64_C (4294967295);
  char *endptr;
  double start;
  double end;
  union
  {
    uint32_t u;
    float f;
  } tf, rf, rf2;
  union
  {
    uint64_t ul;
    double d;
  } td, rd, rd2;
  static const struct
  {
    int32_t v;
    const char *r;
  } sitst[] = {
    {0, "0"},
    {1, "1"},
    {-1, "-1"},
    {10, "10"},
    {-10, "-10"},
    {100, "100"},
    {-100, "-100"},
    {1000, "1000"},
    {-1000, "-1000"},
    {999999999, "999999999"},
    {-999999999, "-999999999"},
    {2147483647, "2147483647"},
    {-2147483648, "-2147483648"}
  };
  static const struct
  {
    uint32_t v;
    const char *r;
  } uitst[] = {
    {0, "0"},
    {1, "1"},
    {10, "10"},
    {100, "100"},
    {1000, "1000"},
    {999999999, "999999999"},
    {4294967295, "4294967295"},
  };
  static const struct
  {
    int64_t v;
    const char *r;
  } sltst[] = {
    {0, "0"},
    {1, "1"},
    {-1, "-1"},
    {10, "10"},
    {-10, "-10"},
    {100, "100"},
    {-100, "-100"},
    {1000, "1000"},
    {-1000, "-1000"},
    {999999999, "999999999"},
    {-999999999, "-999999999"},
    {1000000000, "1000000000"},
    {-1000000000, "-1000000000"},
    {INT64_C (999999999999999999), "999999999999999999"},
    {INT64_C (-999999999999999999), "-999999999999999999"},
    {INT64_C (1000000000000000000), "1000000000000000000"},
    {INT64_C (-1000000000000000000), "-1000000000000000000"},
    {INT64_C (9000000000000000000), "9000000000000000000"},
    {INT64_C (-9000000000000000000), "-9000000000000000000"},
    {INT64_C (9223372036854775807), "9223372036854775807"},
    /* gcc gives error with -9223372036854775808 */
    {INT64_C (-9223372036854775807) - 1, "-9223372036854775808"}
  };
  static const struct
  {
    uint64_t v;
    const char *r;
  } ultst[] = {
    {0, "0"},
    {1, "1"},
    {10, "10"},
    {100, "100"},
    {1000, "1000"},
    {999999999, "999999999"},
    {1000000000, "1000000000"},
    {UINT64_C (999999999999999999), "999999999999999999"},
    {UINT64_C (1000000000000000000), "1000000000000000000"},
    {UINT64_C (12000000000000000000), "12000000000000000000"},
    {UINT64_C (18446744073709551615), "18446744073709551615"}
  };
  static const uint32_t ftst[] = {
    0x00000000,
    0x80000000,
    0x7FC00000,
    0XFFC00000,
    0x7F800000,
    0xFF800000,
    0x00000001,
    0x00400000,
    0x3A000000,
    0x42F68000,
    0xC2f68000,
    0x2EDBE6FF,
    0x501502F9,
    0x4640E400,
    0x3D800000,
    0x60AD78EC,
    0x19416D9A,
    0xBDAB80D9,
    0x3D800000,
    0x015965DC,
    0x7F7FFFFF
  };
  static const uint64_t dtst[] = {
    0x0000000000000000,
    0x8000000000000000,
    0x7FF8000000000000,
    0xFFF8000000000000,
    0x7FF0000000000000,
    0xFFF0000000000000,
    0x0000000000000001,
    0x0008000000000000,
    0x3F40000000000000,
    0x405ED00000000000,
    0xC05ED00000000000,
    0x3DDB7CDFD9D7BDBB,
    0x4202A05F20000000,
    0x40C81C8000000000,
    0x3FB0000000000000,
    0x4415AF1D78B58C40,
    0xEFB04A2E0D69FE4C,
    0xA630920E8868104F,
    0xC2AB41C01CE507A0,
    0xFAFE218675E0741B,
    0x0030000000000000,
    0x7FEFFFFFFFFFFFFF,
  };
  char line[1000];
  char line2[1000];

  for (i = 0; i < sizeof (sitst) / sizeof (sitst[0]); i++) {
    len = fast_sint32 (sitst[i].v, line);
    if (strcmp (line, sitst[i].r) || len != strlen (sitst[i].r)) {
      printf ("fast_sint32: conversion failed for %d %s\n", sitst[i].v, line);
    }
  }
  for (i = 0; i < sizeof (uitst) / sizeof (uitst[0]); i++) {
    len = fast_uint32 (uitst[i].v, line);
    if (strcmp (line, uitst[i].r) || len != strlen (uitst[i].r)) {
      printf ("fast_uint32: conversion failed for %u %s\n", uitst[i].v, line);
    }
  }
  for (i = 0; i < sizeof (sltst) / sizeof (sltst[0]); i++) {
    len = fast_sint64 (sltst[i].v, line);
    if (strcmp (line, sltst[i].r) || len != strlen (sltst[i].r)) {
      printf ("fast_sint64: conversion failed for %" PRId64 " %s\n",
	      sltst[i].v, line);
    }
  }
  for (i = 0; i < sizeof (ultst) / sizeof (ultst[0]); i++) {
    len = fast_uint64 (ultst[i].v, line);
    if (strcmp (line, ultst[i].r) || len != strlen (ultst[i].r)) {
      printf ("fast_uint64: conversion failed for %" PRIu64 " %s\n",
	      ultst[i].v, line);
    }
  }
  for (i = 0; i < sizeof (sitst) / sizeof (sitst[0]); i++) {
    if (fast_strtos32 (sitst[i].r, &endptr) != sitst[i].v || *endptr != '\0') {
      printf ("fast_strtos32: conversion failed for %d %d\n", sitst[i].v,
	      fast_strtos32 (sitst[i].r, &endptr));
    }
    if (sitst[i].v < 0) {
      sprintf (line, "-0x%x", -sitst[i].v);
    }
    else {
      sprintf (line, "0x%x", sitst[i].v);
    }
    if (fast_strtos32 (line, &endptr) != sitst[i].v || *endptr != '\0') {
      printf ("fast_strtos32: conversion failed for %d %d\n", sitst[i].v,
	      fast_strtos32 (line, &endptr));
    }
    if (sitst[i].v < 0) {
      sprintf (line, "-0X%X", -sitst[i].v);
    }
    else {
      sprintf (line, " +0X%X", sitst[i].v);
    }
    if (fast_strtos32 (line, &endptr) != sitst[i].v || *endptr != '\0') {
      printf ("fast_strtos32: conversion failed for %d %d\n", sitst[i].v,
	      fast_strtos32 (line, &endptr));
    }
    if (sitst[i].v < 0) {
      sprintf (line, "-0%o", -sitst[i].v);
    }
    else {
      sprintf (line, "0%o", sitst[i].v);
    }
    if (fast_strtos32 (line, &endptr) != sitst[i].v || *endptr != '\0') {
      printf ("fast_strtos32: conversion failed for %d %d\n", sitst[i].v,
	      fast_strtos32 (line, &endptr));
    }
  }
  fast_strtos32 ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("0x7fffffff0", &endptr);
  if (*endptr != '0') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("-0x80000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("0177777777770", &endptr);
  if (*endptr != '0') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("-020000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("2147483648", &endptr);
  if (*endptr != '8') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  fast_strtos32 ("-2147483649", &endptr);
  if (*endptr != '9') {
    printf ("fast_strtos32: failed endptr %s\n", endptr);
  }
  for (i = 0; i < sizeof (uitst) / sizeof (uitst[0]); i++) {
    if (fast_strtou32 (uitst[i].r, &endptr) != uitst[i].v || *endptr != '\0') {
      printf ("fast_strtou32: conversion failed for %u %u\n", uitst[i].v,
	      fast_strtou32 (uitst[i].r, &endptr));
    }
    sprintf (line, "0x%x", uitst[i].v);
    if (fast_strtou32 (line, &endptr) != uitst[i].v || *endptr != '\0') {
      printf ("fast_strtou32: conversion failed for %u %u\n", uitst[i].v,
	      fast_strtou32 (line, &endptr));
    }
    sprintf (line, " 0X%X", uitst[i].v);
    if (fast_strtou32 (line, &endptr) != uitst[i].v || *endptr != '\0') {
      printf ("fast_strtou32: conversion failed for %u %u\n", uitst[i].v,
	      fast_strtou32 (line, &endptr));
    }
    sprintf (line, "0%o", uitst[i].v);
    if (fast_strtou32 (line, &endptr) != uitst[i].v || *endptr != '\0') {
      printf ("fast_strtou32: conversion failed for %u %u\n", uitst[i].v,
	      fast_strtou32 (line, &endptr));
    }
  }
  fast_strtou32 ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtou32: failed endptr %s\n", endptr);
  }
  fast_strtou32 ("0x100000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtou32: failed endptr %s\n", endptr);
  }
  fast_strtou32 ("040000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtou32: failed endptr %s\n", endptr);
  }
  fast_strtou32 ("4294967297", &endptr);
  if (*endptr != '7') {
    printf ("fast_strtou32: failed endptr %s\n", endptr);
  }
  for (i = 0; i < sizeof (sltst) / sizeof (sltst[0]); i++) {
    if (fast_strtos64 (sltst[i].r, &endptr) != sltst[i].v || *endptr != '\0') {
      printf ("fast_strtos64: conversion failed for %" PRId64 " %" PRId64
	      "\n", sltst[i].v, fast_strtos64 (sltst[i].r, &endptr));
    }
    if (sltst[i].v < 0) {
      sprintf (line, "-0x%" PRIx64, -sltst[i].v);
    }
    else {
      sprintf (line, "0x%" PRIx64, sltst[i].v);
    }
    if (fast_strtos64 (line, &endptr) != sltst[i].v || *endptr != '\0') {
      printf ("fast_strtos64: conversion failed for %" PRId64 " %" PRId64
	      "\n", sltst[i].v, fast_strtos64 (line, &endptr));
    }
    if (sltst[i].v < 0) {
      sprintf (line, "-0X%" PRIX64, -sltst[i].v);
    }
    else {
      sprintf (line, " +0X%" PRIX64, sltst[i].v);
    }
    if (fast_strtos64 (line, &endptr) != sltst[i].v || *endptr != '\0') {
      printf ("fast_strtos64: conversion failed for %" PRId64 " %" PRId64
	      "\n", sltst[i].v, fast_strtos64 (line, &endptr));
    }
    if (sltst[i].v < 0) {
      sprintf (line, "-0%" PRIo64, -sltst[i].v);
    }
    else {
      sprintf (line, "0%" PRIo64, sltst[i].v);
    }
    if (fast_strtos64 (line, &endptr) != sltst[i].v || *endptr != '\0') {
      printf ("fast_strtos64: conversion failed for %" PRId64 " %" PRId64
	      "\n", sltst[i].v, fast_strtos64 (line, &endptr));
    }
  }
  fast_strtos64 ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("0x7FFFFFFFFFFFFFFF0", &endptr);
  if (*endptr != '0') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("-0x80000000000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("07777777777777777777770", &endptr);
  if (*endptr != '0') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("-01000000000000000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("9223372036854775808", &endptr);
  if (*endptr != '8') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  fast_strtos64 ("-9223372036854775809", &endptr);
  if (*endptr != '9') {
    printf ("fast_strtos64: failed endptr %s\n", endptr);
  }
  for (i = 0; i < sizeof (ultst) / sizeof (ultst[0]); i++) {
    if (fast_strtou64 (ultst[i].r, &endptr) != ultst[i].v || *endptr != '\0') {
      printf ("fast_strtou64: conversion failed for %" PRIu64 " %" PRIu64
	      "\n", ultst[i].v, fast_strtou64 (ultst[i].r, &endptr));
    }
    sprintf (line, "0x%" PRIx64, ultst[i].v);
    if (fast_strtou64 (line, &endptr) != ultst[i].v || *endptr != '\0') {
      printf ("fast_strtou64: conversion failed for %" PRIu64 " %" PRIu64
	      "\n", ultst[i].v, fast_strtou64 (line, &endptr));
    }
    sprintf (line, " 0X%" PRIX64, ultst[i].v);
    if (fast_strtou64 (line, &endptr) != ultst[i].v || *endptr != '\0') {
      printf ("fast_strtou64: conversion failed for %" PRIu64 " %" PRIu64
	      "\n", ultst[i].v, fast_strtou64 (line, &endptr));
    }
    sprintf (line, "0%" PRIo64, ultst[i].v);
    if (fast_strtou64 (line, &endptr) != ultst[i].v || *endptr != '\0') {
      printf ("fast_strtou64: conversion failed for %" PRIu64 " %" PRIu64
	      "\n", ultst[i].v, fast_strtou64 (line, &endptr));
    }
  }
  fast_strtou64 ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtou64: failed endptr %s\n", endptr);
  }
  fast_strtou64 ("0x10000000000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtou64: failed endptr %s\n", endptr);
  }
  fast_strtou64 ("02000000000000000000001", &endptr);
  if (*endptr != '1') {
    printf ("fast_strtou64: failed endptr %s\n", endptr);
  }
  fast_strtou64 ("18446744073709551617", &endptr);
  if (*endptr != '7') {
    printf ("fast_strtou64: failed endptr %s\n", endptr);
  }
  for (i = 0; i < sizeof (ftst) / sizeof (ftst[0]); i++) {
    tf.u = ftst[i];
    fast_ftoa (tf.f, PREC_FLT_NR, line);
    rf.f = strtof (line, NULL);
    if (rf.u != tf.u) {
      printf ("fast_ftoa: failed 0x%08x %s\n", ftst[i], line);
    }
  }
  if (fast_ftoa (1, PREC_FLT_NR + 1, line) != 1 ||
      strcmp (line, "1") ||
      fast_ftoa (1, -1, line) != 1 ||
      strcmp (line, "1")) {
    printf ("fast_ftoa: size failed: %s\n", line);
  }
  if (fast_ftoa (fast_strtof ("nan", NULL), PREC_FLT_NR, line) != 3 ||
      fast_ftoa (fast_strtof ("nan", NULL), PREC_FLT_NR, line) != 3 ||
      fast_ftoa (0, PREC_FLT_NR, line) != 1 ||
      fast_ftoa (1, 3, line) != 1 ||
      strcmp (line, "1") ||
      fast_ftoa (100, 4, line) != 3 ||
      strcmp (line, "100") ||
      fast_ftoa (965447232.0, 1, line) != 5 ||
      strcmp (line, "1e+09") ||
      fast_ftoa (295000000.0, 2, line) != 5 ||
      strcmp (line, "3e+08") ||
      fast_ftoa (-531790.812, 9, line) != 11 ||
      strcmp (line, "-531790.812")) {
    printf ("fast_ftoa: size failed: %s\n", line);
  }
  for (i = 0; i < sizeof (dtst) / sizeof (dtst[0]); i++) {
    td.ul = dtst[i];
    fast_dtoa (td.d, PREC_DBL_NR, line);
    rd.d = strtod (line, NULL);
    if (rd.ul != td.ul) {
      printf ("fast_dtoa: failed 0x%016" PRIx64 " %s\n", dtst[i], line);
    }
  }
  if (fast_dtoa (1, PREC_DBL_NR + 1, line) != 1 ||
      strcmp (line, "1") ||
      fast_dtoa (1, -1, line) != 1 ||
      strcmp (line, "1")) {
    printf ("fast_dtoa: size failed: %s\n", line);
  }
  if (fast_dtoa (fast_strtod ("nan", NULL), PREC_DBL_NR, line) != 3 ||
      fast_dtoa (fast_strtod ("nan", NULL), PREC_DBL_NR, line) != 3 ||
      fast_dtoa (0, PREC_DBL_NR, line) != 1 ||
      fast_dtoa (1, 3, line) != 1 ||
      strcmp (line, "1") ||
      fast_dtoa (100, 4, line) != 3 ||
      strcmp (line, "100") ||
      fast_dtoa (107696295750000, 10, line) != 15 ||
      strcmp (line, "1.076962958e+14") ||
      fast_dtoa (9.5202756046990724e-14, 1, line) != 5 ||
      strcmp (line, "1e-13")) {
    printf ("fast_dtoa: size failed: %s\n", line);
  }
  for (i = 0; i < sizeof (ftst) / sizeof (ftst[0]); i++) {
    tf.u = ftst[i];
#ifdef WIN
    if (!isnan (tf.f) && !isinf (tf.f)) {
#endif
      sprintf (line, "%." PREC_FLT "g", tf.f);
      rf.f = fast_strtof (line, &endptr);
      if (rf.u != tf.u || *endptr != '\0') {
	printf ("fast_strtof: failed 0x%08x %s\n", ftst[i], line);
      }
      tf.u = ftst[i];
      sprintf (line, "%.30g", tf.f);
      rf.f = fast_strtof (line, &endptr);
      if (rf.u != tf.u || *endptr != '\0') {
	printf ("fast_strtof: failed 0x%08x %s\n", ftst[i], line);
      }
#ifdef WIN
    }
#endif
#ifndef WIN
    tf.u = ftst[i];
    sprintf (line, "%a", tf.f);
    rf.f = fast_strtof (line, &endptr);
    if (rf.u != tf.u || *endptr != '\0') {
      printf ("fast_strtof: failed 0x%08x %s\n", ftst[i], line);
    }
    tf.u = ftst[i];
    sprintf (line, "%A", tf.f);
    rf.f = fast_strtof (line, &endptr);
    if (rf.u != tf.u || *endptr != '\0') {
      printf ("fast_strtof: failed 0x%08x %s\n", ftst[i], line);
    }
#endif
  }
  fast_strtof ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtof: failed endptr %s\n", endptr);
  }
  if (fast_strtof (" +1.0", &endptr) != 1.0 || *endptr != '\0') {
    printf ("fast_strtof: failed endptr %s\n", endptr);
  }
  fast_strtof ("nan(1234)", &endptr);
  if (*endptr != '\0') {
    printf ("fast_strtof: nan failed %s\n", endptr);
  }
  fast_strtof ("nabc", &endptr);
  if (*endptr != 'n') {
    printf ("fast_strtof: endptr failed %s\n", endptr);
  }
  fast_strtof ("iabc", &endptr);
  if (*endptr != 'i') {
    printf ("fast_strtof: endptr failed %s\n", endptr);
  }
  fast_strtof ("0xi", &endptr);
  if (*endptr != 'x') {
    printf ("fast_strtof: endptr failed %s\n", endptr);
  }
  fast_strtof ("infinity", &endptr);
  if (*endptr != '\0') {
    printf ("fast_strtof: infinity failed %s\n", endptr);
  }
  if (fast_strtof ("1000000000000.0", &endptr) != 1e+12f || *endptr != '\0') {
    printf ("fast_strtof: conversion failed %s\n", endptr);
  }
  if (fast_strtof (" 0XfffFFFFfff.0", &endptr) != 1.09951163e+12f ||
      *endptr != '\0') {
    printf ("fast_strtof: hex conversion failed %s\n", endptr);
  }
  if (fast_strtof (" 0Xf.000Ff0000", &endptr) != 15.0002432f ||
      *endptr != '\0') {
    printf ("fast_strtof: hex conversion failed %s\n", endptr);
  }
  if (fast_strtof ("0.5000000894069671630859375", &endptr) !=
      0.5000000894069671630859375f || *endptr != '\0') {
    printf ("fast_strtof: conversion failed %s\n", endptr);
  }
  for (i = 0; i < sizeof (dtst) / sizeof (dtst[0]); i++) {
    td.ul = dtst[i];
#ifdef WIN
    if (!isnan (td.d) && !isinf (td.d)) {
#endif
      sprintf (line, "%." PREC_DBL "g", td.d);
      rd.d = fast_strtod (line, &endptr);
      if (rd.ul != td.ul || *endptr != '\0') {
	printf ("fast_strtod: failed 0x%016" PRIx64 "x %s\n", dtst[i], line);
      }
      td.ul = dtst[i];
      sprintf (line, "%.40g", td.d);
      rd.d = fast_strtod (line, &endptr);
      if (rd.ul != td.ul || *endptr != '\0') {
	printf ("fast_strtod: failed 0x%016" PRIx64 "x %s\n", dtst[i], line);
      }
#ifdef WIN
    }
#endif
#ifndef WIN
    td.ul = dtst[i];
    sprintf (line, "%a", td.d);
    rd.d = fast_strtod (line, &endptr);
    if (rd.ul != td.ul || *endptr != '\0') {
      printf ("fast_strtod: failed 0x%016" PRIx64 "x %s\n", dtst[i], line);
    }
    td.ul = dtst[i];
    sprintf (line, "%A", td.d);
    rd.d = fast_strtod (line, &endptr);
    if (rd.ul != td.ul || *endptr != '\0') {
      printf ("fast_strtod: failed 0x%016" PRIx64 "x %s\n", dtst[i], line);
    }
#endif
  }
  fast_strtod ("error", &endptr);
  if (strcmp (endptr, "error")) {
    printf ("fast_strtod: failed endptr %s\n", endptr);
  }
  if (fast_strtod (" +1.0", &endptr) != 1.0 || *endptr != '\0') {
    printf ("fast_strtod: failed endptr %s\n", endptr);
  }
  fast_strtod ("nan(1234)", &endptr);
  if (*endptr != '\0') {
    printf ("fast_strtod: nan failed %s\n", endptr);
  }
  fast_strtod ("nabc", &endptr);
  if (*endptr != 'n') {
    printf ("fast_strtod: endptr failed %s\n", endptr);
  }
  fast_strtod ("iabc", &endptr);
  if (*endptr != 'i') {
    printf ("fast_strtod: endptr failed %s\n", endptr);
  }
  fast_strtod ("0xi", &endptr);
  if (*endptr != 'x') {
    printf ("fast_strtod: endptr failed %s\n", endptr);
  }
  fast_strtod ("infinity", &endptr);
  if (*endptr != '\0') {
    printf ("fast_strtod: infinity failed %s\n", endptr);
  }
  if (fast_strtod ("100000000000000000000.0", &endptr) != 1e+20 ||
      *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod (" 0XfffFFFFfffffffffff.0", &endptr) !=
      4.7223664828696452e+21 || *endptr != '\0') {
    printf ("fast_strtod: hex conversion failed %s\n", endptr);
  }
  if (fast_strtod (" 0Xf.000Ff000000000000", &endptr) != 15.000243186950684 ||
      *endptr != '\0') {
    printf ("fast_strtod: hex conversion failed %s\n", endptr);
  }
  if (fast_strtod ("3.571e266", &endptr) != 3.571e266 || *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod
      ("0.500000000000000166533453693773481063544750213623046875",
       &endptr) != 0.500000000000000166533453693773481063544750213623046875 ||
      *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod ("3.518437208883201171875e13", &endptr) !=
      3.518437208883201171875e13 || *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod ("62.5364939768271845828", &endptr) !=
      62.5364939768271845828 || *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod ("8.10109172351e-10", &endptr) != 8.10109172351e-10 ||
      *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod
      ("1.50000000000000011102230246251565404236316680908203125",
       &endptr) != 1.50000000000000011102230246251565404236316680908203125 ||
      *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (fast_strtod
      ("9007199254740991.4999999999999999999999999999999995",
       &endptr) != 9007199254740991.4999999999999999999999999999999995 ||
      *endptr != '\0') {
    printf ("fast_strtod: conversion failed %s\n", endptr);
  }
  if (argc < 2 || strlen (argv[1]) == 0) {
    printf ("Usage: %s option\n", argv[0]);
    printf ("options:\n");
    printf ("  f test float convert\n");
    printf ("  s test float sprintf convert\n");
    printf ("  d test double convert\n");
    printf ("  S test double sprintf convert\n");
    printf ("  g test fast_strtof convert\n");
    printf ("  G test fast_strtod convert\n");
    printf ("  t test strtof convert\n");
    printf ("  T test strtod convert\n");
    printf ("  p test precision float\n");
    printf ("  P test precision double\n");
    printf ("  c count differences float\n");
    printf ("  C count differences double\n");
    printf ("  if option after first one is 'n' then no check is done\n");
    exit (1);
  }
  start = get_time ();
  if (argv[1][0] == 'f' || argv[1][0] == 's') {
    for (i = 0; i <= max; i++) {
      tf.u = i;
      if (argv[1][0] == 'f') {
	fast_ftoa (tf.f, PREC_FLT_NR, line);
      }
      else {
	sprintf (line, "%." PREC_FLT "g", tf.f);
      }
      if (argv[1][1] != 'n') {
	rf.f = strtof (line, NULL);
	if (isnan (rf.f) && isnan (tf.f)) {
	  if (signbit (rf.f) != signbit (tf.f)) {
	    printf ("%u %u %s %.9g\n", rf.u, tf.u, line, tf.f);
	  }
	}
	else if (isinf (rf.f) && isinf (tf.f)) {
	  if (signbit (rf.f) != signbit (tf.f)) {
	    printf ("%u %u %s %.9g\n", rf.u, tf.u, line, tf.f);
	  }
	}
	else if (rf.u != tf.u || argc > 2) {
	  printf ("%u %u %s %.9g\n", rf.u, tf.u, line, tf.f);
	}
      }
    }
  }
  else if (argv[1][0] == 'd' || argv[1][0] == 'S') {
    for (i = 0; i <= max; i++) {
      r = r * RAND_IA + RAND_IC;
      td.ul = r;
      if (argv[1][0] == 'd') {
	fast_dtoa (td.d, PREC_DBL_NR, line);
      }
      else {
	sprintf (line, "%." PREC_DBL "g", td.d);
      }
      if (argv[1][1] != 'n') {
	rd.d = strtod (line, NULL);
	if (isnan (rd.d) && isnan (td.d)) {
	  if (signbit (rd.d) != signbit (td.d)) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    td.d);
	  }
	}
	else if (isinf (rd.d) && isinf (td.d)) {
	  if (signbit (rd.d) != signbit (td.d)) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    td.d);
	  }
	}
	else {
	  if (rd.ul != td.ul || argc > 2) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    td.d);
	  }
	}
      }
    }
  }
  else if (argv[1][0] == 'g' || argv[1][0] == 't') {
    for (i = 0; i <= max; i++) {
      tf.u = i;
      fast_ftoa (tf.f, PREC_FLT_NR, line);
      if (argv[1][0] == 'g') {
	rf.f = fast_strtof (line, NULL);
      }
      else {
	rf.f = strtof (line, NULL);
      }
      if (argv[1][1] != 'n') {
	if (isnan (rf.f) && isnan (tf.f)) {
	  if (signbit (rf.f) != signbit (tf.f)) {
	    printf ("%u %u %s %.9g\n", rf.u, tf.u, line, rf.f);
	  }
	}
	else if (isinf (rf.f) && isinf (tf.f)) {
	  if (signbit (rf.f) != signbit (tf.f)) {
	    printf ("%u %u %s %.9g\n", rf.u, tf.u, line, rf.f);
	  }
	}
	else if (rf.u != tf.u || argc > 2) {
	  printf ("%u %u %s %.9g\n", rf.u, tf.u, line, rf.f);
	}
      }
    }
  }
  else if (argv[1][0] == 'G' || argv[1][0] == 'T') {
    for (i = 0; i <= max; i++) {
      r = r * RAND_IA + RAND_IC;
      td.ul = r;
      fast_dtoa (td.d, PREC_DBL_NR, line);
      if (argv[1][0] == 'G') {
	rd.d = fast_strtod (line, NULL);
      }
      else {
	rd.d = strtod (line, NULL);
      }
      if (argv[1][1] != 'n') {
	if (isnan (rd.d) && isnan (td.d)) {
	  if (signbit (rd.d) != signbit (td.d)) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    rd.d);
	  }
	}
	else if (isinf (rd.d) && isinf (td.d)) {
	  if (signbit (rd.d) != signbit (td.d)) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    rd.d);
	  }
	}
	else {
	  if (rd.ul != td.ul || argc > 2) {
	    printf ("%" PRIu64 " %" PRIu64 " %s %.17g\n", rd.ul, td.ul, line,
		    rd.d);
	  }
	}
      }
    }
  }
  else if (argv[1][0] == 'p') {
    max /= 100;
    for (j = 1; j <= PREC_FLT_NR; j++) {
      cf = 0;
      cl = 0;
      for (i = 0; i <= max; i++) {
	do {
	  r = r * RAND_IA + RAND_IC;
	  tf.u = r;
	} while (isnan (tf.f) || isinf (tf.f));
#if 1				/* not real compare because sprintf uses double */
	fast_ftoa (tf.f, j, line);
#else
	fast_dtoa (tf.f, j, line);
#endif
	sprintf (line2, "%.*g", j, tf.f);
	rf.f = fast_strtof (line, NULL);
	rf2.f = fast_strtof (line2, NULL);
	df = abs ((int) rf.u - (int) tf.u);
	dl = abs ((int) rf2.u - (int) tf.u);
	if (df > dl) {
	  cf++;
	}
	if (df < dl) {
	  cl++;
	}
	if (df != dl && argc > 2) {
	  printf ("%s: %u %u %u %u %s %s %.9g\n", df > dl ? "fast" : "libc",
		  j, rf.u, rf2.u, tf.u, line, line2, tf.f);
	}
      }
      printf ("%u: fast %" PRIu64 ", libc %" PRIu64 "\n", j, cf, cl);
      nf += cf;
      nl += cl;
    }
    printf ("fast: %" PRIu64 ", libc: %" PRIu64 "\n", nf, nl);
  }
  else if (argv[1][0] == 'P') {
    max /= 100;
    for (j = 1; j <= PREC_DBL_NR; j++) {
      cf = 0;
      cl = 0;
      for (i = 0; i <= max; i++) {
	do {
	  r = r * RAND_IA + RAND_IC;
	  td.ul = r;
	} while (isnan (td.d) || isinf (td.d));
	fast_dtoa (td.d, j, line);
	sprintf (line2, "%.*g", j, td.d);
	rd.d = fast_strtod (line, NULL);
	rd2.d = fast_strtod (line2, NULL);
	df = llabs ((long long) rd.ul - (long long) td.ul);
	dl = llabs ((long long) rd2.ul - (long long) td.ul);
	if (df > dl) {
	  cf++;
	}
	if (df < dl) {
	  cl++;
	}
	if (df != dl && argc > 2) {
	  printf ("%s: %u %" PRIu64 " %" PRIu64 " %" PRIu64 " %s %s %.17g\n",
		  df > dl ? "fast" : "libc", j, rd.ul, rd2.ul, td.ul, line,
		  line2, td.d);
	}
      }
      printf ("%u: fast %" PRIu64 ", libc %" PRIu64 "\n", j, cf, cl);
      nf += cf;
      nl += cl;
    }
    printf ("fast: %" PRIu64 ", libc: %" PRIu64 "\n", nf, nl);
  }
  else if (argv[1][0] == 'c') {
    c = 0;
    for (i = 0; i <= max; i++) {
      tf.u = i;
#if 1				/* not real compare because sprintf uses double */
      fast_ftoa (tf.f, PREC_FLT_NR, line);
#else
      fast_dtoa (tf.f, PREC_FLT_NR, line);
#endif
      sprintf (line2, "%." PREC_FLT "g", tf.f);
      j = strcmp (line, line2);
      c += j;
      if (j && argc > 2) {
	printf ("%u %s %s %.17g\n", tf.u, line, line2, tf.f);
      }
    }
    printf ("%" PRIu64 " %.2f%%\n", c, 100.0 * c / max);
  }
  else if (argv[1][0] == 'C') {
    c = 0;
    for (i = 0; i <= max; i++) {
      r = r * RAND_IA + RAND_IC;
      td.ul = r;
      fast_dtoa (td.d, PREC_DBL_NR, line);
      sprintf (line2, "%." PREC_DBL "g", td.d);
      j = strcmp (line, line2);
      c += j;
      if (j && argc > 2) {
	printf ("%" PRIu64 " %s %s\n", td.ul, line, line2);
      }
    }
    printf ("%" PRIu64 " %.2f%%\n", c, 100.0 * c / max);
  }
  end = get_time ();
  printf ("%s: %.2f\n", argv[1], (end - start) / 1e9);
  return 0;
}
