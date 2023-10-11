/* 
 * @brief instructions does something cool
 *
 * more
 * more
 * more
 */

#include "instructions.hpp"
#include <cstdint>
#include <map>

using Name = const char*;
using Opcode = int32_t;

std::map<Name, Opcode> opcodeTable
{
    {"ADDI",    static_cast<int32_t>(Identifier::ADDI)   },
    {"B",       static_cast<int32_t>(Identifier::B)      },
    {"BEQZ",    static_cast<int32_t>(Identifier::BEQZ)   },
    {"MUL",     static_cast<int32_t>(Identifier::BGE)    },
    {"BNE",     static_cast<int32_t>(Identifier::BNE)    },
    {"LA",      static_cast<int32_t>(Identifier::LA)     },
    {"LB",      static_cast<int32_t>(Identifier::LB)     },
    {"LI",      static_cast<int32_t>(Identifier::LI)     },
    {"SUBI",    static_cast<int32_t>(Identifier::SUBI)   },
    {"SYSCALL", static_cast<int32_t>(Identifier::SYSCALL)}
};