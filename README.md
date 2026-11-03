# a32d (ARM32 Disassembler)
A small, work in progress, prototype Linux diassembler for ARM32 / ARMv7 ELF binaries I'm building for fun in C.

# Setup and install

1. Build the program
```
make a32d
```

2. Launch it (using 32-bit ARM version of curl as example)
```
./a32d ./binaries/curl
```

# Features (implemented so far)
* Currently reads curl's ELF header data, jumps to its e_entry offset, and reads/decodes the first few instructions. Only implemented first stage of decoding instruction (getting which instruction category so far such as branch, data processing, etc.) according to page 192 of ARM Architecture Reference Manual ARMv7-A and ARMv7-R ediiton)

# Helpful resources so far
1. (Wikipedia page for ELF binary structure)https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

2. (ARMv7 ASM Manual) https://support.arm.com/documentation/ddi0406/latest/

3. (Providing Linux ARM binaries to test my disassembler with) https://github.com/Inknyto/arm-binaries