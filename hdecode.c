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
    uint32_t i, front = 0;
    uint8_t c;

    *amountTable = buildFTable();
    if ((read(fileIn, &i, sizeof(int))) <= 0)
    {
        fprintf(stderr, "there was an error while reading file\n");
        exit(-1);
    }

    AmountofUniqCares = (int)i;

    for (i = 0; i < AmountofUniqCares; i++)
    {
        if ((read(fileIn, &c, sizeof(uint8_t))) <= 0)
        {
            fprintf(stderr, "there was an error while reading file\n");
            exit(-1);
        }

        if ((read(fileIn, &front, sizeof(uint32_t))) <= 0)
        {
            fprintf(stderr, "there was an error while reading file\n");
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

void decodeText(int fileIn, int fileOut, int amounitOfTotalUniqCares, int numOfUniqueChars, vertex *HenTree, int *amountTable)
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
        if (write(fileOut, &HenTree->character, sizeof(unsigned char)) <= 0)
            fprintf("no write for you\n");
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
                fprintf("write error\n");
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

    decodeText(fileIn, fileOut, careTotal(*amountTable), AmountofUniqCares, *HenTree, *amountTable);
}

int careTotal(int *amountTable)
{
    int Total = 0;

    if (amountTable == NULL)
        return 0;

    int j;

    for (j = 0; j < CHARACTERAMOUNT; j++)
    {
        if (amountTable[j] > 0)
            Total += amountTable[j];
    }
    return Total;
}

int main(int argc, char *argv[])
{

    int fileIn, fileOut, savedinFile, savedOutFile, *amountTable;
    vertex *root;

    int standardin = (argc == 1 || (argc == 2 && strcmp(argv[1], "-") == 0));

    if (argc > 3)
    {
        fprintf(stderr, "usage: %s Too many args you foo ]\n",argv[0]);
        exit(-1);
    }

    if (!standardin)
    {
        if ((fileIn = open(argv[1], O_RDONLY)) == -1)
        {
            fprintf(stderr,"%s ther was an error", argv[2]);
            exit(-1);
        }
        savedinFile = dup(0);
        dup2(fileIn, 0);
    }

    if (argc == 3)
    {

        if ((fileOut = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
        {
            fprintf(stderr,"%s ther was an error",argv[2]);
            exit(-1);
        }
        
        savedOutFile = dup(1);
        dup2(fileOut, 1);
    }

    decodeFile(0, 1, &root, &amountTable);

    if ((argc == 3 || argc == 2) && (strcmp(argv[1], "-")))
    {
        dup2(savedinFile, 0);
        close(fileIn);
    }
    if (argc == 3)
    {
        dup2(savedOutFile, 1);
        close(fileOut);
    }

    SafeFreeAllD(amountTable, root, lnBuffer);

    return 0;
}