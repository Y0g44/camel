CFLAGS= -O2 -Wall

all: src/cml.c

clean:
	rm -f src/utf.o src/utf8.o src/utf16.o src/utf32.o src/tokenizer.o

.PHONY: clean

src/utf.o: src/utf.c src/utf.h src/def.h
src/utf8.o: src/utf8.c src/utf.o src/utf.h src/def.h
src/utf16.o: src/utf16.c src/utf.o src/utf.h src/def.h
src/utf32.o: src/utf32.c src/utf.o src/utf.h src/def.h
src/tokenizer.o: src/tokenizer.c src/tokenizer.h src/utf.o src/utf.h src/def.h
