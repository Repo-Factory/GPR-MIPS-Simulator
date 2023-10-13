#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include <string>
#include "memory.hpp"

struct OffsetInstruction
{
    int32_t integer_portion;
    int32_t register_portion;
};

struct OffsetInfo
{
    std::string integer_portion_str;
    int register_portion_index;
};

namespace Encoder
{
    int32_t encodeOpCode(const std::string& token);
    int32_t encodeRegister(const std::string& token);
    int32_t encodeImmediate(const std::string& token);
    int32_t encodeOffset(const std::string& token);
    int32_t encodeLabel(const std::string& token, Memory& memory, int32_t* LOCCTR);
}

#endif
