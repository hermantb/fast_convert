fast\_convert
=============

A fast implentation of some print and scan functions. This code can for example be used in writing a very fast json implementation. I tested this on a json library and got a speedup of about 7 times.

Some speedup figures (ROUND\_EVEN == 0):

 * fast\_ftoa is approx. 26 times as fast as sprintf (on x86\_64).
 * fast\_dtoa is approx. 29 times as fast as sprintf (on x86\_64).
 * fast\_strtof is approx. 4 times as fast as strtof (on x86\_64).
 * fast\_strtod is approx. 6 times as fast as strtod (on x86\_64).

Some speedup figures (ROUND\_EVEN == 1):

 * fast\_ftoa is approx. 20 times as fast as sprintf (on x86\_64).
 * fast\_dtoa is approx. 26 times as fast as sprintf (on x86\_64).
 * fast\_strtof is approx. 4 times as fast as strtof (on x86\_64).
 * fast\_strtod is approx. 6 times as fast as strtod (on x86\_64).

Some remarks:

 * All floating point routines have the same binary result as the glibc code (see [tst_convert.c](tst_convert.c)).
 * When ROUND\_EVEN == 1 the strings produced by fast\_ftoa and fast\_dtoa are the same as sprintf.
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
g test fast_strtof convert
G test fast_strtod convert
t test strtof convert
T test strtod convert
p test precision float
P test precision double
c count differences float
C count differences double
if option after first one is 'n' then no check is done

64 bits (i7-4700MQ + fedora 30) ROUND_EVEN=0
f:    594.11 (2510.37 / 594.11 = 4.23)
s:   2510.37
d:   1464.08 (5799.09 / 1464.08 = 3.96)
S:   5799.09
fn:    71.55 (1870.78 / 71.55 = 26.15)
sn:  1870.78
dn:   142.64 (4194.16 / 142.64 = 29.40)
Sn:  4194.16
g:    188.74 ((597.76 - 71.55) / (188.74 - 71.55) = 4.49)
t:    597.76
G:    361.90 ((1463.43 - 142.64) / (361.90 - 142.64) = 6.02)
T:   1463.43
gn:   183.73 ((588.91 - 71.55) / (183.73 - 71.55) = 4.61)
tn:   588.91
Gn:   359.15 ((1453.78 - 142.64) / (359.15 - 142.64) = 6.06)
Tn:  1453.78

64 bits (i7-4700MQ + fedora 30) ROUND_EVEN=1
f:    625.17 (2510.37 / 625.17 = 4.02)
s:   2510.37
d:   1477.76 (5799.09 / 1477.76 = 3.92)
S:   5799.09
fn:    92.23 (1870.78 / 92.23 = 20.28)
sn:  1870.78
dn:   160.43 (4194.16 / 160.43 = 26.14)
Sn:  4194.16
g:    212.92 ((597.76 - 92.23) / (212.92 - 92.23) = 4.18)
t:    597.76
G:    386.96 ((1463.43 - 160.43) / (386.96 - 160.43) = 5.75)
T:   1463.43
gn:   207.17 ((588.91 - 92.23) / (207.17 - 92.23) = 4.32)
tn:   588.91
Gn:   385.13 ((1453.78 - 160.43) / (385.13 - 160.43) = 5.76)
Tn:  1453.78

32 bits (i7-4700MQ + fedora 30) ROUND_EVEN=0
f:   1001.96 (2836.51 / 1001.96 = 2.83)
s:   2836.51
d:   2714.51 (8004.75 / 2714.51 = 2.95)
S:   8004.75
fn:   111.85 (1909.85 / 111.85 = 17.08)
sn:  1909.85
dn:   334.14 (5383.21 / 334.14 = 16.11)
Sn:  5383.21
g:    305.49 ((1083.00 - 111.85) / (305.49 - 111.85) = 5.02)
t:   1083.00
G:    798.17 ((2830.57 - 334.14) / (798.17 - 334.14) = 5.38)
T:   2830.57
gn:   292.99 ((1069.70 - 111.85) / (292.99 - 111.85) = 5.29)
tn:  1069.70
Gn:   787.92 ((2806.51 - 334.14) / (787.92 - 334.14) = 5.45)
Tn:  2806.51

32 bits (i7-4700MQ + fedora 30) ROUND_EVEN=1
f:   1023.48 (2836.51 / 1023.48 = 2.77)
s:   2836.51
d:   2769.17 (8004.75 / 2769.17 = 2.89)
S:   8004.75
fn:   147.78 (1909.85 / 147.78 = 12.92)
sn:  1909.85
dn:   381.08 (5383.21 / 381.08 = 14.12)
Sn:  5383.21
g:    345.59 ((1083.00 - 147.78) / (345.59 - 147.78) = 4.73)
t:   1083.00
G:    891.33 ((2830.57 - 381.08) / (891.33 - 381.08) = 4.80)
T:   2830.57
gn:   334.01 ((1069.70 - 147.78) / (334.01 - 147.78) = 4.95)
tn:  1069.70
Gn:   877.74 ((2806.51 - 381.08) / (877.74 - 381.08) = 4.88)
Tn:  2806.51

raspberry pi (3b + raspbian buster): ROUND_EVEN=0
f:   5892.31 (18654.65 / 5892.31 = 3.17)
s:  18654.65
d:  13151.53 (42775.59 / 13151.53 = 3.25)
S:  42775.59
fn:   639.49 (12308.98 / 639.49 = 19.25)
sn: 12308.98
dn:  1638.83 (30160.90 / 1638.83 = 18.40)
Sn: 30160.90
g:   1888.70 ((6340.77 - 639.49) / (1888.70 - 639.49) = 4.56)
t:   6340.77
G:   3929.60 ((19176.56 - 1638.83) / (3929.60 - 1638.83) = 7.66)
T:  18796.96
gn:  1823.51 ((6226.29 - 639.49) / (1823.51 - 639.49) = 4.72)
tn:  6226.29
Gn:  3785.88 ((18765.85 - 1638.83) / (3785.88 - 1638.83) = 7.98)
Tn: 18765.85

raspberry pi (3b + raspbian buster): ROUND_EVEN=1
f:   6005.82 (18654.65 / 6005.82 = 3.11)
s:  18654.65
d:  13205.23 (42775.59 / 13205.23 = 3.24)
S:  42775.59
fn:   819.58 (12308.98 / 819.58 = 15.02)
sn: 12308.98
dn:  1763.91 (30160.90 / 1763.91 = 17.10)
Sn: 30160.90
g:   2121.96 ((6340.77 - 819.58) / (2121.96 - 819.58) = 4.24)
t:   6340.77
G:   4046.78 ((19176.56 - 1763.91) / (4046.78 - 1763.91) = 7.63)
T:  18796.96
gn:  2170.09 ((6226.29 - 819.58) / (2170.09 - 819.58) = 4.00)
tn:  6226.29
Gn:  3991.01 ((18765.85 - 1763.91) / (3991.01 - 1763.91) = 7.63)
Tn: 18765.85

p:    286.03 fast: 0, libc: 0 (ROUND_EVEN=1)
P:   1090.16 fast: 0, libc: 0 (ROUND_EVEN=1)

c:   2072.77 0 0.00% (ROUND_EVEN=1)
C:   4439.85 0 0.00% (ROUND_EVEN=1)

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
