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

Node *buildTree(int *freqTable)
{
    listNode *queue = NULL;
    Node *root = NULL;
    int i;

    for (i = 0; i < CHARACTERAMOUNT; i++)
    {
        if (freqTable[i] > 0)
        {

            pushNewNode(&queue, (unsigned char)i, freqTable[i]);
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
        Node *left = poll(&queue);
        Node *right = poll(&queue);
        listNode *parent = newListNode('\0', (unsigned int)right->amount + (unsigned int)left->amount, left, right);
        pushNode(&queue, parent);
    }

    root = poll(&queue);
    numCodes = root->amount;

    return root;
}


int isLeaf(Node *n)
{
    if (n->left_child == NULL && n->right_child == NULL)
        return TRUE;
    return FALSE;
}

void makeLTable(Node *node, char *s, long top, struct SearchingTable **table)
{

    unsigned char c = (unsigned char)node->c;

    if (isLeaf(node))
    {
        s[top] = '\0';

        (*table)[c].code = (char *)calloc(strlen(s), sizeof(char));

        if (s != NULL)
        {
            strcpy((*table)[c].code, s);
        }
    }
    if (node->left_child)
    {
        s[top] = '0';
        makeLTable(node->left_child, s, top + 1, table);
    }
    if (node->right_child)
    {
        s[top] = '1';
        makeLTable(node->right_child, s, top + 1, table);
    }
}

  struct SearchingTable *buildSearchTable(Node *root)
{
    struct SearchingTable *table = (struct SearchingTable *)calloc(CHARACTERAMOUNT + 1, sizeof(struct SearchingTable));
    char *s = (char *)calloc(CHARACTERAMOUNT, sizeof(char));
    makeLTable(root, s, 0, &table);
    free(s);
    return table;
}


Node *newNode(unsigned char c, int amount, Node *left, Node *right)
{
    Node *new;
    if ((new = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;

    new->amount = amount;
    new->c = c;

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


listNode *newListNode(unsigned char c, int amount, Node *left, Node *right)
{
    listNode *new = (listNode *)malloc(sizeof(listNode));

    new->curr = newNode(c, amount, left, right);

    new->next = NULL;
    return new;
}

int size(listNode *head)
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

void pushNewNode(listNode **head, unsigned char c, int amount)
{

    listNode *beg = *head;
    listNode *tempNode;

    tempNode = newListNode(c, amount, NULL, NULL);

    if (!*head)
    {
        *head = tempNode;
        return;
    }

    if (beg->curr->amount > amount)
    {
        tempNode->next = *head;
        *head = tempNode;
    }
    else
    {

        while (beg->next != NULL && beg->next->curr->amount < amount)
        {
            beg = beg->next;
        }

        while (beg->next != NULL && beg->next->curr->amount == amount && beg->next->curr->c < c)
        {

            beg = beg->next;
        }
        tempNode->next = beg->next;
        beg->next = tempNode;
    }
}

void pushNode(listNode **head, listNode *parent)
{

    listNode *beg = *head;

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
void pop(listNode **head)
{
    listNode *temp = *head;

    (*head) = (*head)->next;

    free(temp->curr);
    free(temp);
}

Node *peek(listNode **head)
{
    return (*head)->curr;
}

Node *poll(listNode **head)
{
    Node *temp = (Node *)malloc(sizeof(Node));

    if (*head == NULL)
        return NULL;
    else
    {
        temp->c = peek(head)->c;
        temp->amount = peek(head)->amount;
        temp->right_child = peek(head)->right_child;
        temp->left_child = peek(head)->left_child;

        pop(head);
    }
    return temp;
}

int isEmpty(listNode **head)
{
    return (*head) == NULL;
}

void transverse(listNode *head)
{
    while (head->next != NULL)
    {
        printf("node chacter %c and  %d\n", head->curr->c, head->curr->amount);
        head = head->next;
    }
    printf("node chacter %c and  %d\n", head->curr->c, head->curr->amount);
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
            if (table[i].code != NULL)
            {
                free(table[i].code);
            }
        free(table);
    }
    return;
}

void SafeFreeTree(Node *tree)
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
            if (table[i].code != NULL)
            {
                printf("0x%x: %s\n", i, table[i].code);
            }
    }
    return;
}
