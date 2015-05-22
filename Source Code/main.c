/****************************************************************************
** CIS 15C
** Fall    2013
***************
**
** Group Project - Team 6		HASH TABLE W/ BUCKETS
*****************************************************************************

	This Program uses 2 Binary Search Trees, 1 AVL Tree, and 1 Hash Table
	with buckets to store a list of classes. They are sorted by CRN number,
	Class Name, and Instructor name.  

****************************************
**
**  Written By: Quynh Nguyen, Zoey Hsu, Andy Park, Long Hoang, Kevin Church
**
**  Date: December 1, 2013
**
********************************************************************************/

#include "team.h"

int main (void) {
    LIST_HEAD* pList;    
    FILE*      fin;
    int        size;    
    bool       cleanFlg;
    char       option;

    intro();

    fin   = getFile ();
    fscanf(fin, "%d", &size);    
    pList = createList(size);    
    insertList(pList, fin); 
    printf("Data is mounted!\nSuccessful ratio: %d / %d\n\n", 
           pList->count, size); 

    cleanFlg  = true;
    printMenu();
    while ((option = getOption ()) != 'Q') {    
        switch (option) {
            case 'A': if (addClass(pList)) {
                           cleanFlg = false;
                           reHash(pList);
                      }
                      break;
            case 'D': if (deleteClass(pList))
                           cleanFlg = false;
                      break; 
            case 'S': searchClass(pList);
                      break;
            case 'H': printHash(pList);
                      break;
            case 'L': printTree(pList);
                      break;
            case 'T': printTreeIndent(pList);
                      break;            
            case 'E': efficiency(pList);
                      break;
            case 'M': printMenu();
                      break;
            case 'F': if (saveList(pList))
                           cleanFlg = true;           
        }  
    }

    if (!cleanFlg) {
        printf("\n\nThere are changes made to last save!\n");
        if (printFile(pList, TMP_FILE))
            printf("List is saved to %s for auto-recovery\n", TMP_FILE);
    }

    pList = destroyList(pList);
    printf("\n\nGoodbye!\n");

    #ifdef _MSC_VER
    printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
    #endif
    return 0;
}