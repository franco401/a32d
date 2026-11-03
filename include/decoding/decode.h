/*
	TODO: eventually implement the following
	
	- (unfinished)			data processing category and types (just need types now)
	- (not started yet)		load/store category and instruction types
	- (not started yet)		media instruction category and instruction types
	- (unfinished) 			branch instruction types
	- (not started yet)		coprocessor instruction category and instruction tyoes
	- whatever else

*/

// Purpose of this file:
// Decodes instruction category --> category type --> dedicated instructions

#pragma once

#include <math.h>

#define MAX_INSTRUCTION_LENGTH 		255
#define DECODED_INSTRUCTIONS_LEN	3

int instructions_decoded = -1;								// global index used by decode_<instruction>_instruction functions
char decoded_instruction[MAX_INSTRUCTION_LENGTH];			// the current decoded ASM instruction to be written to a file
FILE *fp;

void save_decoded_instruction_to_file(FILE *fp, const char *data, unsigned long bytes_to_write) {
	for (unsigned long i = 0; i < bytes_to_write; i++) {
		fputc(data[i], fp);
	}
}

#include "./branching.h"
#include "./data_processing.h"

// load a stream of n bytes as one large value
unsigned int load_value(const unsigned char *bytes, unsigned int n) {
	unsigned int value = 0;
	unsigned int exp = 0;

	for (unsigned int i = 0; i < n; i++) {
		value += bytes[i] * (int)pow(256, exp);
		exp++;
	}

	return value;
}

// little endian version of the function above
unsigned int load_value_le(const unsigned char *bytes, unsigned int n) {
	unsigned int value = 0;
	unsigned int exp = 0;

	for (int i = n-1; i > -1; i--) {
		value += bytes[i] * (unsigned int)pow(256, exp);
		exp++;
	}

	return value;
}

// determine which type of branch instruction to do
void decode_branch_type(unsigned int instruction, unsigned int cond) {
	unsigned int rn = (instruction & (15 << 19)) >> 19;
	unsigned int op = (instruction & (63 << 20)) >> 20;

	// might be an incorrect way to check for op
	if (op >= 32 && op <= 63) {
		decode_branch_instruction(instruction, cond);
	}
}

void data_processing_register_type(unsigned int instruction, unsigned int cond) {
	if (cond == 0b00000 || cond == 0b00001) {
		sprintf(decoded_instruction, "AND - bitwise AND (register) (page A8-324");
		// decode_bitwise_and_instruction(instruction, cond);
	}


	// last two data processing register type instructions
	if (cond == 0b11100 || cond == 0b11101) {
		sprintf(decoded_instruction, "BIC - bitwise clear (register) (page A8-340)\n");
		// decode_bitwise_bit_clear_instruction(instruction, cond);
	} else if (cond == 0b11110 || cond == 0b11111) {
		sprintf(decoded_instruction, "MVN - bitwise NOT (register) (page A8-507)\n");
		// decode_bitwise_not_instruction(instruction, cond);
	}
}

// implement this next from page 195
void data_processing_register_shifted_register_type(unsigned int instruction, unsigned int cond);

// determine which type of data processing instruction to do
void decode_data_processing_type(unsigned int instruction, unsigned int cond) {
	unsigned int op = (instruction & (1 << 25)) >> 25;
	unsigned int op1 = (instruction & (31 << 20)) >> 20;
	unsigned int op2 = (instruction & (15 << 4) >> 4);

	// TODO: replace sprintf calls with data processing type2 calls (i.e. data processing register, data processing register-shifted register, etc.)

	if (op == 0) {
		// check if op1 is not 10xx0
		if (op1 != 0b10000 && op1 != 0b10010 && op1 != 0b10100 && op1 != 10110) {
			// check if op2 is xxx0
			if (op2 == 0b0000 || op2 == 0b0010 || op2 == 0b0100 || op2 == 0b0110 || op2 == 0b1000 || op2 == 0b1010 || op2 == 0b1100 || op2 == 0b1110) {
				data_processing_register_type(instruction, cond);
			} else if (op2 == 0b001 || op2 == 0b0011 || op2 == 0b0101 || op2 == 0111) { /* check if op2 is 0xx1 */
				sprintf(decoded_instruction, "Data processing (register-shifted register) (page A5-195)\n");
				// data_processing_register_shifted_register_type(instruction, cond);
			}
		} else if (op1 == 0b10000 || op1 == 0b10010 || op1 == 0b10100 || op1 == 0b10110) { /* check if op1 is 10xx0 */
			// check if op2 is 0xxx
			if (op2 <= 7) {
				sprintf(decoded_instruction, "misc instruction (page A5-205)\n");
			} else if (op2 == 0b1000 || op2 == 0b1010 || op2 == 0b1100 || op2 == 0b1110) {
				sprintf(decoded_instruction, "halfword multiply and accumulate (page A5-200)\n");
			}
		} else if (op1 <= 15) { /* check if op1 is 0xxxx */
			if (op2 == 0b1001) {
				sprintf(decoded_instruction, "multiply and accumulate (page A5-200)\n");
			}
		} else if (op1 >= 16 && op1 <= 31) { /* check if op1 is 1xxxx */
			if (op2 == 0b1001) {
				sprintf(decoded_instruction, "sycnhronization primitives (page A5-203)\n");
			}
		} else if (op1 == 1 || op1 == 16 || op1 == 17) { /* check if op1 is not 0xx1x */
			if (op2 == 0b1011 || op2 == 0b1101 || op2 == 0b1111) {
				sprintf(decoded_instruction, "extra load/store instruction (page A5-201)\n");
			}
		} else if (op1 == 0b00010 || op1 == 0b01010 || op1 == 0b10010 || op1 == 0b11010) {
			if (op2 == 0b1101 || op2 == 0b1111) {
				sprintf(decoded_instruction, "extra load/store instruction (page A5-201\n");
			}
		} else if (op1 != 1 && op1 != 16 && op1 != 17) { /* check if op1 is 0xx1x */
			if (op2 == 0b1011) {
				sprintf(decoded_instruction, "extra load/store instruction (page A5-201)\n");
			}
		} else if (op1 == 0b0011 || op1 == 0b0111 || op1 == 0b1011 || op1 == 0b1111) { /* check if op1 is 0xx11 */
			if (op2 == 0b1101 || op2 == 0b1111) {
				sprintf(decoded_instruction, "extra load/store instruction (page A5-201)\n");
			}
		}
	} else {
		// check if op1 is not 10xx0
		if (op1 != 0b10000 && op1 != 0b10010 && op1 != 0b10100 && op1 != 0b10110) {
			sprintf(decoded_instruction, "data processing immediate (page A5-197)\n");
		} else if (op1 == 0b10000) {
			sprintf(decoded_instruction, "16 bit immediate load, MOV (immediate) (page A8-485)\n");
		} else if (op1 == 0b10100) {
			sprintf(decoded_instruction, "High halfword 16-bit immediate load, MOVT (page A8-492)\n");
		} else if (op1 == 0b10010 || op1 == 0b10110) {
			sprintf(decoded_instruction, "MSR (immediate) (page A5-204)\n");
		}
	}

	size_t bytes_to_write = strlen(decoded_instruction);
	save_decoded_instruction_to_file(fp, decoded_instruction, bytes_to_write);
	printf("Decoded instruction %d: %s\n", ++instructions_decoded, decoded_instruction);
}

// decode instruction category which includes:
// data processing, load/store, media, branching, coprocessor, etc
void decode_instruction_category(const unsigned int instruction) {
	unsigned int cond = (instruction & (15 << 28)) >> 28;
	unsigned int op1 = (instruction & (7 << 26)) >> 26;
	unsigned int op = (instruction & (1 << 4)) >> 4;

	printf("cond: %d\nop1: %d\nop: %d\n\n", cond, op1, op);

	if (cond != 0b1111) {
		if (op1 == 0b000 || op1 == 0b001) decode_data_processing_type(instruction, cond);
		else if (op1 == 0b010) printf("Category: load/store\n\n");
		else if (op1 == 0b011 && op == 0) printf("Cateogyr: load/store\n\n");
		else if (op1 == 0b011 && op == 1) printf("Category: media instructions\n\n");
		else if (op1 == 0b100 || op1 == 0b101) decode_branch_type(instruction, cond);
    	else if (op1 == 0b110 || op1 == 0b111) printf("category = coprocessor instructions\n\n");
	}
}

void decode_instructions(const unsigned int entry_address, const unsigned char *file_data) {
	unsigned char current_instruction[4];
	unsigned int current_instruction_as_value = 0;
	
	unsigned count = -1;

	// write instructions to an assembly file
	const char *filename = "curl.s";
	fp = fopen(filename, "w");
	if (!fp) {
		printf("Couldn't create the file: %s for writing\n", filename);
		exit(EXIT_FAILURE);
	}

	// read first 3 instructions (first 3 groups of 4 bytes) from entry point for now
	for (unsigned i = entry_address; i < entry_address + 12; i += 4) {
		
		// load the current 4 bytes into the current_instruction buffer
		for (unsigned j = 0; j < 4; j++) {
			// instruction #j = byte at file_data + i + j
			current_instruction[j] = file_data[i+j];
		}

		// load current 4 bytes as a single integer to decode it as an instruction
		current_instruction_as_value = load_value_le(current_instruction, 4);
		printf("Instruction %u as a value: %u\n\n", ++count, current_instruction_as_value);
		decode_instruction_category(current_instruction_as_value);
	}

	printf("Decoded instructions written to the file: %s\n", filename);
	fclose(fp);
}