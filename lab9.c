#include <stdio.h>
#include <stdlib.h> //for malloc and free

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// Fill out this structure
struct HashType
{
    struct RecordType* pNode;
    struct HashType* pNext;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
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
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("\nIndex %d: ", i);
        struct HashType* pNode = pHashArray[i].pNext;
        while (pNode != NULL)
        {
            printf("%d, %c, %d -> ", pNode->pNode->id, pNode->pNode->name, pNode->pNode->order);
            pNode = pNode->pNext;
        }
    }
}

int main(void)
{
    int i, j, hashSize = 12;
    struct RecordType* pRecords;
    int recordSize = 0;

    recordSize = parseData("/home/cam/CLionProjects/Lab-9/input.txt", &pRecords);
    printRecords(pRecords, recordSize);

    // Your hash implementation
    struct HashType* pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * hashSize);
    for (i = 0; i < hashSize; ++i)
    {
        pHashArray[i].pNext = NULL;
    }

    for (i = 0; i < recordSize; ++i)
    {
        int hashValue = hash(pRecords[i].id, hashSize);
        struct HashType* pHash = &pHashArray[hashValue];
        struct HashType* pNewNode = (struct HashType*) malloc(sizeof(struct HashType));
        if (pNewNode == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        pNewNode->pNode = &pRecords[i];
        pNewNode->pNext = NULL;

        if (pHash->pNext == NULL)
        {
            pHash->pNext = pNewNode;
        }
        else
        {
            struct HashType* pNode = pHash->pNext;
            while (pNode->pNext != NULL)
            {
                pNode = pNode->pNext;
            }

            pNode->pNext = pNewNode;
        }
    }

    displayRecordsInHash(pHashArray, hashSize);

    for (i = 0; i < hashSize; ++i)
    {
        struct HashType* pNode = pHashArray[i].pNext;
        while (pNode != NULL)
        {
            struct HashType* pNext = pNode->pNext;
            free(pNode);
            pNode = pNext;
        }
    }

    free(pHashArray);
    free(pRecords);

    return 0;
}