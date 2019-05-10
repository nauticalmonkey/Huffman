
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
    vertex *me;
    struct  vertexList *next;
}listvertex;




int *buildFTable();
void insertFTable(int **, unsigned char );
vertex *buildTree(int *);
int isLeaf(vertex *);
void makeLTable(vertex *, char *, long  ,struct SearchingTable **);
struct SearchingTable *buildSearchTable(vertex *);
vertex *newvertex(unsigned char , int , vertex *, vertex *);
listvertex *newListvertex(unsigned char , int , vertex * , vertex *);
int size(listvertex *);
void enqueueNewvertex(listvertex **, unsigned char, int );
void enqueueVertex(listvertex **, listvertex *);
void dequeue(listvertex ** );
vertex *peek(listvertex ** );
vertex *poll(listvertex **);
int isEmpty(listvertex **);
char *readLongLine(int );
void safebufferFree(char *);
void SafeFreeLookTable(struct SearchingTable *);
void SafeFreeTree(vertex *);
void SafeFreeFreqTable(int *);
void checkFTable(int *);
void checkLTable(struct SearchingTable *);



char *buff;
char *pbuff, *prev;
int numIDs;

