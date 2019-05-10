#ifndef HDECODE_H
#define HDECODE_H

#include "Functions.h"
#include <stdint.h>

#define RD_MODE 0444
#define MASK_MSB 0x80

extern char *buff;
extern int numUniqueChar;

int decodeHeader(int inFd, Node **huffmanTree, int **ft);
void decodeBody(int inFd, int outFd, int numTotalChars, int numUniqueChar, Node *huffmanTree, int *ft);
int totChars(int *ft);
void decodeFile(int inFd, int outFd, Node **huffmanTree, int **ft);

#endif
