#pragma once
#include <stdint.h>
#include <map>
#include <string>

#define USER_TEXT_SIZE 128
#define KERNEL_TEXT_SIZE  64
#define USER_DATA_SIZE 128
#define KERNEL_DATA_SIZE 64

/* Lays out 4 sections of memory
 * Each section can be accessed by its corresponding pointer field
 * Symbol table will map labels like C: to its corresponding address where the value is held
 */
struct Memory 
{
    int32_t userText[USER_TEXT_SIZE] = {0};
    int32_t kernelText[KERNEL_TEXT_SIZE] = {0};
    int32_t userData[USER_DATA_SIZE] = {0};
    int32_t kernelData[KERNEL_DATA_SIZE] = {0};
    int32_t* userTextPtr = userText;
    int32_t* kernelDataPtr = kernelData;
    int32_t* kernelTextPtr = kernelText;
    int32_t* userDataPtr = userData;
    std::map<int32_t, int32_t*> symbol_table;           // Map symbol bit stream to address
};

int32_t readContents(int32_t* address);
void writeContents(int32_t* address, const int32_t data);