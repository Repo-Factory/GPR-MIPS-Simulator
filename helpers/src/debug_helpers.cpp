/* These functions aren't called from program, they were used for troubleshooting purposes */

#include "debug_helpers.hpp"

std::ostream& operator<<(std::ostream& stream, std::map<std::string, int32_t*>& table)
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

