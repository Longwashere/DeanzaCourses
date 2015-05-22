/*	AVL Tree Library: header file
	Written by: G & F
	Date:       2/98

    Brooks/Cole Publishing Company
	An International Thomson Publishing Company
	Copyright 1998. All Rights Reserved
*/
#include <stdio.h>
#include <stdlib.h>

#define LH +1				// Left High
#define EH  0				// Even High
#define RH -1				// Right High

#ifndef TRUE
	#define TRUE  (1)
	#define FALSE (0)
#endif

typedef struct avl_node
{
	 void            *dataPtr;
	 struct avl_node *left;
	 int              bal;
	 struct avl_node *right;
} AVL_NODE;

typedef struct
{
	 int        count;
	 int       (*compare) (void *argu1, void *argu2);
	 AVL_NODE   *root;
} AVL_TREE;

// Public Functions
AVL_TREE *AVL_Create    (int  (*compare) (void *argu1, void *argu2));
int       AVL_Insert    (AVL_TREE *tree, void *dataInPtr);
int       AVL_Delete    (AVL_TREE *tree, void *dltKey);
void     *AVL_Retrieve  (AVL_TREE *tree, void *keyPtr);
void      AVL_Traverse  (AVL_TREE *tree, void (*process) (void *dataPtr) );
int       AVL_Empty     (AVL_TREE *tree);
int       AVL_Full      (AVL_TREE *tree);
int       AVL_Count     (AVL_TREE *tree);
AVL_TREE *AVL_Destroy   (AVL_TREE *tree, void (*freeStruct)(void* dataPtr));
void      AVL_Print     (AVL_TREE *tree, void (*process)(void* dataPtr));

// Private Functions
AVL_NODE *_insert          (AVL_TREE *tree, AVL_NODE *root, AVL_NODE *newPtr, int *taller, int *duplicate);
AVL_NODE *leftBalance      (AVL_NODE *root, int  *taller);
AVL_NODE *rotateLeft       (AVL_NODE *root);
AVL_NODE * rightBalance    (AVL_NODE *root, int  *taller);
AVL_NODE *rotateRight      (AVL_NODE *root);
AVL_NODE *_delete          (AVL_TREE *tree, AVL_NODE *root, void *dltKey, int *shorter, int *success );
AVL_NODE * dltLeftBalance  (AVL_NODE *root, int  *smaller );
AVL_NODE *dltRightBalance  (AVL_NODE *root, int   *shorter);
void     *_retrieve        (AVL_TREE *tree, void  *keyPtr, AVL_NODE *root);
void     _traversal        (AVL_NODE *root, void (*process) (void *dataPtr) );
void     _destroyAVL       (AVL_NODE *root, void (*freeStruct)(void* dataPtr));
void     _print            (AVL_NODE *root, int level, void (*process)(void* dataPtr));
