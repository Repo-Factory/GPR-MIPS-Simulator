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

#define SPEED_UP(IC, C) 8*IC/C
#define OUTPUT_FILE "result2.txt"
#define WRITE_MODE "w"
#define OUTPUT_MESSAGE "Instructions Executed: %d\nTotal Cycles: %d\nSpeed Up: %f"

#ifndef ASSEMBLY_PATH
    constexpr char* ASSEMBLY_PATH = "quadratic_accumulator.s";             // Default path
#endif

/* Load instructions into memory and execute instructions forever (will stop when END command reached) */
int main() {
    std::unique_ptr<Loader> loader = std::make_unique<Loader>();
    std::unique_ptr<MIPSCPU> cpu = std::make_unique<MIPSCPU>();
    loader->loadProgram(cpu->memory, ASSEMBLY_PATH);             // defined in CMakeLists.txt

    // int instructionsExecuted = 0;
    // int totalCycles = 0;
    // float speed_up = SPEED_UP(instructionsExecuted, totalCycles); 
    
    cpu->userMode = true;
    while (cpu->userMode) 
        executeInstruction(*cpu);                                // defined in cpu.cpp
    
    // FILE* output = popen(OUTPUT_FILE, WRITE_MODE);
    // fprintf(output, OUTPUT_MESSAGE, instructionsExecuted, totalCycles, speed_up);
    exit(EXIT_SUCCESS);
}
