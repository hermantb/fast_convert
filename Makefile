OPTIONS = -g -O3 -Wall
CC = gcc

# -fsanitize=address,pointer-compare,pointer-subtract,leak,undefined
# -fsanitize-address-use-after-scope
# export ASAN_OPTIONS='detect_invalid_pointer_pairs=2'
# export LSAN_OPTIONS=''

all: tst_convert64 tst_convert32 tst_convert64e tst_convert32e

tst_convert64: fast_convert.c tst_convert.c
	${CC} -DROUND_EVEN=0 ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert64

tst_convert32: fast_convert.c tst_convert.c
	${CC} -DROUND_EVEN=0 -m32 ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert32

tst_convert64e: fast_convert.c tst_convert.c
	${CC} -DROUND_EVEN=1 ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert64e

tst_convert32e: fast_convert.c tst_convert.c
	${CC} -DROUND_EVEN=1 -m32 ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert32e

test: tst_convert64 tst_convert32 tst_convert64e tst_convert32e
	./tst_convert64 f
	./tst_convert64 s
	./tst_convert64 d
	./tst_convert64 S
	./tst_convert64 fn
	./tst_convert64 sn
	./tst_convert64 dn
	./tst_convert64 Sn
	./tst_convert64 g
	./tst_convert64 t
	./tst_convert64 G
	./tst_convert64 T
	./tst_convert64 gn
	./tst_convert64 tn
	./tst_convert64 Gn
	./tst_convert64 Tn
	./tst_convert32 f
	./tst_convert32 s
	./tst_convert32 d
	./tst_convert32 S
	./tst_convert32 fn
	./tst_convert32 sn
	./tst_convert32 dn
	./tst_convert32 Sn
	./tst_convert32 g
	./tst_convert32 t
	./tst_convert32 G
	./tst_convert32 T
	./tst_convert32 gn
	./tst_convert32 tn
	./tst_convert32 Gn
	./tst_convert32 Tn
	./tst_convert64e f
	./tst_convert64e s
	./tst_convert64e d
	./tst_convert64e S
	./tst_convert64e fn
	./tst_convert64e sn
	./tst_convert64e dn
	./tst_convert64e Sn
	./tst_convert64e g
	./tst_convert64e t
	./tst_convert64e G
	./tst_convert64e T
	./tst_convert64e gn
	./tst_convert64e tn
	./tst_convert64e Gn
	./tst_convert64e Tn
	./tst_convert32e f
	./tst_convert32e s
	./tst_convert32e d
	./tst_convert32e S
	./tst_convert32e fn
	./tst_convert32e sn
	./tst_convert32e dn
	./tst_convert32e Sn
	./tst_convert32e g
	./tst_convert32e t
	./tst_convert32e G
	./tst_convert32e T
	./tst_convert32e gn
	./tst_convert32e tn
	./tst_convert32e Gn
	./tst_convert32e Tn

clean:
	rm -f tst_convert64 tst_convert32 tst_convert64e tst_convert32e
