#ifndef HENCODE_H
#define HENCODE_H



#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "Functions.h"

#define BUFSIZE 210
#define ALPHABET_SIZE 256
#define BYTE 8.0
#define MASK 1

extern int numUniqueChar;
extern int numCodes;


typedef struct headerBits
{
    uint8_t character;
    uint32_t frequency; /*number of chars in freq table*/

}fieldHeader;

fieldHeader *generateHeader(int *ft, int numUniqueChars);
void printFieldHeader(fieldHeader *header, int numUniqueChars);
void freeHeader(fieldHeader *header);
int writeBits(char c, int lenCode, uint8_t *byte, struct lookUpTable *codeTable);
void freeEveryThing(Node *huffmanTree, struct lookUpTable *table, int *freqTable, fieldHeader *header);
int numBitsOfCode(struct lookUpTable *table);

#endif
