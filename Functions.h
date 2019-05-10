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

typedef struct node{
    unsigned char c;
    int amount;
    struct node *left_child;
    struct node *right_child;
}Node;

typedef struct nodeList{
    Node *curr;
    struct  nodeList *next;
}listNode;




int *buildFTable();
void insertFTable(int **ft, unsigned char c);
Node *buildTree(int *freqTable);
int isLeaf(Node *n);
void makeLTable(Node *node, char *s, long top ,struct SearchingTable **table);

struct SearchingTable *buildSearchTable(Node *root);
Node *newNode(unsigned char c, int freq, Node *left, Node *right);
void padding ( unsigned char ch, int n );
listNode *newListNode(unsigned char c, int freq, Node *left , Node *right);
int size(listNode *head);
void pushNewNode(listNode **head, unsigned char c, int freq);
void pushNode(listNode **head, listNode *parent);
void pop(listNode ** head);
Node *peek(listNode ** head);
Node *poll(listNode **head);
int isEmpty(listNode **head);
void transverse(listNode *head);
char *readLongLine(int inFd);
void safebufferFree(char *buff);
void SafeFreeLookTable(struct SearchingTable *table);
void SafeFreeTree(Node *tree);
void SafeFreeFreqTable(int *ft);
void checkFTable(int *ft);
void printLookUpTable(struct SearchingTable *table);
void printbincharpad(char c);



char *buff;
char *pbuff, *prev;
int numIDs;

#endif

