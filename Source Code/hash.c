/*	hash.c contains all functions that create, size, modify, and
	destroy the hash table.  This Hash table uses buckets as its 
	collision solution.  Functions include:
		-getArySize
		-createHash
		-reHash
		-hash
		-insertHash
		-searchHash
		-deleteHash

			Created by: Andy Park

*/
#include <math.h>
#include "team.h"

/* =========================== getArySize ==========================
   Calculates the hash table size (next prime number of double
   entries size)
    PRE   size the entries size
    POST  return calculated hash table size
*/
int getArySize (int size) {
    int  arySize, i;
    bool isPrime = false;

    arySize = size * 2 + 1;
    while(!isPrime) {
        isPrime = true;
        for (i = 3; (i < ceil(sqrt((double)arySize))) && isPrime; i += 2)
            isPrime = arySize % i;

        if (!isPrime)
            arySize += 2;
    }
    return arySize;
}

/* ========================== createHash ==========================
   Creates a hash table and set all count of BUCKET struct to 0
    PRE   arySize size of the table
    POST  returns pointer to the table
*/
BUCKET* createHash (int arySize) {
    BUCKET* newTable;
    int     i;

    if (newTable = (BUCKET*) calloc (arySize, sizeof(BUCKET)))
        for (i = 0; i < arySize; newTable[i++].count = 0);

    return newTable;
}

/* =========================== reHash ==========================
   Expands the hash table and re-hash all current data when
   threshold is reached
    PRE   pList points to the cur LIST_HEAD struct
    POST  hash table expanded if threshold reached
*/
void reHash (LIST_HEAD* pList) {
    LIST_HEAD tmp;
    int       filledB;
    int       i, j;

    for (i = filledB = 0; i < pList->arySize; i++) {
        if (pList->pHash[i].count)
            filledB++;
    }

    if ((filledB * 100 / pList->arySize) > THRESHOLD) {
        printf("Data threshold reached. Attempting to rehash...\n");

        tmp.arySize    = getArySize(pList->arySize);
        tmp.bucketSize = BUCKET_SIZE;

        if (!(tmp.pHash   = createHash(tmp.arySize))) {
            printf("Canceled: Not enough memory\n");
            return;
        }

        for (i = 0; i < pList->arySize; i++)
            for (j = 0; j < pList->pHash[i].count; j++)
                if (!insertHash(&tmp, pList->pHash[i].classPtrAry[j])) {
                    free(tmp.pHash);
                    printf("Canceled: data loss when rehashing\n");
                    return;
                }

        free(pList->pHash);
        pList->pHash   = tmp.pHash;
        pList->arySize = tmp.arySize;
        printf("Hash is successfully expanded\n");
    }
    return;
}

/* ============================ hash ==========================
   Digests the given string to an int
    PRE   key is string to be digested
          arySize is the size of the hash table
    POST  return the int output
*/
int hash (char* key, int arySize) {
    int   locn;
    char* pW;

    locn = 0;
    for(pW = key; *pW; pW++) 
        if (*pW != ' ') {
            locn += *pW;
            locn = ((locn >> 12) | (locn << (sizeof(int) * 8 - 12)));
        }

    return abs(locn) % arySize;
}

/* ========================== insertHash ==========================
   Insert data to hash table
    PRE   pList point to the LIST_HEAD
          keyPtr pointer data to be inserted
    POST  returns true if inserting is successfull
          false if bucket is full
*/
bool insertHash (LIST_HEAD* pList, CLASS* keyPtr) {
    BUCKET* pHash;
    int     locn;

    locn    = hash(keyPtr->crn, pList->arySize);
    pHash   = pList->pHash;

    if (pHash[locn].count < pList->bucketSize) {
        pHash[locn].classPtrAry[pHash[locn].count] = keyPtr;
        (pHash[locn].count)++;
        return true;
    }
    return false;
}

/* =========================== searchHash ==========================
   Searchs hash table for class of given crn
    PRE   pList
          crn is the value to look for
    POST  returns pointer to class of the searched crn
*/
CLASS* searchHash(LIST_HEAD* pList, char* crn) {
    BUCKET* pHash;
    int     locn;
    int     i;

    locn  = hash (crn, pList->arySize);
    pHash = pList->pHash;

    if (pHash[locn].count)
        for (i = 0; i < pHash[locn].count; i++)
            if (!strcmp(pHash[locn].classPtrAry[i]->crn,crn))
                return pHash[locn].classPtrAry[i];
    return NULL;
}

/* ========================== deleteHash ==========================
   Removes a class of given crn from hash table
    PRE   pList
          string crn of the class
    POST  returns pointer to the class removed
*/
CLASS* deleteHash (LIST_HEAD* pList, char* crn) {
    CLASS*  classOut;
    BUCKET* pHash;
    int     locn;
    int     i;

    locn        = hash (crn, pList->arySize);
    pHash       = pList->pHash;

    if (pHash[locn].count)
        for (i = 0; i < pHash[locn].count; i++)
            if (!strcmp(pHash[locn].classPtrAry[i]->crn,crn)) {
                classOut                   = pHash[locn].classPtrAry[i];
                pHash[locn].classPtrAry[i] = pHash[locn].classPtrAry[--(pHash[locn].count)];
                return classOut;
            }
    return NULL;
}
