/********************************************************************/
/**                       PQ.C per heap                            **/
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#define HEAP_LEFT(i)      (2*(i)+1)
#define HEAP_RIGHT(i)     (2*(i)+2)
#define HEAP_PARENT(i)  (((i)-1)/2)

struct pq {
  job_t **jobs;
  int     num;
  int     size;
};

static void heapify(pq_t *pq, int k);

pq_t *pq_alloc(int size)
{
  pq_t *pq;

  pq = (pq_t *)malloc(sizeof(pq_t));
  if (pq == NULL) {
    printf("Errore di allocazione di memoria\n");
    return NULL;
  }

  pq->size = size;
  pq->num = 0;
  pq->jobs = (job_t **)malloc(size*sizeof(job_t *));
  if (pq->jobs == NULL) {
    printf("Errore di allocazione di memoria\n");
    free(pq); /* libera memoria gia' allocata per pq */
    return NULL;
  }

  return pq;
}

void pq_free(pq_t *pq)
{
  int i;

  if (pq != NULL) {
    for (i=0; i<pq->num; i++) {
      job_free(pq->jobs[i]);
    }
    free(pq->jobs);
  }
  free(pq);
}

int pq_insert(pq_t *pq, job_t *j)
{
  int k, p;

  if (pq_full(pq)) {
    return 1;
  }

  k = pq->num++;
  p = HEAP_PARENT(k);
  while (k>0 && job_compare(pq->jobs[p], j)<0) {
    pq->jobs[k] = pq->jobs[p];
    k = p; p = HEAP_PARENT(k);
  }
  pq->jobs[k] = j;

  return 0;
}

job_t *pq_extract(pq_t *pq)
{
  job_t *j;

  if (pq_empty(pq)) {
    return NULL;
  }

  j = pq->jobs[0];
  pq->num--;
  if (pq->num > 0) {
    pq->jobs[0] = pq->jobs[pq->num];
    heapify(pq, 0);
  }
  pq->jobs[pq->num] = NULL;

  return j;
}

static void heapify(pq_t *pq, int k)
{
  int l, r, key;
  job_t *j;

  l = HEAP_LEFT(k); r = HEAP_RIGHT(k); key = k;

  if (l<pq->num && job_compare(pq->jobs[l], pq->jobs[key])>0) {
    key = l;
  }
  if (r<pq->num && job_compare(pq->jobs[r], pq->jobs[key])>0) {
    key = r;
  }
  if (key != k) {
    j = pq->jobs[k];
    pq->jobs[k] = pq->jobs[key];
    pq->jobs[key] = j;
    heapify(pq, key);
  }
}

int pq_full(pq_t *pq)
{
  return !pq || pq->num>=pq->size;
}

int pq_empty(pq_t *pq)
{
  return !pq || pq->num<=0;
}

int pq_num(pq_t *pq)
{
  return pq ? pq->num : 0;
}
