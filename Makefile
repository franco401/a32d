cc = gcc
cflags = -o a32d a32d.c -Wall -Wextra

a32d:
	$(cc) $(cflags)

clean:
	rm ./a32d; clear;