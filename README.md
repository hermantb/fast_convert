fast\_convert
=============

A fast implentation of some print and scan functions. This code can for example be used in writing a very fast json implementation. I tested this on a json library and got a speedup of about 7 times.

Some speedup figures:

 * fast\_ftoa is approx. 26 times as fast as sprintf (on x86\_64).
 * fast\_dtoa is approx. 30 times as fast as sprintf (on x86\_64).
 * fast\_strtof is approx. 4 times as fast as strtof (on x86\_64).
 * fast\_strtod is approx. 6 times as fast as strtod (on x86\_64).

Some remarks:

 * All floating point routines have the same binary result as the glibc code (see [tst_convert.c](tst_convert.c)). This is only tested for floats with precision 9 and doubles with precision 17.
 * The integer functions have for example no base parameter to speed up the code.
 * The integer functions  will never overflow but instead return the last character that would cause the overflow in endptr.
 * No checking is done on size of supplied strings.

## Functions

These functions are implemented. See also [fast_convert.h](fast_convert.h)

<pre>
unsigned int fast_sint32 (int32_t v, char *str);
unsigned int fast_sint64 (int64_t v, char *str);
unsigned int fast_uint32 (uint32_t v, char *str);
unsigned int fast_uint64 (uint64_t v, char *str);

int32_t fast_strtos32 (const char *str, char **endptr);
int64_t fast_strtos64 (const char *str, char **endptr);
uint32_t fast_strtou32 (const char *str, char **endptr);
uint64_t fast_strtou64 (const char *str, char **endptr);

char *fast_ftoa (float v, int size, char *line);
char *fast_dtoa (double v, int size, char *line);
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

long int strtol(const char *nptr, &endptr, 0);
long long int strtoll(const char *nptr, &endptr, 0);
unsigned long int strtoul(const char *nptr, &endptr, 0);
unsigned long long int strtoul(const char *nptr, &endptr, 0);

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
c count differences float
C count differences float double
g test fast_strtof convert
G test fast_strtod convert
t test strtof convert
T test strtod convert
if option after first one is 'n' then no check is done

64 bits (i7-4700MQ + fedora 30)
f:    588.28 (2510.37 / 588.28 = 4.27)
s:   2510.37
d:   1460.01 (5799.09 / 1460.01 = 3.97)
S:   5799.09
fn:    70.25 (1870.78 / 70.25 = 26.63)
sn:  1870.78
dn:   139.43 (4194.16 / 139.43 = 30.08)
Sn:  4194.16
g:    187.41 ((597.76 - 70.25) / (187.41 - 70.25) = 4.50)
t:    597.76
G:    364.08 ((1463.43 - 139.43) / (364.08 - 139.43) = 5.89)
T:   1463.43
gn:   180.59 ((588.91 - 70.25) / (180.59 - 70.25) = 4.70)
tn:   588.91
Gn:   362.51 ((1453.78 - 139.43) / (362.51 - 139.43) = 5.89)
Tn:  1453.78

32 bits (i7-4700MQ + fedora 30)
f:    977.02 (2836.51 / 977.02 = 2.90)
s:   2836.51
d:   2700.71 (8004.75 / 2700.71 = 2.96)
S:   8004.75
fn:    92.87 (1909.85 / 92.87 = 20.56)
sn:  1909.85
dn:   328.80 (5383.21 / 328.80 = 16.37)
Sn:  5383.21
g:    288.82 ((1083.00 - 92.87) / (288.82 - 92.87) = 5.05)
t:   1083.00
G:    782.89 ((2830.57 - 328.80) / (782.89 - 328.80) = 5.51)
T:   2830.57
gn:   271.69 ((1069.70 - 92.87) / (271.69 - 92.87) = 5.46)
tn:  1069.70
Gn:   771.44 ((2806.51 - 328.80) / (771.44 - 328.80) = 5.60)
Tn:  2806.51

raspberry pi (3b + raspbian buster):
f:   5947.35 (18654.65 / 5947.35 = 3.14)
s:  18654.65
d:  13101.18 (42775.59 / 13101.18 = 3.27)
S:  42775.59
fn:   631.04 (12308.98 / 631.04 = 19.51)
sn: 12308.98
dn:  1756.41 (30160.90 / 1756.41 = 17.17)
Sn: 30160.90
g:   1903.70 ((6340.77 - 631.04) / (1903.70 - 631.04) = 4.49)
t:   6340.77
G:   3969.36 ((19176.56 - 1756.41) / (3969.36 - 1756.41) = 7.87)
T:  18796.96
gn:  1860.55 ((6226.29 - 631.04) / (1860.55 - 631.04) = 4.55)
tn:  6226.29
Gn:  3860.65 ((18765.85 - 1756.41) / (3860.65 - 1756.41) = 8.08)
Tn: 18765.85
</pre>

## License
  
Licensed under either of
      
 * Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)
    
at your option.
    
### Contribution
      
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in fast\_convert by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
