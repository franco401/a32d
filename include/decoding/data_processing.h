// Purpose of this file:
// Decoding functions for dedicated data processing instructions
// such as ADD, SUB, AND, OR, etc.

#pragma once

//#include <stdio.h>
//#include "./decode.h"

// data-processing (register) incomplete
void decode_bitwise_and_instruction(unsigned int instruction, unsigned int cond) {
	unsigned int rm = instruction & 15;
	unsigned int type = (instruction & (3 << 5)) >> 5;
	unsigned int imm5 = (instruction & (31 << 7)) >> 7;
	unsigned int rd = (instruction & (15 << 12)) >> 12;
	unsigned int s = (instruction & (1 << 20)) >> 20;


}

// data-processing (register)
void decode_bitwise_bit_clear_instruction(unsigned int instruction, unsigned int cond);
void decode_bitwise_not_instruction(unsigned instruction, unsigned int cond);