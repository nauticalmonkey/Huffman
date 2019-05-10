#ifndef HENCODE_H
#define HENCODE_H
#define BYTE 8.0
#define MASK 1

#include <stdint.h>
#include <fcntl.h>
#include <math.h>
#include "Functions.h"


extern int AmountofUniqCares;
extern int numCodes;


typedef struct header
{
    uint8_t Care;
    uint32_t amount;

}Header;

Header *CreateHead(int *, int );
void CheackHead(Header *, int );
int HowManyBits(struct SearchingTable *);
int BWrite(char , int , struct SearchingTable* , uint8_t*);
void SafeFreeAll(Node *, struct SearchingTable *, int *, Header *);
void SafeFreeHeader(Header *);
#endif

