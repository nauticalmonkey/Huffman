#ifndef HDECODE_H
#define HDECODE_H

#include "Functions.h"
#include <stdint.h>

#define ENDMASK 0x80

extern int AmountofUniqCares;

int decodeHeader(int , vertex **, int **);
void decodeFile(int , int , vertex **, int **);
void decodeText(int , int , int , int , vertex *, int *);
int careTotal(int *);

#endif
