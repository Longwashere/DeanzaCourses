/*	screenOutput.c includes the source code that introduces the
	program, print the list is different ways, and the effeciency
	code for the hash table.  Functions include:
		-intro
		-printClass
		-efficiency
		-printTree
		-printHash
		-printTreeIndent
		-printIndentCourse
		-printIndentInstr
		-printIndentCrn

			Created by: Quynh Nguyen
*/

#include "team.h"

/*	====================== intro ======================
	Prints program intro to screen
        Pre   nothing
        Post  intro is printed
*/
void intro () {
    printf("\n\n"
           "                         _      ____  _                 _                 \n"
           "                        / \\    / ___|(_)_ __ ___  _ __ | | ___           \n"
           "                       / _ \\   \\___ \\| | '_ ` _ \\| '_ \\| |/ _ \\     \n"
           "                      / ___ \\   ___) | | | | | | | |_) | |  __/          \n"
           "                     /_/___\\_\\ |____/|_|_| |_| |_| .__/|_|\\___|        \n"
           "                      / ___|___  _   _ _ __ ___  |_|                      \n"
           "                     | |   / _ \\| | | | '__/ __|/ _ \\                   \n"
           "                     | |__| (_) | |_| | |  \\__ \\  __/                   \n"
           "                      \\____\\___/ \\__,_|_|  |___/\\___|                 \n"
           "                     |  \\/  | __ _ _ __   __ _  __ _  ___ _ __           \n"
           "                     | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '__|        \n"
           "                     | |  | | (_| | | | | (_| | (_| |  __/ |              \n"
           "                     |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|          \n"
           "                                               |___/                      \n\n"
           "                               ( Version 1.1 )                            \n\n"
           "                       By: Kevin Church, Zoey Hsu, Andy Park              \n"
           "                           Long Hoang, Quynh Nguyen                       \n\n"
           "\n\n");
    return;
}

/* =========================== printClass ==========================
   Prints all fields of a class struct in set format
    PRE   dataPtr points to the class struct
    POST  class info is printed
*/
void printClass(void* dataPtr) {
    CLASS cls;

    cls = *(CLASS*)dataPtr;
    printf(" %5s | %-9s |  %-54s | %-25s | %-7s\n",
           cls.crn, cls.course, cls.title, cls.instr, cls.day);
    return;
}

/* ========================== efficiency ==========================
   Displays analysis of the current hash table, including: loadfactor
   longest buckets, collision.
    PRE   pList
    POST  analysis is printed to screen
*/
void efficiency (LIST_HEAD* pList) {
#define COL 100

    int loadFactor;
    int longestB;
    int collision;
    int filledB;
    int i, j, k, rep;

    filledB = longestB = 0;

    for (i = 0; i < pList->arySize; i++) {
        if (pList->pHash[i].count)
            filledB++;
        if (pList->pHash[i].count > longestB)
            longestB = pList->pHash[i].count;
    }

    loadFactor = filledB * 100 / pList->arySize;
    collision  = pList->count - filledB;

    printf("EFFIENCY\n\n"
           "List count:      %5d\n"
           "List Array Size: %5d        Collision:       %5d\n"
           "Threshold:      %5d%%        Load factor:    %5d%%\n"
           "Max Bucket Size: %5d        Longest bucket:  %5d\n\n"
           "Graph:\n\n",
           pList->count, pList->arySize, collision,
           THRESHOLD, loadFactor,
           pList->bucketSize, longestB);

    rep = pList->arySize / COL + ((pList->arySize % COL) != 0);

    for (k = 0; k < rep; printf("\n"), k++) {
        for (j = BUCKET_SIZE; j > 0; printf("\n"), j--)
            for (i = 0; i < COL ; i++)
                printf(pList->pHash[k * COL + i].count >= j? "x" : " ");
        for (i = 0; i < COL && ((k * COL + i) < pList->arySize); printf("-"), i++);
    }
    return;
}

/* ============================ printTree ==========================
   Print Manager:
   Prompts and prints a choosen BST in the list to screen in in-order
   traverse (sorted):
            .print CRN BST
            .print Course BST
            .Print Instr BST
    PRE   pList
    POST  Tree content is printed in in-order traverse
*/
void printTree(LIST_HEAD* pList) {
    int choice;

    printf("[>>]\tPrint sorted list by:\n"
           "\t    0. CRN             1. Course           2.Instructor\n"
           "Your choice: ");

    scanf("%d", &choice);
    if (!(choice == 0 || choice == 1 || choice == 2)) {
        printf("Invalid choice!\n");
        return;
    }

    printf("TREE DETAIL:\n\n");
    INFO_BAR;
    switch (choice) {
        case 0: AVL_Traverse (pList->pCRN, printClass);
                break;
        case 1: BST_Traverse_inorder(pList->pCourse, printClass);
                break;
        case 2: BST_Traverse_inorder(pList->pInstr, printClass);
    }
    return;
}

/* =========================== printHash ==========================
   Prints all the content of the hash table to screen
    PRE   pList
    POST  content of the hash table is print
*/
void printHash(LIST_HEAD* pList) {
    int i, j;

    printf("HASH TABLE DETAIL:\n\n");
    INFO_BAR;
    for (i = 0; i < pList->arySize; i++) {
        if (pList->pHash[i].count) {
            printf("------------------+  BUCKET [%d]\n",
                    i, pList->pHash[i].count);
            for (j = 0; j < pList->pHash[i].count; j++)
                printClass(pList->pHash[i].classPtrAry[j]);
        }
    }
    return;
}

/* ========================== printTreeIndent ==========================
    Prompts and prints graph-like indented tree
    PRE   pList
    POST  Indented trees is printed
*/
void printTreeIndent (LIST_HEAD* pList) {
    int choice;

    printf("[>>]\tPrint indented tree by:\n"
           "\t    0. CRN             1. Course           2.Instructor\n"
           "Your choice: ");

    scanf("%d", &choice);
    switch (choice) {
        case 0: AVL_Print(pList->pCRN, printIndentCrn);
                break;
        case 1: BST_Print(pList->pCourse, printIndentCourse);
                break;
        case 2: BST_Print(pList->pInstr, printIndentInstr);
                break;
        default: printf("Invalid choice!\n");
    }
    return;
}

/* =================== printIndentCourse ==========================
   Sub function is be called by BST_Print. Prints course field
    PRE   dataPtr point to a class
    POST  course field is printed
*/
void printIndentCourse (void* dataPtr) {
    CLASS c = *(CLASS*)dataPtr;

    printf("( %s ) CRN: %s \n",c.course, c.crn);
    return;
}

/* =================== printIndentInstr ==========================
   Sub function is be called by BST_Print. Prints Instr field
    PRE   dataPtr point to a class
    POST  Instr field is printed
*/
void printIndentInstr (void* dataPtr) {
    CLASS c = *(CLASS*)dataPtr;

    printf("( %s ) CRN: %s \n",c.instr, c.crn);
    return;
}

/* =================== printIndentCrn ==========================
   Sub function is be called by BST_Print. Prints Crn field
    PRE   dataPtr point to a class
    POST  CRN field is printed
*/
void printIndentCrn (void* dataPtr) {
    CLASS c = *(CLASS*)dataPtr;

    printf("( CRN: %s )\n", c.crn);
    return;
}
