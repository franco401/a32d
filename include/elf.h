#pragma once

#include <stdio.h>

typedef struct elf_header {
	unsigned char e_magic_number[4];		// 0x7F 0x45 0x4C 0x46 (zELF, 7 bytes)
	unsigned char e_class;					// 1 = 32-bit, 2 = 64-bit
	unsigned char e_data;					// 1 = little, 2 = big
	unsigned char ei_version;				// 1 = current version of ELF
	unsigned char e_os_abi;					// 0x3 = linux
	unsigned char e_abi_version;			// maybe ignore?
	
	unsigned char e_pad[7];					// ignore, unused
	
	unsigned int e_type;					// ELF file type
	unsigned int e_machine;					// CPU ISA of binary
	unsigned char e_version;				// set to 1 for ELF
	unsigned int e_entry;					// process entry point
	unsigned int e_phoff;					// process header offset
	unsigned int e_shoff;					// section header offset
	unsigned int e_flags;					// use depends on architecture

	unsigned int e_ehsize;					// header size (32 or 64 bytes)
	unsigned int e_phentsize;				// program header entry table size (0x20 or 0x38 for 32-bit and 64-bit)
	unsigned int e_phum;					// number of program header entries
	unsigned int e_shentsize;				// section header entry table size (0x28 or 0x40 for 32-bit and 64-bit)
	unsigned int e_shnum;					// number of sectino header entries
	unsigned int e_shstrndx;				// index of section header table entry containing section names
} elf_header;

typedef struct program_header {
	unsigned int p_type;					// program header type
	unsigned int p_flags;					// 64-bit program header flags (executable, writable or readable)
	unsigned int p_offset;					// program header offset
	unsigned int p_vaddr;					// program header virtual address
	unsigned int p_paddr;					// program header physical address
	unsigned int p_filesz;					// program header file size (may be 0)
	unsigned int p_memsz;					// program header memory size (may be 0)
	unsigned int p_flags32;					// 32-bit program header flags (executable, writable or readable)
	unsigned int p_align;					// program header alignment (0/1 = no alignment or power of 2 where p_vaddr = p_offset - p_align)
} program_header;

typedef struct section_header {
	unsigned int sh_name;					// offset to a section header string in the .shstrtab
	unsigned int sh_type;					// section header type (i.e. null, program data, symbol table, etc.)
	unsigned int sh_flags;					// section header flags (i.e. write, allocate, execute, strings, etc.)
	unsigned int sh_addr;					// section header virtual address
	unsigned int sh_offset;					// section header offset (may be 0)
	unsigned int sh_size;					// section header size (may be 0)
	unsigned int sh_link;					// section header index of an associated index 
	unsigned int sh_info;					// section header info
	unsigned int sh_addralign;				// section header address align (must be a power of 2)
	unsigned int sh_entsize;				// section header entry size (end of section header)
} section_header;

#include "./decoding/decode.h"

char *abi_table[18];
char *get_elf_os_abi(char os_abi) {
	abi_table[0x00] = "Unix - System V";
	abi_table[0x01] = "HP-UX";
	abi_table[0x02] = "NetBSD";
	abi_table[0x03] = "Linux";
	abi_table[0x04] = "GNU Hurd";
	abi_table[0x06] = "Solaris";
	abi_table[0x07] = "AIX (Monterey)";
	abi_table[0x08] = "IRIX";
	abi_table[0x09] = "FreeBSD";
	abi_table[0x0A] = "Tru64";
	abi_table[0x0B] = "Noveil Modesto";
	abi_table[0x0C] = "OpenBSD";
	abi_table[0x0D] = "OpenVMS";
	abi_table[0x0E] = "NonStop Kernel";
	abi_table[0x0F] = "AROS";
	abi_table[0x10] = "FenixOS";
	abi_table[0x11] = "Nuxi CloudABI";
	abi_table[0x12] = "Stratus Technologies OpenVOS";
	return abi_table[(int)os_abi];
}

char *header_type_table[5];
char *other_header_type = "Unknown";
char *get_header_type(int header_type) {
	header_type_table[0x00] = "Unknown";
	header_type_table[0x01] = "ET_REL (Relocatable file)";
	header_type_table[0x02] = "ET_EXEC (Executable file)";
	header_type_table[0x03] = "ET_DYN (Shared object / Position-Independent Executable File)";
	header_type_table[0x04] = "ET_CORE (Core file)";


	if (header_type < 0x05) {
		return header_type_table[header_type];
	}

	switch (header_type) {
		case 0xFE00:
			other_header_type = "ET_LOOS (Reserved inclusive range. Operating system specific)";
			break;
		case 0xFEFF:
			other_header_type = "ET_HIOS (Reserved inclusive range. Operating system specific)";
			break;
		case 0xFF00:
			other_header_type = "ET_LOPROC (Reserved inclusive range. Processor specific)";
			break;
		case 0xFFFF:
			other_header_type = "ET_HIPROC (Reserved inclusive range. Processor specific)";
			break;
	}
	return other_header_type;
}

char *isa_table[79];
char *isa_type;
char *get_isa_type(const unsigned int machine) {
	isa_table[0x00]	= "No specific instruction set";
	isa_table[0x01]	= "AT&T WE 32100";
	isa_table[0x02] = "SPARC";
	isa_table[0x03] = "x86";
	isa_table[0x04] = "Motorola 68000 (M68k)";
	isa_table[0x05]	= "Motorola 88000 (M88k)";
	isa_table[0x06]	= "Intel MCU";
	isa_table[0x07]	= "Intel 80860";
	/*
	0x08	MIPS
	0x09	IBM System/370
	0x0A	MIPS RS3000 Little-endian
	0x0B – 0x0E	Reserved for future use
	0x0F	Hewlett-Packard PA-RISC
	0x13	Intel 80960
	0x14	PowerPC
	0x15	PowerPC (64-bit)
	0x16	S390, including S390x
	0x17	IBM SPU/SPC
	0x18 – 0x23	Reserved for future use
	0x24	NEC V800
	0x25	Fujitsu FR20
	0x26	TRW RH-32
	0x27	Motorola RCE
	*/
	isa_table[0x28] = "Arm (up to Armv7/AArch32)";
	
	/*
	0x29	Digital Alpha
	0x2A	SuperH
	0x2B	SPARC Version 9
	0x2C	Siemens TriCore embedded processor
	0x2D	Argonaut RISC Core
	0x2E	Hitachi H8/300
	0x2F	Hitachi H8/300H
	0x30	Hitachi H8S
	0x31	Hitachi H8/500
	0x32	IA-64
	0x33	Stanford MIPS-X
	0x34	Motorola ColdFire
	0x35	Motorola M68HC12
	0x36	Fujitsu MMA Multimedia Accelerator
	0x37	Siemens PCP
	0x38	Sony nCPU embedded RISC processor
	0x39	Denso NDR1 microprocessor
	0x3A	Motorola Star*Core processor
	0x3B	Toyota ME16 processor
	0x3C	STMicroelectronics ST100 processor
	0x3D	Advanced Logic Corp. TinyJ embedded processor family
	*/
	
	isa_table[0x3E] = "AMD x86-64";

	/*
	0x3F	Sony DSP Processor
	0x40	Digital Equipment Corp. PDP-10
	0x41	Digital Equipment Corp. PDP-11
	0x42	Siemens FX66 microcontroller
	0x43	STMicroelectronics ST9+ 8/16-bit microcontroller
	0x44	STMicroelectronics ST7 8-bit microcontroller
	0x45	Motorola MC68HC16 Microcontroller
	0x46	Motorola MC68HC11 Microcontroller
	0x47	Motorola MC68HC08 Microcontroller
	0x48	Motorola MC68HC05 Microcontroller
	0x49	Silicon Graphics SVx
	0x4A	STMicroelectronics ST19 8-bit microcontroller
	0x4B	Digital VAX
	0x4C	Axis Communications 32-bit embedded processor
	0x4D	Infineon Technologies 32-bit embedded processor
	0x4E	Element 14 64-bit DSP Processor
	0x4F	LSI Logic 16-bit DSP Processor
	*/

	if (machine < 0x8C) {
		return isa_table[machine];
	}

	switch (machine) {
		case 0x8C:
			isa_type = "TMS320C6000 Family";
			break;
		case 0xAF:
			isa_type = "MCST Elbrus e2k";
			break;
		case 0xB7:
			isa_type = "Arm 64-bits (Armv8/AArch64)";
			break;
		case 0xDC:
			isa_type = "Zilog Z80";
			break;
		case 0xF3:
			isa_type = "RISC-V";
			break;
		case 0xF7:
			isa_type = "Berkeley Packet Filter";
			break;
		case 0x101:
			isa_type = "WDC 65C816";
			break;
		case 0x102:
			isa_type = "LoongArch";
			break;
	}

	return isa_type;
}

char *program_segment_type;
char *get_program_segment_type(const unsigned int segment_type) {
	switch (segment_type) {
		case 0x00000000:
			program_segment_type = "PT_NULL (Program header table entry unused)";
			break;
		case 0x00000001:
			program_segment_type = "PT_LOAD (Loadable segment)";
			break;
		case 0x00000002:
			program_segment_type = "PT_DYNAMIC (Dynamic linking information)";
			break;
		case 0x00000003:
			program_segment_type = "PT_INTERP (Interpreter information)";
			break;
		case 0x00000004:
			program_segment_type = "PT_NOTE (Axuiliary information)";
			break;
		case 0x00000005:
			program_segment_type = "PT_SHLIB (Reserved)";
			break;
		case 0x00000006:
			program_segment_type = "PT_PHDR (Segment containing program header table itself)";
			break;
		case 0x00000007:
			program_segment_type = "PT_TLS (Thread-Local Storage template)";
			break;
		case 0x60000000:
			program_segment_type = "PT_LOOS (Reserved inclusive range. Operating system specific.)";
			break;
		case 0x6FFFFFFF:
			program_segment_type = "PT_HIOS (Reserved inclusive range. Operating system specific.)";
			break;
		case 0x70000000:
			program_segment_type = "PT_LOPROC (Reserved inclusive range. Processor specific.)";
			break;
		case 0x7FFFFFFF:
			program_segment_type = "PT_HIPROC (Reserved inclusive range. Processor specific.)";
			break;
	}
	return program_segment_type;
}

char *segement_dependent_flags;
char *get_segment_dependent_flags(const unsigned int p_flags) {
	switch (p_flags) {
		case 0x1:
			segement_dependent_flags = "PF_X (Executable segment)";
			break;
		case 0x2:
			segement_dependent_flags = "PF_W (Writable segment)";
			break;
		case 0x4:
			segement_dependent_flags = "PF_R (Readable segment)";
			break;
		default:
			segement_dependent_flags = "Unknown value";
			break;
	}
	return segement_dependent_flags;
}

char *section_header_type;
char *get_section_header_type(const unsigned int sh_type) {
	switch (sh_type) {
		case 0x0:
			section_header_type = "SHT_NULL";
			break;
		case 0x1:
			section_header_type = "SHT_PROGBITS";
			break;
		case 0x2:
			section_header_type = "SHT_SYMTAB";
			break;
		case 0x3:
			section_header_type = "SHT_STRTAB";
			break;
		case 0x4:
			section_header_type = "SHT_RELA";
			break;
		case 0x5:
			section_header_type = "SHT_HASH";
			break;
		case 0x6:
			section_header_type = "SHT_DYNAMIC";
			break;
		case 0x7:
			section_header_type = "SHT_NOTE";
			break;
		case 0x8:
			section_header_type = "SHT_NOBITS";
			break;
		case 0x9:
			section_header_type = "SHT_REL";
			break;
		case 0xA:
			section_header_type = "SHT_SHLIB";
			break;
		case 0xB:
			section_header_type = "SHT_DYNSYM";
			break;
		case 0xE:
			section_header_type = "SHT_INIT_ARRAY";
			break;
		case 0xF:
			section_header_type = "SHT_FINI_ARRAY";
			break;
		case 0x10:
			section_header_type = "SHT_PREINIT_ARRAY";
			break;
		case 0x11:
			section_header_type = "SHT_GROUP";
			break;
		case 0x12:
			section_header_type = "SHT_SYMTAB_SHNDX";
			break;
		case 0x13:
			section_header_type = "SHT_NUM";
			break;
		case 0x60000000:
			section_header_type = "Start OS-specific";
			break;
		default:
			section_header_type = "Unknown value";
			break;
	}
	return section_header_type;
}

char *section_header_flags;
char *get_section_header_flags(const unsigned int sh_flags) {
	switch (sh_flags) {
		case 0x0:
			section_header_flags = "Unknown value";
			break;
		case 0x1:
			section_header_flags = "SHF_WRITE";
			break;
		case 0x2:
			section_header_flags = "SHF_ALLOC";
			break;
		case 0x4:
			section_header_flags = "SHF_EXECINSTR";
			break;
		case 0x10:
			section_header_flags = "SHF_MERGE";
			break;
		case 0x20:
			section_header_flags = "SHF_STRINGS";
			break;
		case 0x40:
			section_header_flags = "SHF_INFO_LINK";
			break;
		case 0x80:
			section_header_flags = "SHF_LINK_ORDER";
			break;
		case 0x100:
			section_header_flags = "SHF_OS_NONCONFORMING";
			break;
		case 0x200:
			section_header_flags = "SHF_GROUP";
			break;
		case 0x400:
			section_header_flags = "SHF_TLS";
			break;
		case 0x0FF00000:
			section_header_flags = "SHF_MASKOS";
			break;
		case 0xF0000000:
			section_header_flags = "SHF_MASKPROC";
			break;
		case 0x4000000:
			section_header_flags = "SHF_ORDERED";
			break;
		case 0x8000000:
			section_header_flags = "SHF_EXECLUDE";
			break;
		default:
			section_header_flags = "Unknown value";
			break;
	}
	return section_header_flags;
}

void read_section_header(section_header *header, const unsigned char *file_data, unsigned int section_header_offset) {
	header->sh_name = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_type = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_flags = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_addr = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_offset = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_size = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_link = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_info = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_addralign = file_data[section_header_offset];
	section_header_offset += 4;

	header->sh_entsize = file_data[section_header_offset];
	section_header_offset += 4;

	printf("Section Header Data\n\nSection header name offset:\t\t0x%x (%d)\nSection header type:\t\t\t%s\nSection header flags:\t\t\t%s\nSection header offset:\t\t\t0x%x (%u)\nSection header size:\t\t\t0x%x (%u)\nSection header link:\t\t\t0x%x (%u)\nSection header info:\t\t\t0x%x (%u)\nSection header address align:\t\t0x%x (%u)\nSection header entry size:\t\t0x%x (%u)\n", header->sh_name, header->sh_name, get_section_header_type(header->sh_type), get_section_header_flags(header->sh_flags), header->sh_offset, header->sh_offset, header->sh_size, header->sh_size, header->sh_link, header->sh_link, header->sh_info, header->sh_info, header->sh_addralign, header->sh_addralign, header->sh_entsize, header->sh_entsize);
	printf("================================================================================\n");	
}

unsigned int read_program_header(program_header *header, const unsigned char *file_data, unsigned int program_header_offset) {
	header->p_type = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_flags = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_offset = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_vaddr = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_paddr = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_filesz = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_memsz = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_flags = file_data[program_header_offset];
	program_header_offset += 4;

	header->p_align = file_data[program_header_offset];
	program_header_offset += 4;

	printf("Program Header Data\n\nProgram header type:\t\t\t%s\nSegment dependent flags:\t\t%d | %s\nProgram header offset:\t\t\t0x%x (%u)\nProgram header virtual address:\t\t0x%x (%u)\nProgram header physical address:\t0x%x (%u)\nProgram file size:\t\t\t0x%x (%u)\nProgram memory size:\t\t\t0x%x (%u)\nProgram header alignment:\t\t0x%x (%u)\n", get_program_segment_type(header->p_type), header->p_flags, get_segment_dependent_flags(header->p_flags), header->p_offset, header->p_offset, header->p_vaddr, header->p_vaddr, header->p_paddr, header->p_paddr, header->p_filesz, header->p_filesz, header->p_memsz, header->p_memsz, header->p_align, header->p_align);
	printf("================================================================================\n");	

	// return updated offset value to jump to section header
	return program_header_offset;
}

unsigned int read_elf_header(elf_header *header, const unsigned char *file_data) {
	for (unsigned int i = 0; i < 4; i++) {
		header->e_magic_number[i] = file_data[i];
	}

	unsigned long offset = 4;
	header->e_class = file_data[offset];
	header->e_data = file_data[++offset];
	header->ei_version = file_data[++offset];
	header->e_os_abi = file_data[++offset];
	header->e_abi_version = file_data[++offset];
	offset += 8;

	header->e_type = load_value(file_data + offset, 2);
	offset += 2;

	header->e_machine = load_value(file_data + offset, 2);
	offset += 2;

	header->e_version = 0x1;
	offset += 4;

	header->e_entry = load_value(file_data + offset, 4);
	offset += 4;

	header->e_phoff = load_value(file_data + offset, 4);
	offset += 4;

	header->e_shoff = load_value(file_data + offset, 4);
	offset += 4;

	header->e_flags = load_value(file_data + offset, 4);
	offset += 4;

	header->e_ehsize = load_value(file_data + offset, 2);
	offset += 2;

	header->e_phentsize = load_value(file_data + offset, 2);
	offset += 2;

	header->e_phum = load_value(file_data + offset, 2);
	offset += 2;

	header->e_shentsize = load_value(file_data + offset, 2);
	offset += 2;

	header->e_shnum = load_value(file_data + offset, 2);
	offset += 2;

	header->e_shstrndx = load_value(file_data + offset, 2);
	offset += 2;

	printf("================================================================================\n");
	printf("ELF Header Data\n\nMagic number:\t\t\t\t%s\nClass (Format):\t\t\t\t%s\nData (Endianess):\t\t\t%s\nEI Version:\t\t\t\t%d\nOS ABI:\t\t\t\t\t%s\nABI Version:\t\t\t\t%d\n", header->e_magic_number, (header->e_class == 1) ? "ELF32" : "ELF64", (header->e_data == 1) ? "Little Endian" : "Big Endian", header->ei_version, get_elf_os_abi(header->e_os_abi), header->e_abi_version);
	printf("Header Type:\t\t\t\t%s\nMachine ISA:\t\t\t\t%s\nELF Version:\t\t\t\t%d\nEntry point address:\t\t\t0x%x (%u)\nProcess header offset:\t\t\t0x%x (%u)\nSection header offset:\t\t\t0x%x (%u)\nFlags:\t\t\t\t\t0x%x (%d)\n", get_header_type(header->e_type), get_isa_type(header->e_machine), header->e_version, header->e_entry, header->e_entry, header->e_phoff, header->e_phoff, header->e_shoff, header->e_shoff, header->e_flags, header->e_flags);
	printf("ELF header size:\t\t\t0x%x (%d)\nProgram header size:\t\t\t0x%x (%d)\nNumber of entires in program header:\t0x%x (%u)\nSection header size:\t\t\t0x%x (%d)\nNumber of entires in section header:\t0x%x (%d)\nSection header string index:\t\t0x%x (%d)\n", header->e_ehsize, header->e_ehsize, header->e_phentsize, header->e_phentsize, header->e_phum, header->e_phum, header->e_shentsize, header->e_shentsize, header->e_shnum,header->e_shnum, header->e_shstrndx, header->e_shstrndx);
	printf("================================================================================\n");

	// return program header offset to jump to it for reading that header
	return header->e_phoff;
}