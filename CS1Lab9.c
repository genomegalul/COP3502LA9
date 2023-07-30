#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
};

// Fill out this structure
struct HashType
{
	struct RecordType** hashArray;
	int size;
};

// Compute the hash function
int hash(int x)
{
	int hashSz = 31; // Adding this since I probably shouldn't modify the signature
	return x % hashSz;
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
	int i, j;

	for (i = 0; i < hashSz; ++i)
	{
		if (pHashArray->hashArray[i] != NULL)
		{
			printf("index %d -> ", i);
			printf("%d, %c, %d", pHashArray->hashArray[i]->id, pHashArray->hashArray[i]->name, pHashArray->hashArray[i]->order);

			for (j = i + 1; j < hashSz; ++j)
			{
				if (pHashArray->hashArray[j] != NULL && hash(pHashArray->hashArray[j]->id, hashSz) == i)
				{
					printf(" -> %d, %c, %d", pHashArray->hashArray[j]->id, pHashArray->hashArray[j]->name, pHashArray->hashArray[j]->order);
				}
			}

			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType* pRecords;
	int recordSz = 0;
	int i;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	int hashSz = 31; // Seems like this is what we are using
	struct HashType hashTable;
	hashTable.size = hashSz;
	hashTable.hashArray = (struct RecordType**)malloc(sizeof(struct RecordType*) * hashSz);

	// Initializing the hash table array with NULL pointers
	for (i = 0; i < hashSz; ++i)
	{
		hashTable.hashArray[i] = NULL;
	}

	// Inserting records into the hash table
	for (i = 0; i < recordSz; ++i)
	{
		int index = hash(pRecords[i].id, hashSz);

		int slot = index;
		while (hashTable.hashArray[slot] != NULL)
		{
			slot = (slot + 1) % hashSz;
			if (slot == index)
			{
				printf("Error: Hash table is full.\n");
				exit(1);
			}
		}

		hashTable.hashArray[slot] = &pRecords[i];
	}

	displayRecordsInHash(&hashTable, hashSz);

	// No need to free the hash table or its elements as it's pointing to the original array (pRecords)

	// Free dynamically allocated memory for pRecords
	free(pRecords);

	return 0;
}