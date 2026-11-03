cc = gcc
cflags = -o a32d ./src/a32d.c -Wall -Wextra -lm

a32d: ./src/a32d.c ./include/elf.h ./include/decode.h
	$(cc) $(cflags)

clean:
	rm ./a32d; clear;