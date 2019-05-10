#ifndef HDECODE_H
#define HDECODE_H

#include "Functions.h"
#include <stdint.h>

#define ENDMASK 0x80

extern int AmountofUniqCares;

int decodeHeader(int , Node **, int **);
void decodeFile(int , int , Node **, int **);
void decodeBody(int , int , int , int , Node *, int *);
int careTotal(int *);

#endif
