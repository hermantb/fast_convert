/* Copyright 2019 Herman ten Brugge
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#ifndef __FAST_STDIO_H
#define __FAST_STDIO_H

#include <inttypes.h>

#if defined (__cplusplus)
extern "C"
{
#endif

/* See: https://en.wikipedia.org/wiki/IEEE_754#2019 */
#define PREC_FLT_NR	9
#define PREC_DBL_NR	17
#define PREC_FLT	"9"
#define PREC_DBL	"17"

/** \brief fast_sint32
 * 
 * \b Description
 *
 * Convert signed integer to string
 *
 * \param v Integer
 * \param str Buffer to print to
 * \returns lenght string
 */
  extern unsigned int fast_sint32 (int32_t v, char *str);

/** \brief fast_sint64
 * 
 * \b Description
 *
 * Convert signed long integer to string
 *
 * \param v Long integer
 * \param str Buffer to print to
 * \returns lenght string
 */
  extern unsigned int fast_sint64 (int64_t v, char *str);

/** \brief fast_uint32
 * 
 * \b Description
 *
 * Convert unsigned integer to string
 *
 * \param v Integer
 * \param str Buffer to print to
 * \returns lenght string
 */
  extern unsigned int fast_uint32 (uint32_t v, char *str);

/** \brief fast_uint64
 * 
 * \b Description
 *
 * Convert unsigned long integer to string
 *
 * \param v Long integer
 * \param str Buffer to print to
 * \returns lenght string
 */
  extern unsigned int fast_uint64 (uint64_t v, char *str);

/** \brief fast_strtos32
 * 
 * \b Description
 *
 * Convert string to signed integer
 *
 * \param str String to convert from
 * \param endptr optional endptr
 * \returns converted string
 */
  extern int32_t fast_strtos32 (const char *str, char **endptr);

/** \brief fast_strtos64
 * 
 * \b Description
 *
 * Convert string to signed long
 *
 * \param str String to convert from
 * \param endptr optional endptr
 * \returns converted string
 */
  extern int64_t fast_strtos64 (const char *str, char **endptr);

/** \brief fast_strtou32
 * 
 * \b Description
 *
 * Convert string to unsigned int
 *
 * \param str String to convert from
 * \param endptr optional endptr
 * \returns converted string
 */
  extern uint32_t fast_strtou32 (const char *str, char **endptr);

/** \brief fast_strtou64
 * 
 * \b Description
 *
 * Convert string to unsigned long
 *
 * \param str String to convert from
 * \param endptr optional endptr
 * \returns converted string
 */
  extern uint64_t fast_strtou64 (const char *str, char **endptr);

/** \brief fast_ftoa
 * 
 * \b Description
 *
 * Convert float to ascii
 *
 * \param v float value
 * \param size precision
 * \param line pointer to result
 * \returns lenght string
 */
  extern unsigned int fast_ftoa (float v, int size, char *line);

/** \brief fast_dtoa
 * 
 * \b Description
 *
 * Convert double to ascii
 *
 * \param v float value
 * \param size precision
 * \param line pointer to result
 * \returns lenght string
 */
  extern unsigned int fast_dtoa (double v, int size, char *line);

/** \brief fast_strtof
 * 
 * \b Description
 *
 * Convert string to float
 *
 * \param str string to convert
 * \param endptr optional endptr
 * \returns converted float value
 */
  extern float fast_strtof (const char *str, char **endptr);

/** \brief fast_strtod
 * 
 * \b Description
 *
 * Convert string to double
 *
 * \param str string to convert
 * \param endptr optional endptr
 * \returns converted double value
 */
  extern double fast_strtod (const char *str, char **endptr);

#if defined (__cplusplus)
}
#endif

#endif				/* __FAST_STDIO_H */
