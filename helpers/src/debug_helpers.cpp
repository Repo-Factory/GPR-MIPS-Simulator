/* These functions aren't called from program, they were used for troubleshooting purposes */

#include "debug_helpers.hpp"

std::ostream& operator<<(std::ostream& stream, std::map<int32_t, int32_t*>& table)
{
    for (const auto& [symbol, address] : table)
    {
        stream << (char)symbol << " " << *address << std::endl;
    }
    return stream;
}

std::string int32ToAscii(int32_t intValue) 
{
    std::string ascii;
    for (int i = 0; i < 4; ++i) {
        char byte = static_cast<char>((intValue >> (i * 8)) & 0xFF);
        ascii = byte + ascii;
    }
    return ascii;
}

void debugPrint(Memory& memory)
{
    for (int i = 0; i < 256; ++i) {
        std::cout << "Memory[" << i << "]: " << int32ToAscii(*((int32_t*)&memory + i)) << std::endl;
    }
    for (int i = 0; i < 256; ++i) {
        std::cout << "Memory[" << i << "]: " <<  *((int32_t*)&memory + i) << std::endl;
    }
    std::cout << memory.symbol_table;
}