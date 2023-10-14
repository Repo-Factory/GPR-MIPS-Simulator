/*******************************************************************************
 * @author Conner Sommerfield
 * @name gprSim
 * 
 * @details Reads instructions from assembly file and simulates execution 
 * 
 * Program does not require command line arguments but path to a valid text file must be defined in CMakeLists.txt 
 * 
 * Relevant entities     - Program can be thought of in two major stages; loading and executing
 * - Loader
 *      - Source Parser - Parse assembly 
 *      - Encoder       - Encode instructions as binary
 *      - Symbol Table  - Handle labels
 * - Processor
 *      - Memory        - Simple memory access functions
 *      - Instructions  - Defines specification for opcodes
 *      - Binary Parser - Splits binary commands into useable pieces
 *      - CPU           - Simulates actions using registers
 * - Helpers
 * See cpp file of each for more details in each respective area
 ******************************************************************************/

#include "cpu.hpp"
#include "loader.hpp"
#include <memory>

/* Load instructions into memory and execute instructions forever (will stop when sycall called with appropriate argument) */
int main(int argc, char* argv[]) {
    #ifndef ASSEMBLY_PATH
        const char* ASSEMBLY_PATH = argv[1];
    #endif

    std::unique_ptr<Loader> loader = std::make_unique<Loader>();
    std::unique_ptr<MIPSCPU> cpu = std::make_unique<MIPSCPU>();
    loader->loadProgram(cpu->memory, ASSEMBLY_PATH);                   // First Command line arg will be assembly path

    cpu->userMode = true;
    while (cpu->userMode) {
        executeInstruction(*cpu);                                // defined in cpu.cpp
    }

    exit(EXIT_SUCCESS);
}
