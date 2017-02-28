/********************************************************************/
/**                       PQ.C per list                            **/
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

typedef struct node node_t;

struct node {
  job_t *j;
  node_t *next;
};

struct pq {
  node_t *head;
  int num;
};

static node_t *alloc(job_t *j);

pq_t *pq_alloc(int size)
{
  pq_t *pq;

  pq = (pq_t *)malloc(sizeof(pq_t));
  if (pq == NULL) {
    printf("Errore di allocazione di memoria\n");
    return NULL;
  }

  pq->head = NULL;
  pq->num = 0;

  return pq;
}

void pq_free(pq_t *pq)
{
  node_t *node, *head;
  
  if (pq != NULL) {
    head = pq->head;
    while (head != NULL) {
      node = head;
      head = head->next;
      job_free(node->j);
      free(node);
    }
  }
  free(pq);
}

int pq_insert(pq_t *pq, job_t *j)
{
  node_t *node, *ptr;

  if (pq==NULL || (node=alloc(j))==NULL) {
    return 1;
  }

  pq->num++;
  if (pq->head==NULL || job_compare(j, pq->head->j)>0) {
    node->next = pq->head;
    pq->head = node;
  } else {
    ptr = pq->head;
    while (ptr->next!=NULL && job_compare(ptr->next->j, j)>0) {
      ptr = ptr->next;
    }
    node->next = ptr->next;
    ptr->next = node;
  }

  return 0;
}

job_t *pq_extract(pq_t *pq)
{
  node_t *node;
  job_t *j;

  if (pq==NULL || (node=pq->head)==NULL) {
    return NULL;
  }

  pq->head = node->next;
  pq->num--;
  j = node->j;
  free(node);
  return j;
}

static node_t *alloc(job_t *j)
{
  node_t *p;

  p = (node_t *)malloc(sizeof(node_t));
  if (p == NULL) {
    printf("Errore: memoria dinamica insufficiente\n");
    return NULL;
  }

  p->j = j;
  p->next = NULL;
  return p;
}

int pq_full(pq_t *pq)
{
  return 0;
}

int pq_empty(pq_t *pq)
{
  return !pq || pq->num<=0;
}

int pq_num(pq_t *pq)
{
  return pq ? pq->num : 0;
}
