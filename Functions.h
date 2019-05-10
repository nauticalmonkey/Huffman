#ifndef FREQTABLE
#define FREQTABLE

#define CHARACTERAMOUNT 256
#define BUFFERAMOUNT 1200 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

typedef enum boolean{FALSE,TRUE} bool;

struct SearchingTable{
        char *id;
};

typedef struct vertex{
    unsigned char character;
    int amount;
    struct vertex *left_child;
    struct vertex *right_child;
}vertex;

typedef struct vertexList{
    vertex *curr;
    struct  vertexList *next;
}listvertex;




int *buildFTable();
void insertFTable(int **ft, unsigned char c);
vertex *buildTree(int *freqTable);
int isLeaf(vertex *n);
void makeLTable(vertex *vertex, char *s, long top ,struct SearchingTable **table);

struct SearchingTable *buildSearchTable(vertex *root);
vertex *newvertex(unsigned char c, int freq, vertex *left, vertex *right);
void padding ( unsigned char ch, int n );
listvertex *newListvertex(unsigned char c, int freq, vertex *left , vertex *right);
int size(listvertex *head);
void pushNewvertex(listvertex **head, unsigned char c, int freq);
void pushvertex(listvertex **head, listvertex *parent);
void pop(listvertex ** head);
vertex *peek(listvertex ** head);
vertex *poll(listvertex **head);
int isEmpty(listvertex **head);
void transverse(listvertex *head);
char *readLongLine(int inFd);
void safebufferFree(char *buff);
void SafeFreeLookTable(struct SearchingTable *table);
void SafeFreeTree(vertex *tree);
void SafeFreeFreqTable(int *ft);
void checkFTable(int *ft);
void printLookUpTable(struct SearchingTable *table);
void printbincharpad(char c);



char *buff;
char *pbuff, *prev;
int numIDs;

#endif

