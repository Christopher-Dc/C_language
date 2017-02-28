/********************************************************************/
/**                           QUEUE.H                              **/
/********************************************************************/

#ifndef _QUEUE_INCLUDED
#define _QUEUE_INCLUDED

#include "dato.h"

#define QUEUE_ERROR 0

typedef struct queue queue_t;

queue_t *queue_alloc(int size);
void queue_free(queue_t *qp);
int queue_insert(queue_t *qp, dato_t d);
int queue_extract(queue_t *qp, dato_t *dp);
void queue_print(FILE *fp, queue_t *qp);
queue_t *queue_load(FILE *fp);

#endif
