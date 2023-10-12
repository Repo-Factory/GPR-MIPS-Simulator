/* 
 * @brief instructions does something cool
 *
 * more
 * more
 * more
 */

#include "instructions.hpp"
#include "cpu.hpp"
#include <cstdint>
#include <map>
#include <algorithm>
#include <iostream>

using Name = std::string;
using Opcode = int32_t;
using Register = int32_t;

std::string toUpper(const std::string& str)
{
    std::string copy = str;
    std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
    return copy;
}

std::string removeWhitespace(const std::string& str) 
{
    std::string copy = str;
    copy.erase(std::remove_if(copy.begin(), copy.end(), ::isspace), copy.end());
    return copy;
}

std::map<Name, Opcode> getOpcodeTable()
{
    return std::map<Name, Opcode>
    {
        {"ADDI",    static_cast<int32_t>(Identifier::ADDI)   },
        {"B",       static_cast<int32_t>(Identifier::B)      },
        {"BEQZ",    static_cast<int32_t>(Identifier::BEQZ)   },
        {"BGE",     static_cast<int32_t>(Identifier::BGE)    },
        {"BNE",     static_cast<int32_t>(Identifier::BNE)    },
        {"LA",      static_cast<int32_t>(Identifier::LA)     },
        {"LB",      static_cast<int32_t>(Identifier::LB)     },
        {"LI",      static_cast<int32_t>(Identifier::LI)     },
        {"SUBI",    static_cast<int32_t>(Identifier::SUBI)   },
        {"SYSCALL", static_cast<int32_t>(Identifier::SYSCALL)} 
    };
}

bool OpcodeTable::searchTable(const std::string& token)
{
    std::map<Name, Opcode> opcodeTable = getOpcodeTable();
    auto it = opcodeTable.find(toUpper(token));
    if (it != opcodeTable.end()) {
        return true;
    }
    return false;
}

Opcode OpcodeTable::getOpcode(const std::string& token)
{
    std::map<Name, Opcode> opcodeTable = getOpcodeTable();
    return opcodeTable[toUpper(token)]; 
}

std::map<Name, Register> getRegisterTable()
{
    return std::map<Name, Register>
    {
        {"$zero", 0},
        {"$at",   1},
        {"$v0",   2},
        {"$v1",   3},
        {"$a0",   4},
        {"$a1",   5},
        {"$a2",   6},
        {"$a3",   7},
        {"$t0",   8},
        {"$t1",   9},
        {"$t2",   10},
        {"$t3",   11},
        {"$t4",   12},
        {"$t5",   13},
        {"$t6",   14},
        {"$t7",   15},
        {"$s0",   16},
        {"$s1",   17},
        {"$s2",   18},
        {"$s3",   19},
        {"$s4",   20},
        {"$s5",   21},
        {"$s6",   22},
        {"$s7",   23},
        {"$t8",   24},
        {"$t9",   25},
        {"$k0",   26},
        {"$k1",   27},
        {"$gp",   28},
        {"$sp",   29},
        {"$fp",   30},
        {"$ra",   31},
    };
}

Register RegisterTable::getRegister(const std::string& token)
{
    std::map<Name, Register> registerTable = getRegisterTable();
    return registerTable[token]; 
}

bool RegisterTable::searchTable(const std::string& token)
{
    std::map<Name, Register> registerTable = getRegisterTable();
    return registerTable.find(token) != registerTable.end();
}