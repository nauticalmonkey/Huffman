#include "Functions.h"
int numUniqueChar = 0;
int Indexer = 0;

int *buildFreqTable()
{

    int *freq = (int *)malloc(ALPHABET_SIZE * sizeof(int));

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
        freq[i] = 0;

    return freq;
}

void insertToFreqTable(int **ft, unsigned char c)
{

    if (*ft != NULL)
    {
        if (ft[0][(unsigned char)c] == 0)
        {
            numUniqueChar++;
        }

        ft[0][(unsigned char)c]++;
    }
    return;
}

void printFreqTable(int *freqTable)
{
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (freqTable[i] > 0)
            printf("freqTable[ %c ] = %d\n", (unsigned char)i, freqTable[i]);
    }
}

void freeFreqTable(int *ft)
{
    if (ft != NULL)
        free(ft);
    return;
}

Node *buildHuffTree(int *freqTable)
{
    listNode *priorityQ = NULL;
    Node *root = NULL;
    int i;

    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (freqTable[i] > 0)
        {

            pushNewNode(&priorityQ, (unsigned char)i, freqTable[i]);
        }
    }

    if (priorityQ == NULL)
        return NULL;

    if (size(priorityQ) == 1)
    {
        poll(&priorityQ);
        return NULL;
    }

    while (size(priorityQ) > 1)
    {
        Node *left = poll(&priorityQ);
        Node *right = poll(&priorityQ);
        listNode *parent = newListNode('\0', (unsigned int)right->freq + (unsigned int)left->freq, left, right);
        pushNode(&priorityQ, parent);
    }

    root = poll(&priorityQ);
    numCodes = root->freq;

    return root;
}

void inorder(Node *root)
{
    if (root == NULL)
        return;

    inorder(root->left_child);
    printf("binary tree node with value char %c and freq %d\n", root->c, root->freq);
    inorder(root->right_child);
}

int isLeaf(Node *n)
{
    if (n->left_child == NULL && n->right_child == NULL)
        return TRUE;
    return FALSE;
}

void freeHuffmanTree(Node *tree)
{
    if (tree == NULL)
        return;
    else
    {
        freeHuffmanTree(tree->left_child);
        freeHuffmanTree(tree->right_child);
        free(tree);
    }
}

void initLookUpTable(Node *node, char *s, long top, struct lookUpTable **table)
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
        initLookUpTable(node->left_child, s, top + 1, table);
    }
    if (node->right_child)
    {
        s[top] = '1';
        initLookUpTable(node->right_child, s, top + 1, table);
    }
}

struct lookUpTable *buildLookUpTable(Node *root)
{
    struct lookUpTable *table = (struct lookUpTable *)calloc(ALPHABET_SIZE + 1, sizeof(struct lookUpTable));
    char *s = (char *)calloc(ALPHABET_SIZE, sizeof(char));
    initLookUpTable(root, s, 0, &table);
    free(s);
    return table;
}

void printLookUpTable(struct lookUpTable *table)
{
    if (table != NULL)
    {
        int i;
        for (i = 0; i < ALPHABET_SIZE; i++)
            if (table[i].code != NULL)
            {
                printf("0x%x: %s\n", i, table[i].code);
            }
    }
    return;
}

void freeLookUpTable(struct lookUpTable *table)
{
    if (table != NULL)
    {
        int i;
        for (i = 0; i < ALPHABET_SIZE; i++)
            if (table[i].code != NULL)
            {
                free(table[i].code);
            }
        free(table);
    }
    return;
}

Node *newNode(unsigned char c, int freq, Node *left, Node *right)
{
    Node *new;
    if ((new = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;

    new->freq = freq;
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

void structure(struct node *root, int level)
{

    if (root == NULL)
    {
        padding('\t', level);
        puts("~");
    }
    else
    {
        structure(root->right_child, level + 1);
        padding('\t', level);
        printf("%c\n", root->c);
        structure(root->left_child, level + 1);
    }
}

listNode *newListNode(unsigned char c, int freq, Node *left, Node *right)
{
    listNode *new = (listNode *)malloc(sizeof(listNode));

    new->curr = newNode(c, freq, left, right);

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

void pushNewNode(listNode **head, unsigned char c, int freq)
{

    listNode *beg = *head;
    listNode *tempNode;

    tempNode = newListNode(c, freq, NULL, NULL);

    if (!*head)
    {
        *head = tempNode;
        return;
    }

    if (beg->curr->freq > freq)
    {
        tempNode->next = *head;
        *head = tempNode;
    }
    else
    {

        while (beg->next != NULL && beg->next->curr->freq < freq)
        {
            beg = beg->next;
        }

        while (beg->next != NULL && beg->next->curr->freq == freq && beg->next->curr->c < c)
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

    if (beg->curr->freq > parent->curr->freq)
    {
        parent->next = *head;
        *head = parent;
    }

    else
    {

        while (beg->next != NULL && beg->next->curr->freq < parent->curr->freq)
        {
            beg = beg->next;
        }

        if (beg->next != NULL && beg->next->curr->freq == parent->curr->freq)
        {

            while (beg->next != NULL && beg->next->curr->freq == parent->curr->freq && beg->next->curr->c < parent->curr->c)
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
        temp->freq = peek(head)->freq;
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
        printf("node chacter %c and  %d\n", head->curr->c, head->curr->freq);
        head = head->next;
    }
    printf("node chacter %c and  %d\n", head->curr->c, head->curr->freq);
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

char *read_long_line(int inFd)
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

void freeBuffs(char *buff)
{
    if (buff != NULL)
        free(buff);
    return;
}

