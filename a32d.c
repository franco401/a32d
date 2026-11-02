#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("(Incorrect format) Usage: a32d <binary> -o <output_filename>\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[2], "-o") != 0) {
		printf("(Missing requried -o flag) Usage: a32d <binary> -o <output_filename>\n");
		exit(EXIT_FAILURE);
	}

	printf("Name of input binary: %s\n", argv[1]);
	return 0;
}