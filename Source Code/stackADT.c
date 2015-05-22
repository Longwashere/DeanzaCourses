/*
   Stack Abstract Data Type - function definitions:

           . createStack
           . pushStack
           . popStack
           . stackTop
           . destroyStack
           . emptyStack
           . fullStack
           . stackCount

*/
#include <stdio.h>
#include <stdlib.h>

#include "stackADT.h"

/* ======================== createStack ======================== */
/* This function creates an empty stack.
     PRE    Nothing
     POST   Returns: - pointer to a header of an empty stack
                     - NULL in case of overflow
*/

STACK *createStack ( void ) {
    STACK *stack;

    stack = ( STACK *) malloc( sizeof( STACK ) );
    if( stack ) {
        stack->top   = NULL;
        stack->count = 0;
    }

    return stack;

}// createStack

/* ======================== pushStack ======================== */
/* This function inserts an item into  the stack.
     PRE    stack - a pointer to the stack
	        dataPtr - pointer to the data to be inserted
     POST   Returns: - 1 if success;
                     - 0 if heap overflow
*/
int pushStack( STACK *stack, void * dataInPtr ) {
    STACK_NODE *newPtr;
    int success = 0;

    newPtr = ( STACK_NODE *) malloc( sizeof( STACK_NODE ) );
    if( newPtr != NULL ) {
        success = 1;
        newPtr->dataPtr = dataInPtr;
        newPtr->link = stack->top;
        stack->top   = newPtr;
        (stack->count)++;
    }

    return success;

}// pushStack

/* ======================== popStack ======================== */
/* This function deletes an item from the stack.
     PRE    stack - a pointer to the stack
     POST   Returns: - a pointer to user data if success;
                     - NULL if the stack is empty
*/
void *popStack( STACK *stack ) {
    void *dataOutPtr = NULL;
    STACK_NODE *temp;

    if( stack->count > 0 ) {
        temp = stack->top;
        dataOutPtr = stack->top->dataPtr;
        stack->top = stack->top->link;
        free( temp );
        ( stack->count )--;
    }

    return dataOutPtr;

}// popStack


/* ======================== stackTop ======================== */
/* This function retrieves the data from the top of the stack
   without changing the stack.
     PRE    stack - a pointer to the stack
     POST   Returns: - the data pointer if success;
                     - NULL if the stack is empty
*/
void *stackTop( STACK *stack ) {

    void *dataOutPtr = NULL;

    if( stack->count > 0 )
        dataOutPtr = stack->top->dataPtr;

    return dataOutPtr;

}// stackTop


/* ======================== destroyStack ======================== */
/* This function releases all nodes to the heap
     PRE     stack - a pointer to the stack
     POST    Returns: NULL
*/
STACK *destroyStack( STACK *stack ) {

    STACK_NODE *temp;

    if( stack ) {
        while( stack->top != NULL ) {
            free( stack->top->dataPtr );
            temp = stack->top;
            stack->top = stack->top->link;
            free( temp );
        }
        free( stack );
    }

    return NULL;

}// destroyStack

/* ======================== emptyStack ======================== */
/* This function determines if a stack is empty
     PRE     stack - a pointer to the stack
     POST    Returns: - 1 if empty;
	                  - 0 if data are in the stack
*/
int emptyStack  ( STACK *stack ) {
    return (stack->count == 0);

}// emptyStack

/* ======================== fullStack ======================== */
/* This function determines if a stack is full
     PRE     stack - a pointer to the stack
     POST    Returns: - 1 if no memory available (full)
	                  - 0 otherwise
*/
int fullStack  ( STACK *stack ) {
    STACK_NODE *temp;
    int         success = 1;

    if( temp = (STACK_NODE *) malloc (sizeof( STACK_NODE ))) {
        free( temp );
        success = 0;
    }
    return success;
}// fullStack

/* ======================== stackCount ======================== */
/* This function returns the number of elements in the stack.
     PRE     stack - a pointer to the stack
     POST    count returned
*/
int stackCount  ( STACK *stack ) {

    return stack->count;
}// stackCount
