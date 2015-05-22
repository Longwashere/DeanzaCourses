/*	AVL Tree Library: function definitions
	Written by: G & F
	Date:       2/98

    Brooks/Cole Publishing Company
	An International Thomson Publishing Company
	Copyright 1998. All Rights Reserved

	AVLTresADT.c is the source code that creates, modifies, and destroys
	the AVL tree.  Its purpose is to hold the address of a class in order
	of a unique CRN key.

		Modified by: Kevin Church on December 1st, 2013

*/

#include "AVLtreeADT.h"


/*	=============== AVL_Create ==============	*/
/*	Allocates dynamic memory for an AVL tree head
	node and returns its address to caller
	Pre		compare is address of compare function used
			whenever two nodes need to be compared.
	Post		head allocated or error returned
	Return		head node pointer or null if memory overflow
*/
AVL_TREE *AVL_Create (int  (*compare) (void *argu1, void *argu2)) {
    AVL_TREE *tree;

    tree = (AVL_TREE *) malloc (sizeof (AVL_TREE));
    if (tree) {
        tree->root    = NULL;
        tree->count   = 0;
        tree->compare = compare;
    }

    return tree;
}	// AVL_Create

/*  ==================== AVL_Insert ==================== */
/*	This function inserts new data into the tree.
	Pre    tree is a pointer to a valid AVL tree structure.
	Post   data have been inserted or memory overflow.
	Return Success (TRUE) or Overflow (FALSE)
*/
int AVL_Insert (AVL_TREE *tree, void *dataInPtr) {
    AVL_NODE *newPtr;
    int   forTaller;
    int   duplicate = FALSE;

    newPtr = (AVL_NODE *)malloc(sizeof(AVL_NODE));
    if (!newPtr)
        return FALSE;

    newPtr->bal         = EH;
    newPtr->right       = NULL;
    newPtr->left        = NULL;
    newPtr->dataPtr     = dataInPtr;

    tree->root = _insert(tree,  tree->root, newPtr, &forTaller, &duplicate);

    // fixed: handle duplicate case
    if( duplicate ) {
        free(newPtr);
        return FALSE;
    }

    (tree->count)++;
    return TRUE;
}   // AVL_Insert

/*	======================= _insert =======================
	This function uses recursion to insert the new data into
	a leaf node in the AVL tree.
	Pre    Application has called AVL_Insert, which passes
	       root and data pointers.
	Post   Data have been inserted.
	Return pointer to [potentially] new root.
*/
AVL_NODE *_insert (AVL_TREE *tree, AVL_NODE *root, AVL_NODE *newPtr, int *taller, int *duplicate) {
    if (!root) {
        root     = newPtr;
        *taller  = TRUE;
        return   root;
    } // if NULL tree

    if (tree->compare(newPtr->dataPtr, root->dataPtr) < 0) { // new data < root data
        root->left = _insert(tree, root->left, newPtr, taller, duplicate);
        if ( *taller )
            // Left subtree is taller
            switch ( root->bal ) {
            case LH:  // Was left high--rotate
                root = leftBalance ( root, taller );
                break;

            case EH:  // Was balanced--now LH
                root->bal = LH;
                break;

            case RH:  // Was right high--now EH
                root->bal = EH;
                *taller   = FALSE;
                break;
            } // switch
        return root;
    }  // new < node
    else if (tree->compare(newPtr->dataPtr, root->dataPtr) > 0) { // new data > root data
        root->right = _insert (tree, root->right, newPtr, taller, duplicate);
        if ( *taller )
            // Right subtree is taller
            switch ( root->bal ) {
            case LH:  // Was left high--now EH
                root->bal = EH;
                *taller   = FALSE;
                break;

            case EH:  // Was balanced--now RH
                root->bal = RH;
                break;

            case RH:  // Was right high--rotate
                root = rightBalance (root, taller );
                break;
            } // switch
        return root;
    }

    *duplicate = TRUE; // set duplicate to TRUE
    return root;
}	// _insert

/* ===================== leftBalance =====================
	The tree is out-of-balance to the left. This function
	rotates the tree to the right.
	Pre	 The tree is left high.
	Post Balance restored. Returns potentially new root.
*/
AVL_NODE *leftBalance (AVL_NODE *root, int  *taller) {
    AVL_NODE *rightTree;
    AVL_NODE *leftTree;

    leftTree = root->left;
    switch (leftTree->bal ) {
    case LH: // Left High--Rotate Right
        root->bal     = EH;
        leftTree->bal = EH;

        // Rotate Right
        root     = rotateRight (root);
        *taller  = FALSE;
        break;
    case EH: // This is an error
        //printf ("\n\a\aError in leftBalance\n");
        exit (100);
    case RH: // Right High - Requires double rotation: first left, then right
        rightTree = leftTree->right;
        switch (rightTree->bal) {
        case LH:
            root->bal     = RH;
            leftTree->bal = EH;
            break;
        case EH:
            root->bal     = EH;
            leftTree->bal = EH;
            break;
        case RH:
            root->bal     = EH;
            leftTree->bal = LH;
            break;
        } // switch rightTree

        rightTree->bal = EH;
        // Rotate Left
        root->left = rotateLeft (leftTree);

        // Rotate Right
        root     = rotateRight (root);
        *taller  = FALSE;
    } // switch
    return root;
}	// leftBalance

/* ===================== rotateLeft ======================
	This function exchanges pointers so as to rotates the tree
	to the left.
	Pre  root points to tree to be rotated.
	Post Node rotated and new root returned.
*/
AVL_NODE *rotateLeft (AVL_NODE *root) {
    AVL_NODE *tempPtr;

    tempPtr        = root->right;
    root->right    = tempPtr->left;
    tempPtr->left  = root;

    return tempPtr;
}	// rotateLeft

/* ============================= rightBalance ============================= */
/*	The tree is out-of-balance to the right. This function
	rotates the tree to the left.
	Pre	 The tree is right high.
	Post Balance restored. Returns potentially new root.
*/
AVL_NODE * rightBalance (AVL_NODE *root, int  *taller) {
    AVL_NODE *rightTree;
    AVL_NODE *leftTree;

    rightTree = root->right;
    switch (rightTree->bal ) {
    case LH:	// Left High - Requires double rotation: first right, then left
        leftTree = rightTree-> left;

        // First Rotation - Left
        switch (leftTree->bal ) {
        case RH:
            root->bal		= LH;
            rightTree->bal	= EH;
            break;

        case EH:
            root->bal		= EH;
            rightTree->bal	= EH;
            break;

        case LH:
            root->bal		= EH;
            rightTree->bal	= RH;
            break;
        } // switch
        leftTree->bal		= EH;

        root->right	= rotateRight (rightTree );
        root		= rotateLeft  (root );
        *taller	= FALSE;
        break;

    case EH:	// This is an error
        /*  	 	 			printf( "\n\a\aError in rightBalance\n" );
            	 	 			break;
        */
        // Deleting from balanced node
        root->bal = EH;
        taller    = FALSE;
        break;

    case RH:	// Right High - Rotate Left
        root->bal		= EH;
        rightTree->bal	= EH;

        root	= rotateLeft ( root );
        *taller	= FALSE;
        break;

    } // switch
    return root;

}/* rightBalance */

/* ===================== rotateRight =====================
	This function exchanges pointers to rotate the tree right
	to the right.
	Pre  root points to tree to be rotated.
	Post Node rotated and new root returned.
*/
AVL_NODE *rotateRight (AVL_NODE *root) {
    AVL_NODE *tempPtr;

    tempPtr         = root->left;
    root->left      = tempPtr->right;
    tempPtr->right  = root;

    return tempPtr;
}	// rotateRight

/* ====================== AVL_Delete ======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	Pre    Tree must be initialized. Null tree is OK.
          dltKey contains pointer to key to be deleted
	Post   The node is deleted and its space recycled
	       -or- An error code is returned.
    Return Success (TRUE) or Not found (FALSE)
*/
int AVL_Delete (AVL_TREE *tree, void *dltKey) {
    int shorter;
    int success;

    AVL_NODE *newRoot;

    newRoot = _delete (tree,    tree->root, dltKey,
                       &shorter, &success);
    if (success) {
        tree->root = newRoot;
        (tree->count)--;
        return TRUE;
    } else
        return FALSE;
}   // AVL_Delete

/*	======================== _delete =======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	Pre   Tree must be initialized. Null tree is OK.
	      dltKey contains key of node to be deleted
	      shorter is boolean indicating tree is shorter
	Post   The node is deleted and its space recycled
	       -or- if key not found, tree is unchanged.
	Return success TRUE if deleted, FALSE if not found
	       pointer to root
*/
AVL_NODE *_delete (AVL_TREE *tree, AVL_NODE *root, void *dltKey, int *shorter, int *success ) {
    AVL_NODE *dltPtr;
    AVL_NODE *exchPtr;
    AVL_NODE *newRoot;

    if (!root) {
        *shorter = FALSE;
        *success = FALSE;
        return NULL;
    }

    if (tree->compare(dltKey, root->dataPtr) < 0) {
        root->left = _delete (tree,    root->left, dltKey,
                              shorter, success);
        if (*shorter)
            root   = dltRightBalance (root, shorter);
    } else if (tree->compare(dltKey, root->dataPtr) > 0) {
        root->right = _delete (tree,    root->right, dltKey,
                               shorter, success);
        if (*shorter)
            root = dltLeftBalance (root, shorter);
    } else { // Found equal node
        dltPtr = root;
        if (!root->right) {		// Only left subtree
            newRoot  = root->left;
            *success = TRUE;
            *shorter = TRUE;
            free ( dltPtr);
            return newRoot;		// base case
        } else if (!root->left) {	// Only right subtree
            newRoot  = root->right;
            *success = TRUE;
            *shorter = TRUE;
            free (dltPtr);
            return newRoot;	// base case
        } else { // Delete Node has two subtrees
            exchPtr = root->left;
            while (exchPtr->right)
                exchPtr = exchPtr->right;
            root->dataPtr = exchPtr->dataPtr;
            root->left = _delete (tree, root->left,
                                  exchPtr->dataPtr,
                                  shorter, success);
            if (*shorter)
                root = dltRightBalance (root, shorter);
        }

    }
    return root;
}	// _delete

/* ============================= dltLeftBalance ============================= */
/*	The tree is out-of-balance to the left (left high). This function rotates
	the tree to the right.
	Pre	 The tree is left high.
	Post Balance has been restored. Returns potentially new root.
*/
AVL_NODE * dltLeftBalance  ( AVL_NODE *root,
                             int  *smaller ) {
    AVL_NODE *rightTree;
    AVL_NODE *leftTree;

    switch ( root->bal ) {
    case RH:
        root->bal	= EH;
        break;

    case EH:	// Delete occurred on right
        root->bal   = LH;
        *smaller    = FALSE;
        break;

    case LH:
        leftTree = root->left;

        switch ( leftTree->bal ) {
        case LH:
        case EH:	// Rotate Single Left
            if ( leftTree->bal  == EH ) {
                root->bal		= LH;
                leftTree->bal	= RH;
                *smaller       = FALSE;
            } else {
                root->bal     = EH;
                leftTree->bal = EH;
            }
            // 	*****		root->left      = leftTree->right;
            // 	*****		leftTree->right = root;
            // 	*****		root            = leftTree;

            root = rotateRight (root);
            break;

        case RH:	// Double Rotation
            rightTree = leftTree->right;
            switch ( rightTree->bal ) {
            case LH:
                root->bal     = RH;
                leftTree->bal = EH;
                break;
            case EH:
                root->bal     = EH;
                leftTree->bal = EH;
                break;
            case RH:
                root->bal     = EH;
                leftTree->bal = LH;
                break;
            } // switch
            rightTree->bal = EH;

            // balance indicators set: now rotate
            // ***** 			leftTree->right  = rightTree->left;
            // ***** 			rightTree->left  = leftTree;

            // ***** 			root->left       = rightTree->right;
            // ***** 			rightTree->right = root;
            // ***** 			root             = rightTree;
            root->left = rotateLeft (leftTree);
            root       = rotateRight (root);

            break;

        } // switch : leftTree->bal

    } // switch : root->bal
    return root;

}// dltLeftBalance

/*	=================== dltRightBalance ==================
The tree is shorter after a delete on the left. This
function adjusts the balance factors and rotates the tree
to the left if necessary.
Pre  The tree shorter.
Post Balance factors adjusted and balance restored.
     Returns potentially new root.
*/
AVL_NODE *dltRightBalance (AVL_NODE *root,
                           int  *shorter) {
    AVL_NODE *rightTree;
    AVL_NODE *leftTree;

    switch (root->bal) {
    case LH: // Deleted Left--Now balanced
        root->bal  = EH;
        break;

    case EH: // Now Right high
        root->bal  = RH;
        *shorter   = FALSE;
        break;

    case RH: // Right High - Rotate Left
        rightTree = root->right;
        if (rightTree->bal == LH)
            // Double rotation required
        {
            leftTree  = rightTree->left;

            switch (leftTree->bal) {
            case LH:
                rightTree->bal = RH;
                root->bal      = EH;
                break;
            case EH:
                root->bal      = EH;
                rightTree->bal = EH;
                break;
            case RH:
                root->bal      = LH;
                rightTree->bal = EH;
                break;
            } // switch

            leftTree->bal = EH;

            // Rotate Right then Left
            root->right   = rotateRight (rightTree);
            root          = rotateLeft  (root);
        } // if rightTree->bal == LH
        else {
            // Single Rotation Only
            switch (rightTree->bal) {
            case LH:
            case RH:
                root->bal      = EH;
                rightTree->bal = EH;
                break;
            case EH:
                root->bal      = RH;
                rightTree->bal = LH;
                *shorter       = FALSE;
                break;
            } // switch rightTree->bal
            root = rotateLeft ( root );
        }
    } // switch
    return root;

}	// dltRightBalance

/*	==================== AVL_Retrieve ===================
	Retrieve node searches the tree for the node containing
	the requested key and returns pointer to its data.
	Pre     Tree has been created (may be null).
	        data pointer is pointer to data structure
	              containing key to be located.
	 Post    Tree searched and data pointer returned.
	 Return  Address of matching node returned.
	         If not found, NULL returned.
*/
void *AVL_Retrieve  (AVL_TREE *tree,
                     void     *keyPtr) {
    if (tree->root)
        return _retrieve (tree, keyPtr, tree->root);
    else
        return NULL;
}	// AVL_Retrieve


/*	===================== _retrieve =====================
	Retrieve node searches tree for node containing requested
	key and returns its data to the calling function.
	Pre     AVL_Retrieve called: passes tree, keyPtr, root.
	        key pointer is pointer to data structure containing
	           key to be located.
	Post    Tree searched and data pointer returned.
	Return  Address of matching node returned.
	        If not found, NULL returned.
*/
void * _retrieve (AVL_TREE *tree,
                  void      *keyPtr,
                  AVL_NODE      *root) {
    if (root) {
        if (tree->compare (keyPtr, root->dataPtr) < 0)
            return _retrieve (tree, keyPtr, root->left);
        else if ((tree->compare (keyPtr, root->dataPtr) > 0))
            return _retrieve (tree, keyPtr, root->right);
        else
            // Found equal key
            return (root->dataPtr);
    }  // if root
    else
        // Data not in tree
        return NULL;
}	// _retrieve

/*	==================== AVL_Traverse ====================
	Process tree using inorder traversal.
	Pre   Tree has been created (may be null).
	      process used to ñvisitî nodes during traversal.
	Post  All nodes processed in LNR (inorder) sequence.
*/
void AVL_Traverse (AVL_TREE *tree,
                   void (*process) (void *dataPtr) ) {
    _traversal (tree->root, process);
    return;
}  // end AVL_Traverse

/*	===================== _traversal =====================
/*	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	Pre   Tree has been created (may be null).
	Post  All nodes processed.
*/
void _traversal (AVL_NODE *root,
                 void (*process) (void *dataPtr) ) {
//	Statements
    if  (root) {
        _traversal  (root->left, process);
        process     (root->dataPtr);
        _traversal  (root->right, process);
    }
    return;
}  // _traversal

/*	=============== AVL_Empty ==============
	Returns true if tree is empty, false if any data.
	Pre      Tree has been created. May be null.
	Returns  True if tree empty, false if any data.
*/
int AVL_Empty (AVL_TREE *tree) {
    return (tree->count == 0);
}	// AVL_Empty

/*	=============== AVL_Full ==============
	If there is no room for another node, returns true.
	Pre      Tree has been created.
	Returns  True if no room for another insert,
	         false if room.
*/
int AVL_Full (AVL_TREE *tree) {
    AVL_NODE *newPtr;

    newPtr = (AVL_NODE *)malloc(sizeof (AVL_NODE));
    if (newPtr) {
        free (newPtr);
        return FALSE;
    } else
        return TRUE;
}	// AVL_Full

/*	=============== AVL_Count ==============
	Returns number of nodes in tree.
	Pre     Tree has been created.
	Returns tree count
*/
int AVL_Count (AVL_TREE *tree) {
    return (tree->count);
}	// AVL_Count

/*	=============== AVL_Destroy ==============
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	Pre      tree is a pointer to a valid tree.
	Post     All data and head structure have been deleted.
	Return   null head pointer
*/
AVL_TREE *AVL_Destroy (AVL_TREE *tree, void (*freeStruct) (void* dataPtr)) {
    if (tree)
        _destroyAVL ((tree->root), freeStruct);
    // All nodes deleted. Free structure
    free (tree);
    return NULL;
}	// AVL_Destroy

/*	=============== _destroyAVL ==============
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	Pre      root is a pointer to a valid tree or subtree.
	Post     All data and head structure have been deleted.
	Return   null head pointer
*/
void _destroyAVL (AVL_NODE *root, void (*freeStruct) (void* dataPtr)) {
    if (root) {
        _destroyAVL (root->left, freeStruct);
        freeStruct (root->dataPtr);
        _destroyAVL (root->right, freeStruct);
        free (root);
    }
    return;
}	// _destroyAVL

/*  ============================= AVL_Print =============================
    To correctly visualize the tree when turned sideways, the actual
	traversal is RNL.
	Pre	 Tree must be initialized. Null tree is OK.
		 Level is node level: root == 0
	Post Tree has been printed.
*/
void AVL_Print (AVL_TREE *tree, void (*process) (void* dataPtr)) {
    _print (tree->root, 0, process);
    return;
}   // AVL_PRINT

/*  ============================= _print =============================
/*  This function uses recursion to print the tree. At each level, the
    level number is printed along with the node contents (an integer).
    Pre		root is the root of a tree or subtree
            level is the level of the tree: tree root is 0
    Post    Tree has been printed.
*/
void _print (AVL_NODE *root, int level, void (*process)(void* dataPtr)) {
    int i;

    if ( root ) {
        _print ( root->right, level + 1, process);

        printf( "bal %3d: Level: %3d", root->bal, level );
        for (i = 0; i <= level; i++ )
            printf ("---" );
        process(root->dataPtr);

        if (root->bal == LH)
            printf( " (LH)\n");
        else if (root->bal == RH)
            printf( " (RH)\n");
        else
            printf( " (EH)\n");

        _print ( root->left, level + 1, process);
    }
    return;
}
