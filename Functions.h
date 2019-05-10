#ifndef FREQTABLE
#define FREQTABLE

#define ALPHABET_SIZE 256
#define BUFFERAMOUNT 1200 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
/*#include <unistd.h>*/

struct lookUpTable{
        char *code;
};

typedef struct node{
    unsigned char c;
    int freq;
    struct node *left_child;
    struct node *right_child;
}Node;

typedef struct nodeList{
    Node *curr;
    struct  nodeList *next;
}listNode;


typedef enum boolean{FALSE,TRUE} bool;
int *buildFreqTable();
void insertToFreqTable(int **ft, unsigned char c);
void printFreqTable(int *ft);
void freeFreqTable(int *ft);
Node *buildHuffTree(int *freqTable);
void inorder(Node *root);
int isLeaf(Node *n);
void freeHuffmanTree(Node *tree);
void initLookUpTable(Node *node, char *s, long top ,struct lookUpTable **table);
struct lookUpTable *buildLookUpTable(Node *root);
void printLookUpTable(struct lookUpTable *table);
void freeLookUpTable(struct lookUpTable *table);
Node *newNode(unsigned char c, int freq, Node *left, Node *right);
void padding ( unsigned char ch, int n );
void structure ( struct node *root, int level );
listNode *newListNode(unsigned char c, int freq, Node *left , Node *right);
int size(listNode *head);
void pushNewNode(listNode **head, unsigned char c, int freq);
void pushNode(listNode **head, listNode *parent);
void pop(listNode ** head);
Node *peek(listNode ** head);
Node *poll(listNode **head);
int isEmpty(listNode **head);
void transverse(listNode *head);
char *read_long_line(int inFd);
void freeBuffs(char *buff);
void printbincharpad(char c);



char *buff;
char *pbuff, *prev;
int numCodes;

#endif

