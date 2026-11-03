#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "../include/elf.h"
#include "../include/decoding/decode.h"

#define MAX_FILENAME_LENGTH 255

unsigned long get_file_size(FILE *fp) {
	// go to end of file
	fseek(fp, 0L, SEEK_END);

	// to get the file size in bytes
	long file_size = ftell(fp);
	
	// go back to start of file
	fseek(fp, 0L, SEEK_SET);
	return file_size;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("(Incorrect format) Usage: a32d <binary>\n");
		exit(EXIT_FAILURE);
	}

	// check length of input binary and output filename lengths
	if (strlen(argv[1]) > MAX_FILENAME_LENGTH) {
		printf("The input binary filename must be up to 255 bytes/characters long.\n");
		exit(EXIT_FAILURE);
	}

	const char *binary = argv[1];
	const char *mode = "rb";
	FILE *fp = fopen(binary, mode);
	if (!fp) {
		printf("Couldn't open the binary: %s\n", binary);
		exit(EXIT_FAILURE);
	}

	unsigned long file_size = get_file_size(fp);
	unsigned char *file_data = malloc(file_size);
	if (!file_data) {
		printf("Couldn't allocate %lu bytes in the heap\n", file_size);
		exit(EXIT_FAILURE);
	}

	// zero out the heap buffer
	memset(file_data, 0, file_size);

	// read the binary and store it into the file_data buffer
	unsigned long i = -1;
	while (!feof(fp)) {
		file_data[++i] = fgetc(fp);
	}

	// don't need the FILE pointer anymore
	fclose(fp);

	elf_header e_header;
	program_header p_header;
	section_header s_header;

	unsigned int program_header_offset = read_elf_header(&e_header, file_data);
	unsigned int section_header_offset = read_program_header(&p_header, file_data, program_header_offset);
	read_section_header(&s_header, file_data, section_header_offset);

	decode_instructions(e_header.e_entry, file_data);
	printf("================================================================================\n");

	return 0;
}