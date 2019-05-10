#include "hdecode.h"
char *lnBuffer;

void SafeFreeAllD(int *ft, Node *Tree, unsigned char *lnBuffer)
{
    if (ft != NULL)
        SafeFreeFreqTable(ft);
    if (Tree != NULL)
        SafeFreeTree(Tree);
    if (pbuff != NULL)
        safebufferFree(lnBuffer);
    return;
}

int decodeHeader(int inFd, Node **HenTree, int **ft)
{
    int AmountofUniqCares = 0;
    uint32_t i;
    uint32_t front = 0;
    uint8_t c;

    *ft = buildFTable();
    if ((read(inFd, &i, sizeof(int))) <= 0)
    {
        fprintf(stderr, "error reading file\n");
        exit(-1);
    }

    AmountofUniqCares = (int)i;

    for (i = 0; i < AmountofUniqCares; i++)
    {
        if ((read(inFd, &c, sizeof(uint8_t))) <= 0)
        {
            fprintf(stderr, "error reading file\n");
            exit(-1);
        }

        if ((read(inFd, &front, sizeof(uint32_t))) <= 0)
        {
            fprintf(stderr, "error reading file\n");
            exit(-1);
        }

        ft[0][(unsigned char)c] = front;
    }

    if (*ft != NULL)
    {
        *HenTree = buildTree(*ft);
    }

    return AmountofUniqCares;
}

void decodeBody(int inFd, int outFd, int numberoftCares, int numOfUniqueChars, Node *HenTree, int *ft)
{

    int indexBuff;
    int i;
    int numCodes = 0;
    uint8_t mask = ENDMASK;

    Node *root = HenTree;
    lnBuffer = readLongLine(inFd);

    for (i = numberoftCares, indexBuff = 0; i > 0 && HenTree != NULL; i--, HenTree = root)
    {

        while (!isLeaf(HenTree))
        {

            if ((lnBuffer[indexBuff] & mask) == 0)
            {
                HenTree = HenTree->left_child;
            }
            else
            {
                HenTree = HenTree->right_child;
            }

            if ((mask = mask >> 1) == 0)
            {
                indexBuff++;
                mask = ENDMASK;
            }
        }
        if (write(outFd, &HenTree->c, sizeof(unsigned char)) <= 0)
            perror("write error\n");
    }

    if (HenTree == NULL)
    {
        int i;
        char ch;
        if (numOfUniqueChars == 1)
        {
            for (i = 0; i < CHARACTERAMOUNT; i++)
            {
                if (ft[i] > 0)
                    ch = i;
            }
        }
        while (numberoftCares != 0)
        {
            if (write(outFd, &ch, sizeof(unsigned char)) <= 0)
            {
                perror("write error\n");
            }
            numberoftCares--;
        }
    }
}

void decodeFile(int inFd, int outFd, Node **HenTree, int **ft)
{

    int AmountofUniqCares;
    int i;

    AmountofUniqCares = decodeHeader(inFd, HenTree, ft);

    decodeBody(inFd, outFd, totChars(*ft), AmountofUniqCares, *HenTree, *ft);
}

int totChars(int *ft)
{
    int tot = 0;

    if (ft == NULL)
        return 0;

    int j;

    for (j = 0; j < CHARACTERAMOUNT; j++)
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

    SafeFreeAllD(ft, root, lnBuffer);

    return 0;
}