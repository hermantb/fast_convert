fast\_convert
=============

A fast implentation of some print and scan functions. This code can for example be used in writing a very fast json implementation. I tested this on a json library and got a speedup of about 7 times.

Some speedup figures:

 * fast\_ftoa is approx. 26 times as fast as sprintf (on x86\_64).
 * fast\_dtoa is approx. 30 times as fast as sprintf (on x86\_64).
 * fast\_strtof is approx. 4 times as fast as strtof (on x86\_64).
 * fast\_strtod is approx. 6 times as fast as strtod (on x86\_64).

Some remarks:

 * All floating point routines have the same binary result as the glibc code (see [tst_convert.c](tst_convert.c)).
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
f:    584.64 (2510.37 / 584.64 = 4.29)
s:   2510.37
d:   1447.68 (5799.09 / 1447.68 = 4.01)
S:   5799.09
fn:    71.29 (1870.78 / 71.29 = 26.24)
sn:  1870.78
dn:   142.09 (4194.16 / 142.09 = 29.51)
Sn:  4194.16
g:    198.36 ((597.76 - 71.29) / (198.36 - 71.29) = 4.14)
t:    597.76
G:    364.08 ((1463.43 - 142.09) / (364.08 - 142.09) = 5.95)
T:   1463.43
gn:   192.01 ((588.91 - 71.29) / (192.01 - 71.29) = 4.29)
tn:   588.91
Gn:   358.27 ((1453.78 - 142.09) / (358.27 - 142.09) = 6.07)
Tn:  1453.78

32 bits (i7-4700MQ + fedora 30)
f:    977.42 (2836.51 / 977.42 = 2.90)
s:   2836.51
d:   2713.56 (8004.75 / 2713.56 = 2.95)
S:   8004.75
fn:    91.59 (1909.85 / 91.59 = 20.85)
sn:  1909.85
dn:   335.27 (5383.21 / 335.27 = 16.06)
Sn:  5383.21
g:    284.70 ((1083.00 - 91.59) / (284.70 - 91.59) = 5.14)
t:   1083.00
G:    799.30 ((2830.57 - 335.27) / (799.30 - 335.27) = 5.38)
T:   2830.57
gn:   270.69 ((1069.70 - 91.59) / (270.69 - 91.59) = 5.46)
tn:  1069.70
Gn:   791.07 ((2806.51 - 335.27) / (791.07 - 335.27) = 5.42)
Tn:  2806.51

raspberry pi (3b + raspbian buster):
f:   5820.49 (18654.65 / 5820.49 = 3.21)
s:  18654.65
d:  13131.95 (42775.59 / 13131.95 = 3.26)
S:  42775.59
fn:   598.60 (12308.98 / 598.60 = 20.56)
sn: 12308.98
dn:  1597.20 (30160.90 / 1597.20 = 18.88)
Sn: 30160.90
g:   1876.92 ((6340.77 - 598.60) / (1876.92 - 598.60) = 4.49)
t:   6340.77
G:   3804.73 ((19176.56 - 1597.20) / (3804.73 - 1597.20) = 7.96)
T:  18796.96
gn:  1809.65 ((6226.29 - 598.60) / (1809.65 - 598.60) = 4.65)
tn:  6226.29
Gn:  3751.56 ((18765.85 - 1597.20) / (3751.56 - 1597.20) = 7.96)
Tn: 18765.85

p:    286.03 fast: 32, libc: 2
P:   1090.16 fast:  0, libc: 0

c:   2076.86 1200564350 27.95%
C:   4591.32   36397658  0.85%

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
