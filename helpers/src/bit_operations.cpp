/* 
 * Helpers to convert items in source code to bit stream 
 * What this means is we have something like PUSH
 * We want to turn push into an int32_t type
 * We can take the chars and overlay the bits into a int32_t type using logic OR masks
 * This makes it so our simulator only deals with int32_t types
 */

#include "bit_operations.hpp"

#define INIT_BIT_STREAM 0
#define STRING_END_CHAR '\0'
#define LABEL_END_CHAR ':'
#define EIGHT_BITS 8

/* For each char of word, OR mask the 8 bits with a 32 bit integer and shift 8 bits (accomodates up to 4-letter words) */
int32_t stringToBitStream(const std::string& word)
{
    const char* c_word = word.c_str();
    int32_t bitStream = INIT_BIT_STREAM;
    while (*c_word != STRING_END_CHAR) {
        bitStream = (bitStream << EIGHT_BITS) | static_cast<int8_t>(*c_word++);
    }
    return bitStream;
}

/* Same idea here but we stop at a colon so we don't save the symbol with colon into symbol table (just the symbol) */
int32_t labelToBitStream(const std::string& label)
{
    const char* c_label = label.c_str();
    int32_t bitStream = INIT_BIT_STREAM;
    while (*c_label != LABEL_END_CHAR) {
        bitStream = (bitStream << EIGHT_BITS) | static_cast<int8_t>(*c_label++);
    }
    return bitStream;
}
