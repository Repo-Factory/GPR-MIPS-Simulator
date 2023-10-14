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

int sumArray(const int* array, const int size)
{
    int sum = 0;
    for (int i=0; i<size; i++) {
        sum += *(array + i);
    }
    return sum;
}          

// Add one array on top of another
void overlayArrays(int* targetArray, const int* sourceArray, const int size)
{
    for (int i=0; i<size; i++) {
        *(targetArray + i) += *(sourceArray + i);
    }
}

void forEachItem(const int* array, const int size, std::function<void(int, int)> action)
{
    for (int i=0; i<size; i++) {
        action(*(array+i), i);
    }
}

// void debugPrint(Memory& memory)
// {
//     for (int i = 0; i < 256; ++i) {
//         std::cout << "Memory[" << i << "]: "; printBinary(*((int32_t*)&memory + i));
//     }
//     int32_t* ptr = (int32_t*)&memory+5648;
//     std::cout << *(std::string*)ptr << std::endl;
//     std::cout << *(std::string*)memory.symbol_table["is_palin_msg"] << std::endl;
//     std::cout << memory.symbol_table;
// }
