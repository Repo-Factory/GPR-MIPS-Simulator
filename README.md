# MIPS General Purpose Register Assembler/Simulator

This is a MIPS Assembler/Simulator that supports a small portion of the instruction set (10 key instructions)

## Description

This Assembler/Simulator is based around a MIPS 32-bit 32-register architecture. It will read in an assembly file given as a command line argument and will execute the instructions given (assuming the instructions are supported and correct)

## Directories ##

- `gprSim/`  
    - `include && src`
        - `loader`                      - Will read assembly source code and store binary instructions into memory
        - `processor`                   - Will read binary instructions from memory and execute them
        - `main.cpp`                    - Read here for more details on the loader/processor stages
    - `helpers`                         - Has some helper simple functions pertaining to file operations, arrays, and debugging
    - `CMakeLists.txt`                  - The build system which will compile and link our files

The loader will have a source_parser to tokenize the assembly code and an encoder to turn each token into a binary value. It will take every line of the source code and output a 32-bit instruction. It uses a two-pass system to create a symbol table in the first pass and then replaces the labels in the 2nd pass using the addresses it stored in the symbol table from the 1st pass. The assignment indicates to provide a partially assembled palindrome.s by manually assigning addresses, but I decided to do this dynamically in the code itself using the loader. If you define DEBUG in the CMakeLists you can see the printout of the binary instructions that are created along with the symbol table. I wanted to actually create a working assembler instead of doing it exactly how the assignment asked and having to manually write in addresses (this is way better). 

The processor will have a binary parser that takes those instructions and splits them up into immediates/registers/addresses/etc... The instructions.cpp and instructions.hpp files lays out the available opcodes and their syntax. The CPU will execute in a loop reading the next 32 bit instruction from memory (text/data section memory will have been populated already by the loader), parsing it, and performing the appropriate operations to carry out the program. It will perform these operations by using the 32 general purpose registers, jumping to appropriate addresses, and overall using the common mechanisms seen in MIPS / assembly languages. The switch statement specified as a requirement in the assignment can be seen in cpu.cpp. It will keep track of the cycles used for each functional unit during the program, and also outputs it to a file results2.txt which can be found in the build folder inside gprSim. 

## Getting Started

This project will need to be cloned or copied somehwere onto your computer. You will have to build it before execution

### Compiling/Linking

Clone the project and go into the gprSim directory. Run this command

    mkdir build && cd build && cmake .. && sudo make

This should populate the executable inside of a new directory called build

### Executing program

You can execute the program like any other executable and it will point automatically to palindrome.s

    ./gpr_exec 

In the CMakeLists.txt, you can also comment out line 37 "add_definitions(-DASSEMBLY_PATH="${SRC_DIR}/palindrome.s")"
After compiling again, the program will instead take a command line argument so you can execute any other assembly file

## Difficulties

The hardest part of this program is of course taking care of symbols and being able to corectly jump to addresses in memory, making sure when we jump we don't go to some incorrect random address and Seg Fault. I spent a lot of time figuring out how to correctly derive the addresses using either a PC relative address or an absolute address derived from a set base address. Syscalls were also important to get right as they have more potential to involve more complex operations than loading an immediate to a register or performing arithmetic on registers. Getting the symbols right makes the rest of the program not too complicated actually, as parsing the registers and loading values into them is somewhat trivial.

## Authors

[@Conner Sommerfield](https://github.com/Repo-Factory/) Github
