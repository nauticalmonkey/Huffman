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

            pushNewvertex(&queue, (unsigned char)i, freqTable[i]);
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
        pushvertex(&queue, parent);
    }

    root = poll(&queue);
    numCodes = root->amount;

    return root;
}

listvertex *newListvertex(unsigned char c, int amount, vertex *left, vertex *right)
{
    listvertex *new = (listvertex *)malloc(sizeof(listvertex));

    new->curr = newvertex(c, amount, left, right);

    new->next = NULL;
    return new;
}

int size(listvertex *head)
{
    if (head == NULL)
        return 0;
    else if (head->next == NULL)
        return 1;
    int count = 1;

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

void makeLTable(vertex *vertex, char *s, long top, struct SearchingTable **table)
{

    unsigned char c = (unsigned char)vertex->character;

    if (isLeaf(vertex))
    {
        s[top] = '\0';

        (*table)[c].id = (char *)calloc(strlen(s), sizeof(char));

        if (s != NULL)
        {
            strcpy((*table)[c].id, s);
        }
    }
    if (vertex->left_child)
    {
        s[top] = '0';
        makeLTable(vertex->left_child, s, top + 1, table);
    }
    if (vertex->right_child)
    {
        s[top] = '1';
        makeLTable(vertex->right_child, s, top + 1, table);
    }
}

  struct SearchingTable *buildSearchTable(vertex *root)
{
    struct SearchingTable *table = (struct SearchingTable *)calloc(CHARACTERAMOUNT + 1, sizeof(struct SearchingTable));
    char *s = (char *)calloc(CHARACTERAMOUNT, sizeof(char));
    makeLTable(root, s, 0, &table);
    free(s);
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

void padding(unsigned char ch, int n)
{
    int i;

    for (i = 0; i < n; i++)
        putchar(ch);
}


void pushNewvertex(listvertex **head, unsigned char c, int amount)
{

    listvertex *beg = *head;
    listvertex *tempvertex;

    tempvertex = newListvertex(c, amount, NULL, NULL);

    if (!*head)
    {
        *head = tempvertex;
        return;
    }

    if (beg->curr->amount > amount)
    {
        tempvertex->next = *head;
        *head = tempvertex;
    }
    else
    {

        while (beg->next != NULL && beg->next->curr->amount < amount)
        {
            beg = beg->next;
        }

        while (beg->next != NULL && beg->next->curr->amount == amount && beg->next->curr->character < c)
        {

            beg = beg->next;
        }
        tempvertex->next = beg->next;
        beg->next = tempvertex;
    }
}

void pushvertex(listvertex **head, listvertex *parent)
{

    listvertex *beg = *head;

    if (!*head)
    {
        *head = parent;
        return;
    }

    if (beg->curr->amount > parent->curr->amount)
    {
        parent->next = *head;
        *head = parent;
    }

    else
    {

        while (beg->next != NULL && beg->next->curr->amount < parent->curr->amount)
        {
            beg = beg->next;
        }

        if (beg->next != NULL && beg->next->curr->amount == parent->curr->amount)
        {

            while (beg->next != NULL && beg->next->curr->amount == parent->curr->amount && beg->next->curr->c < parent->curr->c)
            {

                beg = beg->next;
            }
        }

        parent->next = beg->next;
        beg->next = parent;
    }
}
void pop(listvertex **head)
{
    listvertex *temp = *head;

    (*head) = (*head)->next;

    free(temp->curr);
    free(temp);
}

vertex *peek(listvertex **head)
{
    return (*head)->curr;
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

        pop(head);
    }
    return temp;
}

int isEmpty(listvertex **head)
{
    return (*head) == NULL;
}

void transverse(listvertex *head)
{
    while (head->next != NULL)
    {
        printf("vertex chacter %c and  %d\n", head->curr->c, head->curr->amount);
        head = head->next;
    }
    printf("vertex chacter %c and  %d\n", head->curr->c, head->curr->amount);
}

void print_binary(char x)
{
    int b = 128;

    while (b != 0)
    {

        if (b <= x)
        {
            x -= b;
            printf("1");
        }
        else
            printf("0");

        b = b >> 1;
    }
}

void printbincharpad(char c)
{
    int i;
    for (i = 7; i >= 0; --i)
    {
        putchar((c & (1 << i)) ? '1' : '0');
    }
    putchar('\n');
}

char *readLongLine(int inFd)
{
    int sizebuff = BUFFERAMOUNT;
    char c;
    int onebuff = 0;

    pbuff = (char *)calloc(BUFFERAMOUNT, sizeof(char));

    while ((read(inFd, &c, sizeof(char))) > 0)
    {
        pbuff[Indexer++] = c;
        if (Indexer >= BUFFERAMOUNT - 1)
        {
            sizebuff = sizebuff + BUFFERAMOUNT;
            pbuff = (char *)realloc(pbuff, sizebuff);
        }
    }

    return pbuff;
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

void checkFTable(int *freqTable)
{
    int i;
    for (i = 0; i < CHARACTERAMOUNT; i++)
    {
        if (freqTable[i] > 0)
            printf("freqTable[ %c ] = %d\n", (unsigned char)i, freqTable[i]);
    }
}


void printLookUpTable(struct SearchingTable *table)
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
