/*
   STACK ADT with header structure:
              - type definitions
			  - protoype declarations
*/

typedef struct stackNode {
    void             *dataPtr;
    struct stackNode *link;
} STACK_NODE;


typedef struct {
    int         count;
    STACK_NODE *top;
} STACK;


int    pushStack   ( STACK *stack, void * dataInPtr );
void  *popStack    ( STACK *stack );
void  *stackTop    ( STACK *stack );
STACK *createStack ( void );
STACK *destroyStack( STACK *stack );
int	   emptyStack  ( STACK *stack );
int    fullStack   ( STACK *stack );
int    stackCount  ( STACK *stack );
