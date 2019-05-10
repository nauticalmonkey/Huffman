#include "hdecode.h"

void SafeFreeAllD(int *ft, Node *Tree, unsigned char *buff)
{
    if (ft != NULL)
        SafeFreeFreqTable(ft);
    if (Tree != NULL)
        SafeFreeTree(Tree);
    if (pbuff != NULL)
        freeBuffs(buff);
    return;
}

int decodeHeader(int inFd, Node **huffmanTree, int **ft)
{
    int AmountofUniqCares;
    AmountofUniqCares = 0;

    uint32_t i;

    uint8_t c;

    *ft = buildFreqTable();
    if ((read(inFd, &i, sizeof(int))) <= 0)
    {
        perror("error reading file\n");
        exit(-1);
    }

    AmountofUniqCares = (int)i;

    uint32_t ft_adder;
    ft_adder = 0;
    for (i = 0; i < AmountofUniqCares; i++)
    {
        if ((read(inFd, &c, sizeof(uint8_t))) <= 0)
        {
            perror("error reading file\n");
            exit(-1);
        }

        if ((read(inFd, &ft_adder, sizeof(uint32_t))) <= 0)
        {
            perror("error reading file\n");
            exit(-1);
        }

        ft[0][(unsigned char)c] = ft_adder;
    }

    if (*ft != NULL)
    {
        *huffmanTree = buildHuffTree(*ft);
    }

    return AmountofUniqCares;
}

void decodeBody(int inFd, int outFd, int numTotalChars, int numOfUniqueChars, Node *huffmanTree, int *ft)
{

    int indexBuff;

    Node *root = huffmanTree;

    buff = read_long_line(inFd);

    uint8_t mask;
    mask = MASK_MSB;

    int i;

    int numCodes;
    numCodes = 0;

    for (i = numTotalChars, indexBuff = 0; i > 0 && huffmanTree != NULL; i--, huffmanTree = root)
    {

        while (!isLeaf(huffmanTree))
        {

            if ((buff[indexBuff] & mask) == 0)
            {
                huffmanTree = huffmanTree->left_child;
            }
            else
            {
                huffmanTree = huffmanTree->right_child;
            }

            if ((mask = mask >> 1) == 0)
            {
                indexBuff++;
                mask = MASK_MSB;
            }
        }
        if (write(outFd, &huffmanTree->c, sizeof(unsigned char)) <= 0)
            perror("write error\n");
    }

    if (huffmanTree == NULL)
    {
        int i;
        char ch;
        if (numOfUniqueChars == 1)
        {
            for (i = 0; i < ALPHABET_SIZE; i++)
            {
                if (ft[i] > 0)
                    ch = i;
            }
        }
        while (numTotalChars != 0)
        {
            if (write(outFd, &ch, sizeof(unsigned char)) <= 0)
            {
                perror("write error\n");
            }
            numTotalChars--;
        }
    }
}

void decodeFile(int inFd, int outFd, Node **huffmanTree, int **ft)
{

    int numUniqueChars;
    numUniqueChars = decodeHeader(inFd, huffmanTree, ft);
    int i;

    decodeBody(inFd, outFd, totChars(*ft), numUniqueChars, *huffmanTree, *ft);
}

int totChars(int *ft)
{
    int tot = 0;

    if (ft == NULL)
        return 0;

    int j;

    for (j = 0; j < ALPHABET_SIZE; j++)
    {
        if (ft[j] > 0)
            tot += ft[j];
    }
    return tot;
}

int main(int argc, char *argv[])
{

    int inFd, outFd, inSavedFd, outSavedFd, *ft;
    Node *root;

    int readStdIn = argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0);

    if (argc > 3)
    {
        fprintf(stderr, "usage: ./hdecode infile [ ( infile | - ) [ outfile ] ]\n");
        exit(-1);
    }

    if (!readStdIn)
    {
        if ((inFd = open(argv[1], O_RDONLY)) == -1)
        {
            perror(argv[2]);
            exit(-1);
        }
        inSavedFd = dup(0);
        dup2(inFd, 0);
    }

    if (argc == 3)
    {

        if ((outFd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0700)) == -1)
        {
            perror(argv[2]);
            exit(-1);
        }
        outSavedFd = dup(1);
        dup2(outFd, 1);
    }

    decodeFile(0, 1, &root, &ft);

    if ((argc == 3 || argc == 2) && (strcmp(argv[1], "-")))
    {
        dup2(inSavedFd, 0);
        close(inFd);
    }
    if (argc == 3)
    {
        dup2(outSavedFd, 1);
        close(outFd);
    }

    SafeFreeAllD(ft, root, buff);

    return 0;
}