# a32d (ARM32 Disassembler)
A small, work in progress, prototype Linux diassembler for ARM32 / ARMv7 ELF binaries I'm building for fun in C.

# Setup and install

1. Build the program
```
make a32d
```

2. Launch it (using 32-bit ARM version of curl as an example)
```
./a32d ./binaries/curl
```

# Features (implemented so far using 32-bit ARM version of curl)

- Parses 52-byte long 32-bit ELF binary header data
- Parses ELF program and section headers

- Reads, prints, and decodes the first 3 instructions, then writes them into a .s file

- Currently only decodes just the branch instruction (specifically bne or Branch Not Equal)

- Currently only finished implementing decoding the instruction's category according to page 192 of ARM Architecture Reference Manual ARMv7-A and ARMv7-R ediiton


# Some goals to do later
- Decode the other instruction category types
- Decode more specific instructions
- Write out more than a handful of instructions to a .s file


# Helpful resources so far
1. Wikipedia page for ELF binary structure
https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

2. ARMv7 ASM Manual
https://support.arm.com/documentation/ddi0406/latest/

3. Providing Linux ARM binaries to test my disassembler with https://github.com/Inknyto/arm-binaries