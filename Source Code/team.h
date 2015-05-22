/*	Project: A Simple Course Manager
    Team Header file
    Written By: Kevin Church, Long Hoang, Zoey Hsu
                Andy Park, Quynh Nguyen
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bstADT.h"
#include "AVLtreeADT.h"

#ifdef   _MSC_VER
#include <crtdbg.h>
#endif

#define THRESHOLD     75
#define BUCKET_SIZE   3
#define MAX_STR       100
#define TMP_FILE      "courseOut.tmp"
#define INFO_BAR \
    printf("   CRN |  COURSE   |  TITLE                                                  |  INSTRUCTOR               | DAY\n" \
           "==============================================================================================================\n")
#define MEM_ERR       printf("Memory error\n"), exit (101)

typedef struct {
    char     crn[6];
    char     course[9];
    char*    title;
    char*    instr;
    char*    day;
} CLASS;

typedef struct {
    int       count;
    CLASS*    classPtrAry[BUCKET_SIZE];
} BUCKET;

typedef struct {
    int       count;
    int       arySize;
    int       bucketSize;
    BUCKET*   pHash;
    AVL_TREE* pCRN;
    BST_TREE* pCourse;
    BST_TREE* pInstr;
} LIST_HEAD;

// hash table function prototypes
BUCKET*       createHash        (int arySize);
CLASS*        searchHash        (LIST_HEAD* pList, char*   crn);
CLASS*        deleteHash        (LIST_HEAD* pList, char*   crn);
bool          insertHash        (LIST_HEAD* pList, CLASS*  keyPtr);
void          reHash            (LIST_HEAD* pList);

int           hash              (char* key, int last);
int           getArySize        (int size);

// BST application functions prototype
int           compare           (void* class1, void* class2);
int           compareCourse     (void* class1, void* class2);
int           compareCourseDel  (void* class1, void* class2);
int           compareInstr      (void* class1, void* class2);
int           compareInstrDel   (void* class1, void* class2);

// sub function prototypes
char*         allocStr          (char*      temp);
CLASS*        getClass          (FILE*      fin);
bool          retrieveClass     (LIST_HEAD* pList,
                                 QUEUE*     q,
                                 char*      key,
                                 int        choice);
void          freeClass         (void*      dataPtr);
void          freeClassNull     (void*      dataPtr);
void          printClass        (void*      dataPtr);
void          printIndentCrn    (void*      dataPtr);
void          printIndentCourse (void*      dataPtr);
void          printIndentInstr  (void*      dataPtr);
bool          printFile         (LIST_HEAD* pList,
                                 char*      file);

// list management function prototypes
void          printMenu       (void);
char          getOption       (void);
void          efficiency      (LIST_HEAD* pList);
bool          addClass        (LIST_HEAD* pList);
bool          deleteClass     (LIST_HEAD* pList);
void          searchClass     (LIST_HEAD* pList);
void          printHash       (LIST_HEAD* pList);
void          printTree       (LIST_HEAD* pList);
void          printTreeIndent (LIST_HEAD* pList);
bool          saveList        (LIST_HEAD* pList);

// application function prototypes
void          intro           (void);
FILE*         getFile         (void);
LIST_HEAD*    createList      (int        size);
void          insertList      (LIST_HEAD* pList, FILE* fin);
LIST_HEAD*    destroyList     (LIST_HEAD* pList);
