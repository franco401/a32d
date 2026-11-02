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

	// check input filename length
	if (strlen(argv[1]) > 255 || strlen(argv[3]) > 255) {
		printf("The input and output filenames must be up to 255 bytes/characters long.\n");
		exit(EXIT_FAILURE);
	}

	const char *binary = argv[1];
	char output_filename[255];

	// add .s extension to given output filename
	int bytes_written = sprintf(output_filename, "%s.s", argv[3]);
	if (bytes_written < 0) {
		printf("Couldn't format with sprintf\n");
		exit(EXIT_FAILURE);
	}

	const char *mode = "rb";
	FILE *fp = fopen(binary, mode);
	if (!fp) {
		printf("Couldn't open the binary: %s\n", binary);
		exit(EXIT_FAILURE);
	}

	printf("File names:\nInput binary%s\nOutput filename: %s\n", binary, output_filename);
	return 0;
}