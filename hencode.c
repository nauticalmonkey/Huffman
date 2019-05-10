#include "hencode.h"


int HowManyBits(struct SearchingTable *table)
{
    int AmountB = 0;
    int j;
    if (table != NULL)
        for (j = 0; j < CHARACTERAMOUNT; ++j)
            if (table[j].id != NULL)
            {
                AmountB += strlen(table[j].id);
                printf("character: %c,   code length : %ld , for %s\n", j, strlen(table[j].id), table[j].id);
            }

    return AmountB;
}

Header *CreateHead(int *ft, int AmountofUniqCares)
{
    Header *header = NULL;
    int i;
    int next = 0;
    header = (Header *)calloc(AmountofUniqCares, sizeof(Header));

    for (i = 0; i < CHARACTERAMOUNT; i++)
    {
        if (ft[i] > 0)
        {
            header[(unsigned char)next].amount = (uint32_t)ft[i];
            header[(unsigned char)next].Care = (uint8_t)i;
            next++;
        }
    }
    return header;
}

int BWrite(char c, int lenCode, struct SearchingTable *IdTable, uint8_t *eightbit)
{
    int static aBits = BYTE;
    int i;
    char *temp;

    if (lenCode == 0)
        return 0;

    temp = IdTable[(unsigned char)c].id;
    for (i = 0; i < lenCode; i++)
    {

        *eightbit = *eightbit << 1;

        *eightbit = *eightbit | (*temp & MASK);

        temp++;
        aBits--;

        if (aBits == 0)
        {
            write(1, eightbit, sizeof(uint8_t));
            aBits = BYTE;
            *eightbit = 0;
        }
    }
    return aBits;
}

void CheackHead(Header *header, int AmountofUniqCares)
{
    int i;
    for (i = 0; i < AmountofUniqCares; ++i)
    {
        printf("| %c | %u |", (char)header[i].Care, header[i].amount);
    }
}

void SafeFreeHeader(Header *header)
{
    if (header != NULL)
        free(header);
    return;
}

void SafeFreeAll(vertex *huffmanTree, struct SearchingTable *table, int *freqTable, Header *header)
{
    SafeFreeHeader(header);
    SafeFreeFreqTable(freqTable);
    SafeFreeLookTable(table);
    SafeFreeTree(huffmanTree);
}

int main(int argc, char *argv[])
{

    int inFd, outputFile, i, ByteChecker, aOutput, *ft = NULL, bitermcbiter = 1;
    unsigned char c;
    struct SearchingTable *IdTable = NULL;
    vertex *head = NULL;
    Header *header = NULL, *startingHead = NULL;
    uint8_t output;
    uint32_t CharactersInForest, nuthin=0;

    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "usage: %s infile, outfile\n", argv[0]);
        exit(-1);
    }
    if ((inFd = open(argv[1], O_RDONLY)) == -1)
    {
        perror(argv[1]);
        exit(-1);
    }

    ft = buildFTable();

    while (read(inFd, &c, sizeof(unsigned char)))
    {
        insertFTable(&ft, c);
    }

    if (AmountofUniqCares >= 2)
    {

        head = buildTree(ft);
        if ((IdTable = buildSearchTable(head)) == NULL)
            exit(-1);
    }

    /*printLookUpTable(IdTable);*/

    if (argc == 3)
    {

        if ((outputFile = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0700)) == -1)
        {
            perror(argv[2]);
            exit(-1);
        }

        aOutput = dup(1);
        dup2(outputFile, 1);
    }

    CharactersInForest = (uint32_t)AmountofUniqCares;

    if (AmountofUniqCares > 0)
    {
        header = startingHead = CreateHead(ft, CharactersInForest);

        for (i = 0; i < CharactersInForest + 1 && bitermcbiter > 0; i++)
        {
            if (i == 0)
                bitermcbiter = write(1, &CharactersInForest, sizeof(uint32_t));
            else
            {
                bitermcbiter = write(1, &header->Care, sizeof(char));
                bitermcbiter = write(1, &header->amount, sizeof(int));
                header++;
            }
        }
    }

    if (AmountofUniqCares > 1)
    {

        lseek(inFd, 0, 0);

        while (read(inFd, &c, sizeof(uint8_t)) > 0)
        {

            ByteChecker = BWrite(c, strlen(IdTable[(unsigned)c].id), IdTable, &output);
        }

        if (ByteChecker != BYTE)
        {

            output = output << ByteChecker;
            write(1, &output, sizeof(uint8_t));
        }
    }

    if (AmountofUniqCares == 0)
    {
        /*write(1, &nuthin, sizeof(uint32_t));*/
    }

    SafeFreeAll(head, IdTable, ft, startingHead);

    if (argc == 3)
    {
        dup2(aOutput, 1);
        close(outputFile);
    }

    return 0;
}
