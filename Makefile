CFLAGS= -O2 -Wall

all: src/cml.c

clean:
	rm -f src/utf.o src/utf8.o src/utf16.o src/utf32.o src/encoder.o

.PHONY: clean

src/utf.o: src/utf.c src/utf.h src/types.h
src/utf8.o: src/utf8.c src/utf.o src/utf.h src/types.h
src/utf16.o: src/utf16.c src/utf.o src/utf.h src/types.h
src/utf32.o: src/utf32.c src/utf.o src/utf.h src/types.h
src/encoder.o: src/encoder.c src/encoder.h src/utf.o
