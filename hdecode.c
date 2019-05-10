#include "hdecode.h"
char *lnBuffer;

void SafeFreeAllD(int *amountTable , vertex *Tree, unsigned char *lnBuffer)
{
    if (amountTable != NULL)
        SafeFreeFreqTable(amountTable );
    if (Tree != NULL)
        SafeFreeTree(Tree);
    if (pbuff != NULL)
        safebufferFree(lnBuffer);
    return;
}

int decodeHeader(int fileIn, vertex **HenTree, int **amountTable )
{
    int AmountofUniqCares = 0;
    uint32_t i;
    uint32_t front = 0;
    uint8_t c;

    *amountTable = buildFTable();
    if ((read(fileIn, &i, sizeof(int))) <= 0)
    {
        fprintf(stderr, "error reading file\n");
        exit(-1);
    }

    AmountofUniqCares = (int)i;

    for (i = 0; i < AmountofUniqCares; i++)
    {
        if ((read(fileIn, &c, sizeof(uint8_t))) <= 0)
        {
            fprintf(stderr, "error reading file\n");
            exit(-1);
        }

        if ((read(fileIn, &front, sizeof(uint32_t))) <= 0)
        {
            fprintf(stderr, "error reading file\n");
            exit(-1);
        }

        amountTable[0][(unsigned char)c] = front;
    }

    if (*amountTable != NULL)
    {
        *HenTree = buildTree(*amountTable);
    }

    return AmountofUniqCares;
}

void decodeBody(int fileIn, int fileOut, int amounitOfTotalUniqCares, int numOfUniqueChars, vertex *HenTree, int *amountTable)
{

    int indexBuff;
    int i;
    int numCodes = 0;
    uint8_t mask = ENDMASK;

    vertex *root = HenTree;
    lnBuffer = readLongLine(fileIn);

    for (i = amounitOfTotalUniqCares, indexBuff = 0; i > 0 && HenTree != NULL; i--, HenTree = root)
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
        if (write(fileOut, &HenTree->c, sizeof(unsigned char)) <= 0)
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
                if (amountTable[i] > 0)
                    ch = i;
            }
        }
        while (amounitOfTotalUniqCares != 0)
        {
            if (write(fileOut, &ch, sizeof(unsigned char)) <= 0)
            {
                perror("write error\n");
            }
            amounitOfTotalUniqCares--;
        }
    }
}

void decodeFile(int fileIn, int fileOut, vertex **HenTree, int **amountTable)
{

    int AmountofUniqCares;
    int i;

    AmountofUniqCares = decodeHeader(fileIn, HenTree, amountTable);

    decodeBody(fileIn, fileOut, careTotal(*amountTable), AmountofUniqCares, *HenTree, *amountTable);
}

int careTotal(int *amountTable)
{
    int tot = 0;

    if (amountTable == NULL)
        return 0;

    int j;

    for (j = 0; j < CHARACTERAMOUNT; j++)
    {
        if (amountTable[j] > 0)
            tot += amountTable[j];
    }
    return tot;
}

int main(int argc, char *argv[])
{

    int fileIn, fileOut, inSavedFd, outSavedFd, *amountTable;
    vertex *root;

    int readStdIn = argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0);

    if (argc > 3)
    {
        fprintf(stderr, "usage: ./hdecode infile [ ( infile | - ) [ outfile ] ]\n");
        exit(-1);
    }

    if (!readStdIn)
    {
        if ((fileIn = open(argv[1], O_RDONLY)) == -1)
        {
            perror(argv[2]);
            exit(-1);
        }
        inSavedFd = dup(0);
        dup2(fileIn, 0);
    }

    if (argc == 3)
    {

        if ((fileOut = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0700)) == -1)
        {
            perror(argv[2]);
            exit(-1);
        }
        outSavedFd = dup(1);
        dup2(fileOut, 1);
    }

    decodeFile(0, 1, &root, &amountTable);

    if ((argc == 3 || argc == 2) && (strcmp(argv[1], "-")))
    {
        dup2(inSavedFd, 0);
        close(fileIn);
    }
    if (argc == 3)
    {
        dup2(outSavedFd, 1);
        close(fileOut);
    }

    SafeFreeAllD(amountTable, root, lnBuffer);

    return 0;
}