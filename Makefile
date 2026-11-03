cc = gcc
cflags = -o a32d ./src/a32d.c -Wall -Wextra -lm

a32d: ./src/a32d.c ./include/elf.h ./include/decoding/decode.h ./include/decoding/branching.h
	$(cc) $(cflags);

clean:
	rm ./a32d; rm ./curl.s; clear;