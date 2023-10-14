/*******************************************************************************
 * @author Conner Sommerfield
 * @name accumulator
 * 
 * @details Reads instructions from assembly file and simulates execution 
 * 
 * Program does not require command line arguments but path to a valid text file must be defined in CMakeLists.txt 
 * 
 * Relevant entities
 * - CPU
 * - Memory
 * - Loader
 * - Helpers
 * See cpp file of each for more details in each respective area
 ******************************************************************************/

#include "cpu.hpp"
#include "loader.hpp"
#include "memory.hpp"
#include <memory>
#include <fstream>
#include <iostream>

#ifndef ASSEMBLY_PATH
    constexpr const char* ASSEMBLY_PATH = "palindrome.s";             // Default path
#endif

/* Load instructions into memory and execute instructions forever (will stop when END command reached) */
int main() {
    std::unique_ptr<Loader> loader = std::make_unique<Loader>();
    std::unique_ptr<MIPSCPU> cpu = std::make_unique<MIPSCPU>();
    loader->loadProgram(cpu->memory, ASSEMBLY_PATH);             // defined in CMakeLists.txt

    cpu->userMode = true;
    while (cpu->userMode) {
        executeInstruction(*cpu);                                // defined in cpu.cpp
    }

    exit(EXIT_SUCCESS);
}
