cc = gcc
cflags = -o a32d a32d.c -Wall -Wextra -lm

a32d:
	$(cc) $(cflags)

clean:
	rm ./a32d; clear;