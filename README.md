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

## Authors

[@Conner Sommerfield](https://github.com/Repo-Factory/) Github
