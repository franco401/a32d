// Purpose of this file:
// Decoding functions for dedicated branching and data transfer instructions
// such as B/BNE (branch/branch not equal), POP, PUSH etc.

#pragma once

#include <stdio.h>
#include "./decode.h"

#define MAX_INSTRUCTION_LENGTH 255

extern int instructions_decoded;
extern char decoded_instruction[MAX_INSTRUCTION_LENGTH];
extern FILE *fp;

// decodes B/BNE instruction (page A8-332)
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
	printf("Decoded instruction %d: %s\n", ++instructions_decoded, decoded_instruction);
}