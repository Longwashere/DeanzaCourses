/*	fileIO.c contains the source code for reading input files, creating
	output files, and the compare functions for the various keys for 
	the AVL and BST Trees.  Functions include:
		-getFile
		-insertList
		-compare
		-compareCourseDel
		-compareCourse
		-compareInstrDel
		-compareInstr
		-allocStr
		-printFile

			Created by: Long Hoang
*/

#include "team.h"

/* ============================ getFile ==========================
   Retrieves an auto-recovery save file or get a new input file
   form user.
    PRE   nothing
    POST  returns pointer to openned input file
*/
FILE* getFile (void) {
    char  f_name[MAX_STR];
    FILE* fin;

    if (fin = fopen(TMP_FILE, "r")) {
        printf("An auto-recovery file is found.\n"
               "Would you like to use this file? [y] ");

        if (!(toupper(getchar()) == 'Y')) {
            fclose (fin);
            fin = NULL;
        }
    }

    while (!fin) {
        printf("Please enter input file name: ");
        scanf(" %[^\n]", f_name);

        if (!(fin = fopen(f_name, "r")))
            printf("File does not exist.\n");
    }
    return fin;
}

/* ============================ insertList ==========================
   Builds hash table and BSTs from input file
    PRE   pList points to an initilized LIST_HEAD struct
          fin   points to input file
    POST  all database structs are populate with input from file
*/
void insertList (LIST_HEAD* pList, FILE* fin) {
    CLASS*  tmpClass;
    char    tCrn[6];
    char    tCourse[9];
    char    tTitle[100];
    char    tInstr[50];
    char    tDay[10];

    while (fscanf(fin, " %[^;];%[^;];%[^;];%[^;];%[^\n]",
                  tCrn, tCourse, tTitle, tDay, tInstr) != EOF) {

        if (!(tmpClass = (CLASS*) malloc (sizeof(CLASS))))
            MEM_ERR;

        strcpy(tmpClass->crn, tCrn);
        strcpy(tmpClass->course, tCourse);
        tmpClass->title = allocStr(tTitle);
        tmpClass->instr = allocStr(tInstr);
        tmpClass->day   = allocStr(tDay);

        if (!insertHash(pList, tmpClass))
            freeClass(tmpClass);
        else if (!(AVL_Insert(pList->pCRN, tmpClass)
                    && BST_Insert(pList->pCourse, tmpClass)
                    && BST_Insert(pList->pInstr, tmpClass)))
            MEM_ERR;
        else
            (pList->count)++;
    }
    return;
}

/* ============================ compare ==========================
   Compares CRN of 2 classes
    PRE   pointers to 2 classes
    POST  if class1's crn > class2's returns 1
          if class1's crn == class2's returns 0
          if class1's crn < class2's return -1
*/
int compare (void* class1, void* class2) {
    return stricmp(((CLASS*)class1)->crn,((CLASS*)class2)->crn);
}

/* ======================== compareCourseDel =====================
   Compares course string of 2 classes (only used for BST_Delete)
    PRE   pointers to 2 classes
    POST  if class1's course > class2's returns 1
          if class1's course < class2's return -1
          if class1's course == class2's:
                if CRN is the same return 0
                else return 1
*/
int compareCourseDel (void* class1, void* class2) {
    CLASS c1, c2;
    int   cmp;

    c1 = *(CLASS*)class1;
    c2 = *(CLASS*)class2;

    if (!(cmp = stricmp(c1.course,c2.course)))
        cmp = strcmp(c1.crn,c2.crn) ? 1 : 0;
    return cmp;
}

/* ===================== compareCourse ==========================
   Compares Course strings of 2 classes
    PRE   pointers to 2 classes
    POST  if class1's course > class2's returns 1
          if class1's course == class2's returns 0
          if class1's course < class2's return -1
*/
int compareCourse (void* class1, void* class2) {
    CLASS c1, c2;

    c1 = *(CLASS*)class1;
    c2 = *(CLASS*)class2;

    return stricmp(c1.course,c2.course);
}

/* ===================== compareInstrDel ==========================
   Compares Instr string of 2 classes (only used for BST_Delete)
    PRE   pointers to 2 classes
    POST  if class1's Instr > class2's returns 1
          if class1's Instr < class2's return -1
          if class1's Instr == class2's:
                if CRN is the same return 0
                else return 1
*/
int compareInstrDel (void* class1, void* class2) {
    CLASS c1, c2;
    int   cmp;

    c1 = *(CLASS*)class1;
    c2 = *(CLASS*)class2;

    if (!(cmp = stricmp(c1.instr,c2.instr)))
        cmp = stricmp(c1.crn,c2.crn) ? 1 : 0;
    return cmp;
}

/* ====================== compareInstr ==========================
   Compares INSTR string of 2 classes
    PRE   pointers to 2 classes
    POST  if class1's instr > class2's returns 1
          if class1's instr == class2's returns 0
          if class1's instr < class2's return -1
*/
int compareInstr (void* class1, void* class2) {
    CLASS c1, c2;

    c1 = *(CLASS*)class1;
    c2 = *(CLASS*)class2;

    return stricmp(c1.instr,c2.instr);
}

/* ============================ allocStr ==========================
   Dynamically allocates a string for a given text string
    PRE   temp - the given string
    POST  returns pointer to address of dynamically allocated string
*/
char *allocStr(char *temp) {
    char *s;

    if(!(s = (char*) calloc (strlen(temp) + 1, sizeof(char))))
        MEM_ERR;
    strcpy(s, temp);
    return s;
}

/* =========================== printFile ==========================
   Prints data to file in breadth-first traversal of BST Tree
    PRE   pList
          file contains output file name string
    POST  data is printed to file
*/
bool printFile (LIST_HEAD* pList, char* file) {
    FILE*  fout;
    QUEUE* q;
    CLASS* c;

    if (!(fout = fopen(file, "w"))) {
        printf("Error opening %s file\n", file);
        return false;
    }
    if (!(q = createQueue()))
        MEM_ERR;

    fprintf(fout,"%d", pList->count);
    BST_Queue_bf(pList->pInstr, q);
    while (dequeue(q, (void**)&c))
        fprintf(fout, "\n%s;%s;%s;%s;%s",
                c->crn, c->course, c->title, c->day, c->instr);
    destroyQueue(q);
    fclose(fout);
    return true;
}
