#include "Functions.h"



/*buildFrequencyTable: takes in a string and relates
 *
 */
int numUniqueChar = 0;
int Indexer = 0;

int *buildFreqTable()
{
    /*make a freq table for each character that we are encoding*/
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
            /*creat a new head to pqueue*/

            pushNewNode(&priorityQ, (unsigned char)i, freqTable[i]);
        }
    }

    /*====================Special cases ===============================*/
    /*case 1 - if numUniqueChar == 0*/
    if (priorityQ == NULL)
        return NULL;

    /*Case 2 - if numUniqueChar == 1*/
    if (size(priorityQ) == 1)
    {
        poll(&priorityQ);
        return NULL;
    }
    /*=================================================================*/

    /*while there is more than one character in the pque*/
    while (size(priorityQ) > 1)
    {
        Node *left = poll(&priorityQ);
        Node *right = poll(&priorityQ);
        /*wrap parent in a listNode*/
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
        /*free each character corresonding to that code*/
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
        /*free each character corresonding to that code*/
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
    /*single case return empty*/
    if (head == NULL)
        return 0;
    else if (head->next == NULL)
        return 1;

    /*count  has to be at least 1 by time to get here*/
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

    listNode *beg = *head; /*get address of first head*/

    /*new node*/
    listNode *tempNode;

    tempNode = newListNode(c, freq, NULL, NULL);

    /*case 1 - list is emtpy*/
    if (!*head)
    {
        *head = tempNode;
        return;
    }

    /*case 2 - the data to be inserted has less freq than the head*/
    if (beg->curr->freq > freq)
    {
        tempNode->next = *head;
        *head = tempNode;
    }
    /*case 3 - general case (it goes through the list and finds a spot)*/
    else
    {
        /*Transverse till the tend of the list and on the way look for a node in
         * the list that has a frequency greater than that to be inserted*/

        while (beg->next != NULL && beg->next->curr->freq < freq)
        {
            beg = beg->next;
        }
        /* At this point beg should be holding a lnode where its next is equal or greater
         * than the frequency node to be inserted*/

        /* case 3.1 - could be at the end of the list*/

        /* case 3.2 - that next node could possibly be equal to the one node to be inserted*/

        /*case 3.2.1 - because beg->next->curr->freq == freq */
        /* we need to insert based upon character */

        /*Tranverse the list until a node is found where they are not equal*/
        while (beg->next != NULL && beg->next->curr->freq == freq && beg->next->curr->c < c)
        {
            /*If the soon to be entered character is less than c next*/
            /*we found the location for new node to be inserted*/
            beg = beg->next;
        }

        /*At this poostion soon to inserted freq should be less than or equal to beg*/
        tempNode->next = beg->next;
        beg->next = tempNode;
    }
}

void pushNode(listNode **head, listNode *parent)
{

    listNode *beg = *head; /*get address of first head*/

    /*case 1 - list is emtpy*/
    if (!*head)
    {
        *head = parent;
        return;
    }

    /*case 2 - the data to be inserted has less freq than the head*/
    if (beg->curr->freq > parent->curr->freq)
    {
        parent->next = *head;
        *head = parent;
    }

    /*case 3 - general case (it goes through the list and finds a spot)*/
    else
    {
        /*Transverse till the tend of the list and on the way look for a node in
         * the list that has a frequency greater than that to be inserted*/

        while (beg->next != NULL && beg->next->curr->freq < parent->curr->freq)
        {
            beg = beg->next;
        }
        /* At this point beg should be holding a lnode where its next is equal or greater
         * than the frequency node to be inserted*/

        /* case 3.1 - could be at the end of the list*/

        /* case 3.2 - that next node could possibly be equal to the one node to be inserted*/

        if (beg->next != NULL && beg->next->curr->freq == parent->curr->freq)
        {
            /*case 3.2.1 - because beg->next->curr->freq == freq */
            /* we need to insert based upon character */

            /*Tranverse the list until a node is found where they are not equal*/
            while (beg->next != NULL && beg->next->curr->freq == parent->curr->freq && beg->next->curr->c < parent->curr->c)
            {
                /*If the soon to be entered character is less than c next*/
                /*we found the location for new node to be inserted*/
                beg = beg->next;
            }
        }

        /*At this poostion soon to inserted freq should be less than or equal to beg*/
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

/*removes head and retreives it*/
Node *poll(listNode **head)
{
    Node *temp = (Node *)malloc(sizeof(Node));

    if (*head == NULL)
        return NULL;
    else
    {
        /*copy contents of what is head before pop*/
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



void print_binary(char x) {
    int b = 128;

    while (b != 0) {

        if (b <= x) {
            x -= b;
            printf("1");
        } else
            printf("0");

        b = b >> 1;
    }
}
void printbincharpad(char c)
{
    int i;
    for (i = 7; i >= 0; --i)
    {
        putchar( (c & (1 << i)) ? '1' : '0' );
    }
    putchar('\n');
}


char *read_long_line(int inFd)
{
	int sizebuff = MAXCHAR;
	char c; /*temp is for moving allong pbuff*/

	/*initalize current size of buffer*/	
	int onebuff = 0;

	/*case 2: reading the whole file */
	pbuff = (char*)calloc(MAXCHAR, sizeof(char));

	while((read(inFd, &c, sizeof(char))) > 0)
	{
	   /* print_binary(c);
	    printf(" above is %c \n", c);
	    */
		/*case 3: store value of c in buffer */
		pbuff[Indexer++] = c;
		/*case 4: check if an overflow occured in buffer same buff
		 *		count number of overflows in buff */
		if ( Indexer >= MAXCHAR-1 )
		{
			/*Size alloationa are alwaysgoing to be mutliples os MAXCHAr*/
			sizebuff = sizebuff + MAXCHAR;
			pbuff = (char*)realloc(pbuff,sizebuff);
		}
	}

	return pbuff;
}

void freeBuffs(char *buff)
{
    if(buff != NULL)
        free(buff);
    return;
}



