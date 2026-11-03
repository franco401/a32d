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

# Features
* Currently reads curl's ELF header data, jumps to its e_entry offset, and reads/decodes the first few instructions. Only implemented first stage of decoding instruction (getting which instruction category so far such as branch, data processing, etc.)