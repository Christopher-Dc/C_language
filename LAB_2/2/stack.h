/********************************************************************/
/**                           STACK.H                              **/
/********************************************************************/

#ifndef _STACK_INCLUDED
#define _STACK_INCLUDED

#include "dato.h"

#define STACK_ERROR 0

typedef struct stack stack_t;

stack_t *stack_alloc(int size);
void stack_free(stack_t *sp);
int stack_push(stack_t *sp, dato_t d);
int stack_pop(stack_t *sp, dato_t *dp);
void stack_print(FILE *fp, stack_t *sp);
stack_t *stack_load(FILE *fp);

#endif
