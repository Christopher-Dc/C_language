/********************************************************************/
/**                             PQ.H                               **/
/********************************************************************/

#ifndef _PQ_INCLUDED
#define _PQ_INCLUDED

#include "job.h"

typedef struct pq pq_t;

pq_t *pq_alloc(int size);
void pq_free(pq_t *pq);
int pq_insert(pq_t *pq, job_t *j);
job_t *pq_extract(pq_t *pq);
int pq_full(pq_t *pq);
int pq_empty(pq_t *pq);
int pq_num(pq_t *pq);

#endif
