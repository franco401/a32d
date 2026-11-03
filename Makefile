cc = gcc
cflags = -o a32d ./src/a32d.c -Wall -Wextra -lm

a32d: ./src/a32d.c ./include/elf.h ./include/decode.h
	$(cc) $(cflags); #./a32d ./binaries/curl;

clean:
	rm ./a32d; rm ./curl.s; clear;