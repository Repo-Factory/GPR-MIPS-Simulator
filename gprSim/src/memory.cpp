/*
 *  @brief
 *          Define simple read/write operations for memory access
 */

#include "memory.hpp"

// Dereference address to get its contents
int32_t readContents(int32_t* address)
{
    return *address;
}

// Dereference address and write data to it
void writeContents(int32_t* address, const int32_t data)
{
    *address = data;
}