#pragma once

#include <math.h>
#include "./elf.h"

//unsigned int load_value(const unsigned char *bytes, unsigned int n);
//unsigned int load_value_le(const unsigned char *bytes, unsigned int n);
//void decode_instructions(elf_header *header, const unsigned char *file_data);

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

// TODO: replace the printf calls with decode_<category_type> calls
void decode_instruction_category(const unsigned int instruction) {
	unsigned int cond = (instruction & (15 << 28)) >> 28;
	unsigned int op1 = (instruction & (7 << 26)) >> 26;
	unsigned int op = (instruction & (1 << 4)) >> 4;

	printf("cond: %d\nop1: %d\nop: %d\n\n", cond, op1, op);

	if (cond != 0b1111) {
		if (op1 == 0b000 || op1 == 0b001) printf("Category: data processing\n\n");
		else if (op1 == 0b010) printf("Category: load/store\n\n");
		else if (op1 == 0b011 && op == 0) printf("Cateogyr: load/store\n\n");
		else if (op1 == 0b011 && op == 1) printf("Category: media instructions\n\n");
		else if (op1 == 0b100 || op1 == 0b101) printf("category = brach and block data transfer\n\n");
    	else if (op1 == 0b110 || op1 == 0b111) printf("category = coprocessor instructions\n\n");
	}
}

void decode_instructions(elf_header *header, const unsigned char *file_data) {
	unsigned char current_instruction[4];
	unsigned int current_instruction_as_value = 0;
	
	unsigned count = 0;

	// read first 3 instructions (first 3 groups of 4 bytes) from entry point for now
	for (unsigned i = header->e_entry; i < header->e_entry + 12; i += 4) {
		
		// load the current 4 bytes into the current_instruction buffer
		for (unsigned j = 0; j < 4; j++) {
			//printf("Value (0x%x) for byte #%u for instruction #%u\n", file_data[i+j], j, i);
			current_instruction[j] = file_data[i+j];
		}

		// load current 4 bytes as a single integer to decode it as an instruction
		current_instruction_as_value = load_value_le(current_instruction, 4);
		printf("Instruction #%u as a value: %u\n\n", ++count, current_instruction_as_value);
		decode_instruction_category(current_instruction_as_value);
	}
}