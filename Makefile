OPTIONS = -g -O3 -Wall
CC = gcc
WIN_GCC = i686-w64-mingw32-gcc

# -fsanitize=address,pointer-compare,pointer-subtract,leak,undefined
# -fsanitize-address-use-after-scope
# export ASAN_OPTIONS='detect_invalid_pointer_pairs=2'
# export LSAN_OPTIONS=''

all: tst_convert64 tst_convert32 libfast_convert.a libfast_convert.so

tst_convert64: fast_convert.h fast_convert.c tst_convert.c
	${CC} ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert64

tst_convert32: fast_convert.h fast_convert.c tst_convert.c
	${CC} -m32 ${OPTIONS} fast_convert.c tst_convert.c -o tst_convert32

libfast_convert.a: fast_convert.h fast_convert.c
	${CC} ${OPTIONS} -c fast_convert.c
	rm -f libfast_convert.a
	ar rv libfast_convert.a fast_convert.o
	rm -f fast_convert.o

libfast_convert.so: fast_convert.h fast_convert.c
	${CC} ${OPTIONS} -fPIC -shared -o libfast_convert.so fast_convert.c

test: tst_convert64 tst_convert32
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

allwin: tst_convert64.exe

tst_convert64.exe: fast_convert.c tst_convert.c
	$(WIN_GCC) ${OPTIONS} -DWIN fast_convert.c tst_convert.c -o tst_convert64.exe

doc: fast_convert.h README.md
	doxygen

clean:
	rm -rf tst_convert64 tst_convert32 libfast_convert.a libfast_convert.so doc 
	rm -f tst_convert64.exe
