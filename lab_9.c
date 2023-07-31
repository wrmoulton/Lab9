#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// LinkedList node
struct LinkedList
{
    struct RecordType record;
    struct LinkedList* next;
};

// Hash table
struct HashType
{
    struct LinkedList* chains;
};

// Compute the hash function
int hash(int x, int sz)
{
    return x % sz;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        struct LinkedList* current = pHashArray->chains[i].next;

        // Check if the index is occupied with any records
        if (current != NULL)
        {
            printf("Index %d ->", i);

            while (current != NULL)
            {
                printf(" %d %c %d ->", current->record.id, current->record.name, current->record.order);
                current = current->next;
            }

            printf(" NULL\n");
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    // Read data from the file and store in pRecords array
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    int hashSz = 15; // Assuming the hash size is 15 as mentioned in the rubric

    // Create a hash array of size hashSz and initialize linked lists
    struct HashType hashArray;
    hashArray.chains = (struct LinkedList*)malloc(sizeof(struct LinkedList) * hashSz);
    for (int i = 0; i < hashSz; i++)
    {
        hashArray.chains[i].next = NULL;
    }

    // Fill the hash array using the records from pRecords with separate chaining
    for (int i = 0; i < recordSz; i++)
    {
        int index = hash(pRecords[i].id, hashSz);

        // Create a new node
        struct LinkedList* newNode = (struct LinkedList*)malloc(sizeof(struct LinkedList));
        newNode->record = pRecords[i];
        newNode->next = hashArray.chains[index].next;
        hashArray.chains[index].next = newNode;
    }

    // Display records in the hash structure
    displayRecordsInHash(&hashArray, hashSz);

    // Don't forget to free dynamically allocated memory
    free(pRecords);
    for (int i = 0; i < hashSz; i++)
    {
        struct LinkedList* current = hashArray.chains[i].next;
        while (current != NULL)
        {
            struct LinkedList* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashArray.chains);

    return 0;
}

