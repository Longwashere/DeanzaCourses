/*
    listMngr.c includes the functions used for the interface of
	the program, and also the functions to free the Class structure
	in our program.  Functions include:
		-getOption
		-printMenu
		-createList
		-addClass
		-deleteClass
		-searchClass
		-retreiveClass
		-saveList
		-destroyList
		-freeClass
		-freeClassNull

			Created by: Kevin Church
*/
#include "team.h"

/*	===================== getOption =====================
	Reads and validates processing option from keyboard.
	    Pre  nothing
	    Post valid option returned
*/
char getOption  (void) {
    char* OPT = "ADSHLTEFMQ";
    char* opt;
    char  c;

    do {
        printf("\nEnter Option: ");
        scanf(" %c", &c);
        if (opt = strchr(OPT, toupper(c)))
            return *opt;
        printf("Invalid option. Please re-enter. ");
    } while (true);
}

/* ============================ printMenu ==========================
   Prints Menu to screen
    PRE   nothing
    POST  menu is printed to screen
*/
void printMenu(void) {
    printf("    MENU:\n"
           "    A - Add class...\n"
           "    D - Delete class...\n"
           "    S - Search class...\n"
           "    H - Print hash table\n"
           "    L - Print sorted list >>\n"
           "    T - Print indented tree >>\n"
           "    E - Efficiency\n\n"
           "    F - Save to file...\n"
           "    M - Menu\n"
           "    Q - Quit\n");
    return;
}

/* ========================= createList ==========================
   Creates a LIST_HEAD struct containing a hash table, and 3 BSTs
   to manage database.
    PRE   size number of entried from file
    POST  returns pointer to created LIST_HEAD struct
*/
LIST_HEAD* createList (int size) {
    LIST_HEAD* pList;
    int arySize;

    arySize = getArySize(size);
    if (pList = (LIST_HEAD*) malloc (sizeof(LIST_HEAD))) {
        pList->count      = 0;
        pList->arySize    = arySize;
        pList->bucketSize = BUCKET_SIZE;

        if (!((pList->pCRN = AVL_Create(compare))
                && (pList->pHash   = createHash(arySize))
                && (pList->pCourse = BST_Create(compareCourse))
                && (pList->pInstr  = BST_Create (compareInstr))))
            MEM_ERR;
    }
    return pList;
}

/*	========================= addClass ======================
	Prompts and Adds a class of an new CRN to all data
    structures in list.
	    Pre  pList
	    Post class is added (abort on memory overflow)
*/
bool addClass (LIST_HEAD* pList) {
    CLASS* newClass;
    char   tmp[MAX_STR];
    bool   result = false;

    if (!(newClass = (CLASS*)malloc (sizeof (CLASS))))
        MEM_ERR;

    printf("[...]\n");
    printf("Enter CRN (#####): ");
    scanf (" %5s",  newClass->crn);
    printf("Enter course: ");
    scanf (" %8s",  newClass->course);
    printf("Enter course title: ");
    scanf (" %[^\n]",  tmp);
    newClass->title = allocStr(tmp);
    printf("Enter instructor (Last, First): ");
    scanf (" %[^\n]",  tmp);
    newClass->instr = allocStr(tmp);
    printf("Enter course day (MTWThFSSu): ");
    scanf (" %[^\n]",  tmp);
    newClass->day   = allocStr(tmp);

    if (searchHash(pList, newClass->crn))
        printf("\nNOT added: Existing CRN\n");
    else if (!insertHash(pList, newClass))
        printf("\nNOT added: Bucket is full\n");
    else if (!(AVL_Insert(pList->pCRN, newClass)
                 && BST_Insert(pList->pCourse, newClass)
                 && BST_Insert(pList->pInstr, newClass)))
        MEM_ERR;
    else {
        printf("\nClass added successfully\n");
        (pList->count)++;
        result = true;
    }

    if (!result)
        freeClass(newClass);
    return result;
}

/*	==================== deleteClass ====================
	Prompts and Deletes a class of given CRN from all
    data structures in list
	    Pre  pList
	    Post if found, class of given CRN is removed
                from all data structs, data is free,
                returns true
             else error message is printed
*/
bool deleteClass (LIST_HEAD* pList) {
    QUEUE*    q;
    CLASS*    classOut;
    int       choice;
    bool      found;
    char      str[MAX_STR];

    printf("[...]\tPlease enter key word: ");
    scanf (" %[^\n]", str);

    printf("\tDelete class by:\n"
           "\t    0. CRN             1. Course           2.Instructor\n"
           "Your choice: ");
    scanf (" %d", &choice);
    if (!(choice == 1 || choice == 2 || choice ==0)) {
        printf("Invalid choice!\n");
        return false;
    }

    q = createQueue();
    if (found = retrieveClass(pList, q, str, choice)) {
        printf("Found match:\n\n");
        INFO_BAR;
        while (dequeue(q, (void**)&classOut)) {
            printClass(classOut);

            deleteHash(pList,          classOut->crn);
            AVL_Delete(pList->pCRN,    classOut);
            BST_Delete(pList->pInstr,  classOut, compareInstrDel);
            BST_Delete(pList->pCourse, classOut, compareCourseDel);

            freeClass(classOut);
            (pList->count)--;
        }
        printf("\nClasses are removed from list!\n");
    }
    else
        printf("\nClass %s is NOT found\n", str);

    destroyQueue(q);
    return found;
}

/*	====================== searchClass ======================
	Search Manager:
    Prompts and search for class(es) in different criteria,
    including:
            .search by CRN
            .search by Course
            .search by Intructor
	    Pre  pList
	    Post Info of matching classed are printed
             Error message is no match found
*/
void searchClass (LIST_HEAD* pList) {
    QUEUE*    q;
    CLASS*    classOut;
    int       choice;
    bool      found;
    char      str[MAX_STR];

    printf("[...]\tPlease enter key word: ");
    scanf (" %[^\n]", str);

    printf("\tSearch class by:\n"
           "\t    0. CRN             1. Course           2.Instructor\n"
           "Your choice: ");

    scanf (" %d", &choice);
    if (!(choice == 1 || choice == 2 || choice ==0)) {
        printf("Invalid choice!\n");
        return;
    }

    q = createQueue();
    if (found = retrieveClass(pList, q, str, choice)) {
        printf("Found match:\n\n");
        INFO_BAR;
        while (dequeue(q, (void**)&classOut))
            printClass(classOut);
    }
    else
        printf("No match is found!\n");

    destroyQueue(q);
    return;
}

/*	====================== retrieveClass ======================
	Search for class(es) in different criteria
	    Pre  pList
             q      points to an initialize queue to store matches
             key    search key string
             choice criteria for key string
	    Post if match(es) found in the criteria datastruct, matches
                                        are stored in q, return true
             if no matches found, return false
*/
bool retrieveClass (LIST_HEAD* pList, QUEUE* q, char* key, int choice) {
    bool    found;
    CLASS*  classOut;
    CLASS   tmpClass;

    found = false;
    switch (choice) {
        case 0 : // Search hash for CRN search
            if (classOut = searchHash(pList, key)) {
                enqueue(q, classOut);
                found = true;
            }
            break;
        case 1 : // Search BST Course
            strncpy(tmpClass.course, key, 8);
            found = BST_Retrieve_Dup(pList->pCourse, &tmpClass, q);
            break;
        case 2 : // Search BST Instr
            tmpClass.instr = key;
            found = BST_Retrieve_Dup(pList->pInstr, &tmpClass, q);
    }
    return found;
}

/*	========================= saveList =====================
	Prompts and save data as given name to disk.
	    Pre  pList
	    Post data is saved to file
*/
bool saveList (LIST_HEAD* pList) {
    char file[MAX_STR];
    bool result;

    printf("[...]\tSave data to file as: ");
    scanf(" %[^\n]", file);

    if (result = printFile(pList, file))
        printf("File is saved to %s file\n", file);
    return result;
}

/*	==================== destroyList =====================
	Destroy list and all allocated data.
	    Pre   pList
	    Post  all data is freed
              returns NULL
*/
LIST_HEAD* destroyList (LIST_HEAD* pList) {

    free(pList->pHash);
    BST_Destroy(pList->pCourse,freeClass);
    AVL_Destroy(pList->pCRN,   freeClassNull);
    BST_Destroy(pList->pInstr, freeClassNull);
    free(pList);
    return NULL;
}

/*	====================== freeCLass ======================
	Frees class struct and all its allocated data
	    Pre  pointer to class struct
	    Post class struct and its allocated data are freed
*/
void freeClass (void* dataPtr) {
    CLASS* dCls;

    dCls = (CLASS*)dataPtr;
    free(dCls->title);
    free(dCls->instr);
    free(dCls->day);
    free(dCls);
    return;
}

/*	====================== freeCLassNull ===================
	Place holder for function pointer when calling BST_Destroy
	    Pre  pointer to class struct
	    Post nothing
*/
void freeClassNull (void* dataPtr) {
    return;
}
