/* These functions aren't called from program, they were used for troubleshooting purposes */

#include "debug_helpers.hpp"

std::ostream& operator<<(std::ostream& stream, std::map<std::string, void*>& table)
{
    for (const auto& [symbol, address] : table)
    {
        stream << symbol << ": " << address << std::endl;
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

void printBinary(int32_t num) 
{
    for (int i = 32 - 1; i >= 0; --i) {
        unsigned int mask = 1 << i;
        std::cout << ((num & mask) ? '1' : '0');
        if (i % 4 == 0) std::cout << ' ';
    }
    std::cout << std::endl;
}
