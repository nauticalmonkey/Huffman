#include "hdecode.h"


void freeEveryThing(int *ft, Node *Tree, unsigned char *buff)
{
    if(ft != NULL)
        freeFreqTable(ft);
    if(Tree != NULL)
        freeHuffmanTree(Tree);
    if(pbuff != NULL)
        freeBuffs(buff);
    return;
}

int decodeHeader(int inFd, Node **huffmanTree, int **ft)
{
    int numUniqueOfChars;
    numUniqueOfChars = 0;

    uint32_t i;

    uint8_t c;

    *ft = buildFreqTable();

   /* Step 1 - read the number of unique characters*/
    if((read(inFd, &i, sizeof(int))) <= 0)
    {
        perror("error reading file\n"); /*1st argument permission denied*/
        exit(-1);
    }

    numUniqueOfChars = (int)i;
   /*Step 2 - read in each character store in freqTable
    * if step 1 suceeded the file should be at the first char
    * */
    uint32_t ft_adder;
    ft_adder=0;
   /*read in sets of 5 bytes*/
    for (i= 0; i < numUniqueOfChars; i++) {

        /*Step 3 -  read the character*/
        if((read(inFd, &c, sizeof(uint8_t))) <= 0)
        {
            perror("error reading file\n"); /*1st argument permission denied*/
            exit(-1);
        }

        /* Step 4 - read the corresponding frequency of that character*/
        if((read(inFd, &ft_adder, sizeof(uint32_t))) <= 0)
        {
            perror("error reading file\n"); /*1st argument permission denied*/
            exit(-1);
        }

        /*printf("error for: %c think %d\n", c , ft_adder);*/
        ft[0][(unsigned char)c] = ft_adder;

    }
    /*printFreqTable(*ft);*/

   /*step 5 - build huffmanTree from freqTable*/
   if(*ft != NULL) {
       *huffmanTree = buildHuffTree(*ft);
       /*if(*huffmanTree == NULL)*/
   }


   return numUniqueOfChars;

}

/*decode left to right reading body 1000 0000 = 0x80*/

/*Call after header, so left off at the first part of the body*/
void decodeBody(int inFd, int outFd, int numTotalChars,int numOfUniqueChars,  Node *huffmanTree, int *ft) {

    /*Step 1 - read in the file from where left off
     *
     *         Decode while reading keep track of number of chars read
     * */
    /*initalize the buffer*/


    int indexBuff;

    Node *root = huffmanTree;


    buff = read_long_line(inFd);

    uint8_t mask;
    mask = MASK_MSB;

    int i;

    int numCodes;
    numCodes = 0;


    /*Step 2 - go through the buffer - EOF indicator is for this buffer is '\0'*/
    for (i = numTotalChars, indexBuff = 0; i > 0 && huffmanTree != NULL; i--, huffmanTree = root) {
        /*Step 3 - transverse the tree until a char is found.
         *
         * - shift that current that one encodeod char till find a matching
         * - if leaf found jump out of the loop , start from root
         *
         * */

        /*  printf("buff[ %d ] =  %x\n", indexBuff, (uint8_t)buff[indexBuff]);;*/

        while (!isLeaf(huffmanTree)) {
            /*mask from left to right is x000 1110, is x a 1 or 0? */
            if ((buff[indexBuff] & mask) == 0) {
                huffmanTree = huffmanTree->left_child;
            } else {
                huffmanTree = huffmanTree->right_child;
            }
            /*shift that buffer encoded char that is one byte 0001 _ 1110 buff[indexBuff]*/


            /*if the numCodes seen in a char is 8 then move on to the next byte*/
            if ((mask = mask >> 1) == 0) {
                indexBuff++;
                mask = MASK_MSB;
            }

        }
        /*Step 4 - after reading converting the code to characters write it out*/
        if (write(outFd, &huffmanTree->c, sizeof(unsigned char)) <= 0)
            perror("write error\n");

    }
    /*=============================CASE of one unique char*==================*/
    if (huffmanTree == NULL) {
        int i;
        char ch;
        if (numOfUniqueChars == 1) {
            for (i = 0; i < ALPHABET_SIZE; i++) {
                if (ft[i] > 0)
                    ch = i;
            }
        }
        while (numTotalChars != 0) {
            if (write(outFd, &ch, sizeof(unsigned char)) <= 0) {
                perror("write error\n");
            }
            numTotalChars--;
        }
    }
    /*================================================================*/


}




void decodeFile(int inFd, int outFd, Node **huffmanTree, int **ft)
{

    int numUniqueChars ;
    numUniqueChars =  decodeHeader(inFd, huffmanTree, ft);
    int i;

    /*we need to write decoded msg to the outfile*/
    decodeBody(inFd,outFd,totChars(*ft),numUniqueChars, *huffmanTree, *ft);


}

int totChars(int *ft)
{
    int tot = 0;

    if(ft == NULL)
        return 0;

    int j;

    for (j = 0; j < ALPHABET_SIZE; j++) {
        if (ft[j] > 0)
            tot += ft[j];
    }
    return tot;
}


int main(int argc, char *argv[])
{

    int inFd, outFd, inSavedFd, outSavedFd, *ft;
    Node *root;



    int readStdIn = argc == 1 || (argc == 2 && strcmp(argv[1] ,"-") == 0);

    /*Case 1 - if there is no arguments then read from stdin
     *          also output to stdout*/



    /*Case 1 - First look if the right format*/
    if(argc > 3){
        fprintf(stderr, "usage: ./hdecode infile [ ( infile | - ) [ outfile ] ]\n");
        exit(-1);
    }


    /*===================Check if should where should read in =============================*/
    /*Case 2 -  Check to see if read from file
     * Assumed : After the right Format*/


    if(!readStdIn) /*Dont read from std in*/
    {
        if ((inFd = open(argv[1], O_RDONLY)) == -1) {
            perror(argv[2]); /*1st argument permission denied*/
            exit(-1);
        }
        /*save std in */
        inSavedFd = dup(0);
        dup2(inFd, 0);
    }

   /*=========================================================================================*/



   /*===================Check if should where should write from =============================*/
  /*Case 3 - check to if write to file
   * Assumed : After the right Format
   * */
         if(argc == 3)
         {
             /*If right amount of arguments continue and check if we can open that file */
             if ((outFd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0700)) == -1) {
                 perror(argv[2]); /*1st argument permission denied*/
                 exit(-1);
             } /*read from stdout*/
          /*save std in */
         outSavedFd = dup(1);
         dup2(outFd, 1);

         }
/*=========================================================================================*/


    decodeFile(0,1, &root, &ft);

    /*If there is a infile switch back to and close that file*/
    if ((argc == 3 || argc == 2) && (strcmp(argv[1],"-")) )
    {
        dup2(inSavedFd, 0);
        close(inFd);
    }
    if(argc == 3)
    {
        dup2(outSavedFd, 1);
        close(outFd);
    }

    freeEveryThing(ft, root, buff);

    return 0;
}