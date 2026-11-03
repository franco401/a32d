/*
	TODO: eventually implement the following
	
	- (not finished yet)	data processing category and types
	- (not started yet)		load/store category and instruction types
	- (not started yet)		media instruction category and instruction types
	- (unfinished) 			branch instruction types
	- (not started yet)		coprocessor instruction category and instruction tyoes
	- whatever else

*/

#pragma once

#include <math.h>
#include "./elf.h"

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

void decode_branch_instruction(unsigned int instruction, unsigned int cond) {
	size_t bytes_to_write = 0;
	unsigned int imm24 = (instruction) & ((1 << 24) - 1);

	// continuously shift to the left
	// until imm24 is 4-byte aligned
	while (imm24 % 4 != 0) {
		imm24 <<= 1;
	}
	
	if (cond != 0b1111) {
		sprintf(decoded_instruction, "bne %u\n", imm24);
		bytes_to_write = strlen(decoded_instruction);
	} else {
		sprintf(decoded_instruction, "beq %u\n", imm24);
		bytes_to_write = strlen(decoded_instruction);
	}

	save_decoded_instruction_to_file(fp, decoded_instruction, bytes_to_write);
	printf("Decoded instruction #%d: %s\n", ++instructions_decoded, decoded_instruction);
}

// determine which type of branch instruction to do
void decode_branch_type(unsigned int instruction, unsigned int cond) {
	unsigned int rn = (instruction & (15 << 19)) >> 19;
	unsigned int op = (instruction & (63 << 20)) >> 20;

	if (op >= 32 && op <= 63) {
		decode_branch_instruction(instruction, cond);
	}
}

// determine which type of data processing instruction to do
void decode_data_processing_type(unsigned int instruction, unsigned int cond) {
	sprintf(decoded_instruction, "data_processing_instr\n");
	size_t bytes_to_write = strlen(decoded_instruction);
	save_decoded_instruction_to_file(fp, decoded_instruction, bytes_to_write);
	printf("Decoded instruction #%d: %s\n", ++instructions_decoded, decoded_instruction);
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

void decode_instructions(const elf_header *header, const unsigned char *file_data) {
	unsigned char current_instruction[4];
	unsigned int current_instruction_as_value = 0;
	
	unsigned count = 0;

	// write instructions to an assembly file
	const char *filename = "curl.s";
	fp = fopen(filename, "w");
	if (!fp) {
		printf("Couldn't create the file: %s for writing\n", filename);
		exit(EXIT_FAILURE);
	}

	// read first 3 instructions (first 3 groups of 4 bytes) from entry point for now
	for (unsigned i = header->e_entry; i < header->e_entry + 12; i += 4) {
		
		// load the current 4 bytes into the current_instruction buffer
		for (unsigned j = 0; j < 4; j++) {
			// instruction #j = byte at file_data + i + j
			current_instruction[j] = file_data[i+j];
		}

		// load current 4 bytes as a single integer to decode it as an instruction
		current_instruction_as_value = load_value_le(current_instruction, 4);
		printf("Instruction #%u as a value: %u\n\n", ++count, current_instruction_as_value);
		decode_instruction_category(current_instruction_as_value);
	}

	printf("Decoded instructions written to: %s\n", filename);
	fclose(fp);
}