fast\_convert
=============

A fast implentation of some print and scan functions. This code can for example be used in writing a very fast json implementation. I tested this on a json library and got a speedup of about 7 times.

Some speedup figures:

 * fast\_ftoa is approx. 20 times as fast as sprintf (on x86\_64).
 * fast\_dtoa is approx. 26 times as fast as sprintf (on x86\_64).
 * fast\_strtof is approx. 4 times as fast as strtof (on x86\_64).
 * fast\_strtod is approx. 6 times as fast as strtod (on x86\_64).

Some remarks:

 * All floating point routines have the same binary result as the glibc code (see [tst_convert.c](tst_convert.c)).
 * When ROUND\_EVEN == 1 the strings produced by fast\_ftoa and fast\_dtoa are the same as sprintf.
 * The integer functions  will never overflow but instead return the last character that would cause the overflow in endptr.
 * No checking is done on size of supplied strings.

## Functions

These functions are implemented. See also [fast_convert.h](fast_convert.h)

<pre>
unsigned int fast_sint32 (int32_t v, char *str);
unsigned int fast_sint64 (int64_t v, char *str);
unsigned int fast_uint32 (uint32_t v, char *str);
unsigned int fast_uint64 (uint64_t v, char *str);

int32_t fast_strtos32 (const char *str, char **endptr, int base);
int64_t fast_strtos64 (const char *str, char **endptr, int base);
uint32_t fast_strtou32 (const char *str, char **endptr, int base);
uint64_t fast_strtou64 (const char *str, char **endptr, int base);

unsigned int fast_ftoa (float v, int size, char *line);
unsigned int fast_dtoa (double v, int size, char *line);
float fast_strtof (const char *str, char **endptr);
double fast_strtod (const char *str, char **endptr);
</pre>

The fast\_strto[su] functions support 0x (hex), 0 (octal) and decimal support. <br>
The fast\_strto[fd] functions suppert 0x (hex) and decimal support. Also inf/nan is supported. <br>

The above functions perform simular functions to below libc functions:

<pre>
int sprintf (str, "%d", v);
int sprintf (str, "%ld", v);
int sprintf (str, "%u", v);
int sprintf (str, "%lu", v);

long int strtol(const char *nptr, &endptr, int base);
long long int strtoll(const char *nptr, &endptr, int base);
unsigned long int strtoul(const char *nptr, &endptr, int base);
unsigned long long int strtoul(const char *nptr, &endptr, int base);

int sprintf (line, "%.*g", size, v); // v = float
int sprintf (line, "%.*g", size, v); // v = double
float strtof(const char *str, char **endptr);
double strtod(const char *str, char **endptr);
</pre>

## Performance

The performance for all floating point code is (see [tst_convert.c](tst_convert.c)):

<pre>
f test float convert
s test float sprintf convert
d test double convert
S test double sprintf convert
g test fast_strtof convert
G test fast_strtod convert
t test strtof convert
T test strtod convert
p test precision float
P test precision double
c count differences float
C count differences double
if option after first one is 'n' then no check is done

64 bits (i7-4700MQ + fedora 30)
f:    625.17 (2510.37 / 625.17 = 4.02)
s:   2510.37
d:   1477.76 (5799.09 / 1477.76 = 3.92)
S:   5799.09
fn:    92.23 (1870.78 / 92.23 = 20.28)
sn:  1870.78
dn:   160.43 (4194.16 / 160.43 = 26.14)
Sn:  4194.16
g:    217.58 ((597.76 - 92.23) / (217.58 - 92.23) = 4.03)
t:    597.76
G:    391.04 ((1463.43 - 160.43) / (391.04 - 160.43) = 5.65)
T:   1463.43
gn:   212.41 ((588.91 - 92.23) / (212.41 - 92.23) = 4.13)
tn:   588.91
Gn:   392.33 ((1453.78 - 160.43) / (392.33 - 160.43) = 5.58)
Tn:  1453.78

32 bits (i7-4700MQ + fedora 30)
f:   1023.48 (2836.51 / 1023.48 = 2.77)
s:   2836.51
d:   2769.17 (8004.75 / 2769.17 = 2.89)
S:   8004.75
fn:   147.78 (1909.85 / 147.78 = 12.92)
sn:  1909.85
dn:   381.08 (5383.21 / 381.08 = 14.12)
Sn:  5383.21
g:    449.30 ((1083.00 - 147.78) / (449.30 - 147.78) = 3.10)
t:   1083.00
G:    941.57 ((2830.57 - 381.08) / (941.57 - 381.08) = 4.37)
T:   2830.57
gn:   436.55 ((1069.70 - 147.78) / (436.55 - 147.78) = 3.19)
tn:  1069.70
Gn:   927.53 ((2806.51 - 381.08) / (927.53 - 381.08) = 4.44)
Tn:  2806.51

raspberry pi (3b + raspbian buster):
f:   6005.82 (18654.65 / 6005.82 = 3.11)
s:  18654.65
d:  13205.23 (42775.59 / 13205.23 = 3.24)
S:  42775.59
fn:   819.58 (12308.98 / 819.58 = 15.02)
sn: 12308.98
dn:  1763.91 (30160.90 / 1763.91 = 17.10)
Sn: 30160.90
g:   2422.91 ((6340.77 - 819.58) / (2422.91 - 819.58) = 3.44)
t:   6340.77
G:   4387.61 ((19176.56 - 1763.91) / (4387.61 - 1763.91) = 6.64)
T:  18796.96
gn:  2373.73 ((6226.29 - 819.58) / (2373.73 - 819.58) = 3.48)
tn:  6226.29
Gn:  4347.37 ((18765.85 - 1763.91) / (4347.37 - 1763.91) = 6.58)
Tn: 18765.85

p:    286.03 fast: 0, libc: 0
P:   1090.16 fast: 0, libc: 0

c:   2072.77 0 0.00%
C:   4439.85 0 0.00%

</pre>

## Locale 

The locale decimal point is set at startup. If an application needs to use a different locale it has to call localeconv() to update the decimal point.

## License
  
Licensed under either of
      
 * Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)
    
at your option.
    
### Contribution
      
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in fast\_convert by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
