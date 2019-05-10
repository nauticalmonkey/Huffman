#include "Functions.h"
int AmountofUniqCares = 0;
int Indexer = 0;

int *buildFTable()
{

    int *amount = (int *)malloc(CHARACTERAMOUNT * sizeof(int));

    int i;
    for (i = 0; i < CHARACTERAMOUNT; i++)
        amount[i] = 0;

    return amount;
}

void insertFTable(int **ft, unsigned char c)
{

    if (*ft != NULL)
    {
        if (ft[0][(unsigned char)c] == 0)
        {
            AmountofUniqCares++;
        }

        ft[0][(unsigned char)c]++;
    }
    return;
}

vertex *buildTree(int *freqTable)
{
    listvertex *queue = NULL;
    vertex *root = NULL;
    int i;

    for (i = 0; i < CHARACTERAMOUNT; i++)
    {
        if (freqTable[i] > 0)
        {

            enqueueNewvertex(&queue, (unsigned char)i, freqTable[i]);
        }
    }

    if (queue == NULL)
        return NULL;

    if (size(queue) == 1)
    {
        poll(&queue);
        return NULL;
    }

    while (size(queue) > 1)
    {
        vertex *left = poll(&queue);
        vertex *right = poll(&queue);
        listvertex *parent = newListvertex('\0', (unsigned int)right->amount + (unsigned int)left->amount, left, right);
        enqueueVertex(&queue, parent);
    }

    root = poll(&queue);
    numIDs = root->amount;

    return root;
}

listvertex *newListvertex(unsigned char c, int amount, vertex *left, vertex *right)
{
    listvertex *new = (listvertex *)malloc(sizeof(listvertex));

    new->me = newvertex(c, amount, left, right);

    new->next = NULL;
    return new;
}

int size(listvertex *head)
{
    int count = 1;
    if (head == NULL){
        return 0;
    }
    else if (head->next == NULL){
        return 1;
    }

    while (head->next)
    {
        head = head->next;
        count++;
    }
    return count;
}




int isLeaf(vertex *n)
{
    if (n->left_child == NULL && n->right_child == NULL)
        return TRUE;
    return FALSE;
}

void enqueueVertex(listvertex **head, listvertex *parent)
{

    listvertex *rover = *head;

    if (!*head)
    {
        *head = parent;
        return;
    }

    if (rover->me->amount > parent->me->amount)
    {
        parent->next = *head;
        *head = parent;
    }

    else
    {

        while (rover->next != NULL && rover->next->me->amount < parent->me->amount)
        {
            rover = rover->next;
        }

        if (rover->next != NULL && rover->next->me->amount == parent->me->amount)
        {

            while (rover->next != NULL && rover->next->me->amount == parent->me->amount && rover->next->me->character < parent->me->character)
            {

                rover = rover->next;
            }
        }

        parent->next = rover->next;
        rover->next = parent;
    }
}


void makeLTable(vertex *vertex, char *j, long top, struct SearchingTable **table)
{

    unsigned char c = (unsigned char)vertex->character;

    if (isLeaf(vertex))
    {
        j[top] = '\0';

        (*table)[c].id = (char *)calloc(strlen(j), sizeof(char));

        if (j != NULL && (*table)[c].id != NULL)
        {
            strcpy((*table)[c].id, j);
        }
    }
    if (vertex->left_child)
    {
        j[top] = '0';
        makeLTable(vertex->left_child, j, top + 1, table);
    }
    if (vertex->right_child)
    {
        j[top] = '1';
        makeLTable(vertex->right_child, j, top + 1, table);
    }
}

  struct SearchingTable *buildSearchTable(vertex *root)
{
    struct SearchingTable *table = (struct SearchingTable *)calloc(CHARACTERAMOUNT + 1, sizeof(struct SearchingTable));
    char *j = (char *)calloc(CHARACTERAMOUNT, sizeof(char));
    makeLTable(root, j, 0, &table);
    free(j);
    return table;
}


vertex *newvertex(unsigned char character, int amount, vertex *left, vertex *right)
{
    vertex *new;
    if ((new = (vertex *)malloc(sizeof(vertex))) == NULL)
        return NULL;

    new->amount = amount;
    new->character = character;

    new->left_child = left;
    new->right_child = right;

    return new;
}



void enqueueNewvertex(listvertex **head, unsigned char c, int amount)
{

    listvertex *rover = *head;
    listvertex *tempvertex;

    tempvertex = newListvertex(c, amount, NULL, NULL);

    if (!*head)
    {
        *head = tempvertex;
        return;
    }

    if (rover->me->amount > amount)
    {
        tempvertex->next = *head;
        *head = tempvertex;
    }
    else
    {

        while (rover->next != NULL && rover->next->me->amount < amount)
        {
            rover = rover->next;
        }

        while (rover->next != NULL && rover->next->me->amount == amount && rover->next->me->character < c)
        {

            rover = rover->next;
        }
        tempvertex->next = rover->next;
        rover->next = tempvertex;
    }
}

vertex *peek(listvertex **head)
{
    return (*head)->me;
}


void dequeue(listvertex **head)
{
    listvertex *temp = *head;

    (*head) = (*head)->next;

    free(temp->me);
    free(temp);
}



int isEmpty(listvertex **head)
{
    return (*head) == NULL;
}


char *readLongLine(int inFd)
{
    int sizebuff = BUFFERAMOUNT;
    char c;

    partal = (char *)calloc(BUFFERAMOUNT, sizeof(char));

    while ((read(inFd, &c, sizeof(char))) > 0)
    {
        partal[Indexer++] = c;
        if (Indexer >= BUFFERAMOUNT - 1)
        {
            sizebuff = sizebuff + BUFFERAMOUNT;
            partal = (char *)realloc(partal, sizebuff);
        }
    }

    return partal;
}


vertex *poll(listvertex **head)
{
    vertex *temp = (vertex *)malloc(sizeof(vertex));

    if (*head == NULL)
        return NULL;
    else
    {
        temp->character = peek(head)->character;
        temp->amount = peek(head)->amount;
        temp->right_child = peek(head)->right_child;
        temp->left_child = peek(head)->left_child;

        dequeue(head);
    }
    return temp;
}

void safebufferFree(char *buff)
{
    if (buff != NULL)
        free(buff);
    return;
}

void SafeFreeFreqTable(int *ft)
{
    if (ft != NULL)
        free(ft);
    return;
}

void SafeFreeLookTable(struct SearchingTable *table)
{
    if (table != NULL)
    {
        int i;
        for (i = 0; i < CHARACTERAMOUNT; i++)
            if (table[i].id != NULL)
            {
                free(table[i].id);
            }
        free(table);
    }
    return;
}

void SafeFreeTree(vertex *tree)
{
    if (tree == NULL)
        return;
    else
    {
        SafeFreeTree(tree->left_child);
        SafeFreeTree(tree->right_child);
        free(tree);
    }
}

void checkFTable(int *Table)
{
    int i;
    for (i = 0; i < CHARACTERAMOUNT; i++)
    {
        if (Table[i] > 0)
            printf("Table[ %c ] = %d\n", (unsigned char)i, Table[i]);
    }
}


void checkLTable(struct SearchingTable *table)
{
    if (table != NULL)
    {
        int i;
        for (i = 0; i < CHARACTERAMOUNT; i++)
            if (table[i].id != NULL)
            {
                printf("0x%x: %s\n", i, table[i].id);
            }
    }
    return;
}
