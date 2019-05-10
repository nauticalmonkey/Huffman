#include "hencode.h"

fieldHeader *generateHeader(int *ft, int AmountofUniqCaress)
{
    fieldHeader *header = NULL;
    int i;
    int header_inc = 0;

    header = (fieldHeader *)calloc(AmountofUniqCaress, sizeof(fieldHeader));

    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (ft[i] > 0)
        {
            header[(unsigned char)header_inc].frequency = (uint32_t)ft[i];
            header[(unsigned char)header_inc].character = (uint8_t)i;
            header_inc++;
        }
    }
    return header;
}

void printFieldHeader(fieldHeader *header, int AmountofUniqCaress)
{
    int i;
    for (i = 0; i < AmountofUniqCaress; ++i)
    {
        printf("| %c | %u |", (char)header[i].character, header[i].frequency);
    }
}

void freeHeader(fieldHeader *header)
{
    if (header != NULL)
        free(header);
    return;
}

int writeBits(char c, int lenCode, uint8_t *byte, struct lookUpTable *codeTable)
{
    int static bits_Left_to_write = BYTE;
    char *temp;
    int i;

    if (lenCode == 0)
        return 0;

    temp = codeTable[(unsigned char)c].code;
    for (i = 0; i < lenCode; i++)
    {

        *byte = *byte << 1;

        *byte = *byte | (*temp & MASK);

        temp++;
        bits_Left_to_write--;

        if (bits_Left_to_write == 0)
        {
            write(1, byte, sizeof(uint8_t));
            bits_Left_to_write = BYTE;
            *byte = 0;
        }
    }
    return bits_Left_to_write;
}

void freeEveryThing(Node *huffmanTree, struct lookUpTable *table, int *freqTable, fieldHeader *header)
{
    freeHeader(header);
    freeFreqTable(freqTable);
    freeLookUpTable(table);
    freeHuffmanTree(huffmanTree);
}

int numBitsOfCode(struct lookUpTable *table)
{
    int numBits = 0;
    int j;
    if (table != NULL)
        for (j = 0; j < ALPHABET_SIZE; ++j)
            if (table[j].code != NULL)
            {
                numBits += strlen(table[j].code);
                printf("character: %c,   code length : %ld , for %s\n", j, strlen(table[j].code), table[j].code);
            }

    return numBits;
}

int main(int argc, char *argv[])
{

    int inFd, outFd, outSavedFd, *ft = NULL;
    unsigned char c;
    Node *head = NULL;
    struct lookUpTable *codeTable = NULL;
    uint32_t CharactersInForest;
    fieldHeader *header = NULL, *baseHeader = NULL;
    uint8_t output;
    uint32_t output_for_nothing = 0;
    int divisablity_by_8;
    int i, bitermcbiter= 1;

    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "usage: ./hencode infile [ outfile ]\n");
        exit(-1);
    }

    if ((inFd = open(argv[1], O_RDONLY)) == -1)
    {
        perror(argv[1]);
        exit(-1);
    }

    ft = buildFreqTable();

    while (read(inFd, &c, sizeof(unsigned char)))
    {
        insertToFreqTable(&ft, c);
    }

    if (AmountofUniqCares >= 2)
    {

        head = buildHuffTree(ft);
        if ((codeTable = buildLookUpTable(head)) == NULL)
            exit(-1);
    }

    printLookUpTable(codeTable);

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

    CharactersInForest = (uint32_t)AmountofUniqCares;

    if (AmountofUniqCares > 0)
    {
        header = baseHeader = generateHeader(ft, CharactersInForest);

        for (i = 0; i < CharactersInForest + 1 && bitermcbiter> 0; i++)
        {
            if (i == 0)
                bitermcbiter= write(1, &CharactersInForest, sizeof(uint32_t));
            else
            {
                bitermcbiter= write(1, &header->character, sizeof(char));
                bitermcbiter= write(1, &header->frequency, sizeof(int));
                header++;
            }
        }
    }

    if (AmountofUniqCares > 1)
    {

        lseek(inFd, 0, 0);

        while (read(inFd, &c, sizeof(uint8_t)) > 0)
        {

            divisablity_by_8 = writeBits(c, strlen(codeTable[(unsigned)c].code), &output, codeTable);
        }

        if (divisablity_by_8 != BYTE)
        {

            output = output << divisablity_by_8;
            write(1, &output, sizeof(uint8_t));
        }
    }

    if (AmountofUniqCares == 0)
    {
        write(1, &output_for_nothing, sizeof(uint32_t));
    }

    freeEveryThing(head, codeTable, ft, baseHeader);

    if (argc == 3)
    {
        dup2(outSavedFd, 1);
        close(outFd);
    }

    return 0;
}
