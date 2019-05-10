#ifndef HENCODE_H
#define HENCODE_H



#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "Functions.h"

#define ALPHABET_SIZE 256
#define BYTE 8.0
#define MASK 1

extern int AmountofUniqCares;
extern int numCodes;


typedef struct headerBits
{
    uint8_t character;
    uint32_t frequency; /*number of chars in freq table*/

}Header;

Header *CreateHead(int *ft, int AmountofUniqCares);
void CheackHead(Header *header, int AmountofUniqCares);
int HowManyBits(struct SearchingTable *table);
int BWrite(char c, int lenCode, uint8_t *byte, struct SearchingTable *codeTable);
void SafeFreeAll(Node *huffmanTree, struct SearchingTable *table, int *freqTable, Header *header);
void SafeFreeHeader(Header *header);
#endif

