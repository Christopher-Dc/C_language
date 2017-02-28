/*
 *  Gestore (modulare) di job.
 *  Il gestore e' rappresentato dal puntatore sch a una
 *  struttura di tipo scheduler, che contiene una coda
 *  prioritaria (gestita come ADT di prima categoria da
 *  una libreria esterna), il numero di pc occupati e
 *  due vettori che descrivono l'assegnazione dei job ai
 *  vari pc e il tempo in cui tali job terminano.
 *  Il main si occupa solo di leggere il file con l'elenco
 *  dei job da eseguire, richiamando per ognuno di essi la
 *  funzione di inserimento: e' tale funzione che ispeziona
 *  lo stato dei pc, eventualmente modificandolo, e lancia
 *  in esecuzione (o mette in attesa) i vari job.
 *  La funzione di estrazione, invece, serve solo a svuotare
 *  i job ancora in esecuzione/attesa una volta che la
 *  lettura del file di ingresso e' terminata.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "job.h"
#include "pq.h"

#define NUM   3
#define MAX 100

typedef struct {
  int num;
  int pc[NUM+1];
  job_t *job[NUM];
  pq_t *pq;
} scheduler_t;

scheduler_t *sch_alloc(void);
int sch_insert(scheduler_t *, job_t *);
int sch_extract(scheduler_t *);
void sch_free(scheduler_t *);

int main(void)
{
  scheduler_t *sch=sch_alloc();
  char name[20];
  FILE *fp;
  job_t *j;

  printf("Nome del file dei job? ");
  scanf("%s", name);
  fp = fopen(name, "r");
  if (fp == NULL) {
    printf("Errore nell'apertura del file!\n");
    return 1;
  }

  do {
    j = job_read(fp);
    if (j != NULL) {
      sch_insert(sch, j);
    }
  } while (j != NULL);
  fclose(fp);

  sch_extract(sch);
  sch_free(sch);
  return 0;
}

scheduler_t *sch_alloc(void)
{
  scheduler_t *sch;
  int i;

  sch=(scheduler_t *)malloc(sizeof(scheduler_t));
  if (sch == NULL) {
    printf("Errore di allocazione di memoria\n");
    return NULL;
  }
  sch->num = 0;
  sch->pq = pq_alloc(MAX);
  if (sch->pq == NULL) {
    printf("Errore di allocazione di memoria\n");
    free(sch); /* libera memoria gia' allocata per sch */
    return NULL;
  }
  for (i=0; i<NUM; i++) {
    sch->pc[i] = INT_MAX;
    sch->job[i] = NULL;
  }
  sch->pc[NUM] = INT_MAX;

  return sch;
}

int sch_insert(scheduler_t *sch, job_t *j)
{
  int i, min, ct, at=startTime(j);
  job_t *wait;

  if (sch->num > 0) {
    /* controllo eventuali cambiamenti nello stato dei pc */
    do {
      min = NUM;
      for (i=0; i<NUM; i++) {
        if (sch->pc[i]<sch->pc[min]) {
          min = i;
	    }
      }
      ct = sch->pc[min];
      if (ct < at) {
        /* termina un job */
        printTime(stdout, ct);
        printf(" > Job ");
        job_print(stdout, sch->job[min]);
        printf(" terminato. ");
        job_free(sch->job[min]);
        sch->pc[min] = INT_MAX;
        sch->job[min] = NULL;
        sch->num--;
        /* vedo se c'e' un altro job da lanciare */
        if (!pq_empty(sch->pq)) {
          wait = pq_extract(sch->pq);
          printf("Job ");
          job_print(stdout, wait);
          printf(" lanciato su pc%d.\n", min);
          sch->pc[min] = endTime(wait, ct);
          sch->job[min] = wait;
          sch->num++;
        } else {
          printf("Nessun job in attesa.\n");
        }
      }
    } while (sch->num>0 && ct<at);
  }

  /* ora prendo in carico il nuovo job */
  printTime(stdout, startTime(j));
  printf(" > Job ");
  job_print(stdout, j);
  printf(" acquisito. ");

  if (sch->num < NUM) {
    /* almeno un pc disponibile */
    sch->num++;
    for (i=0; i<NUM; i++) {
      if (sch->job[i] == NULL) {
        sch->job[i] = j;
        sch->pc[i] = endTime(j, startTime(j));
        printf("Job lanciato su pc%d.\n", i);
        return 0;
      }
    }
  }

  if (pq_full(sch->pq)) {
    /* pc tutti occupati e coda piena */
    printf("Coda piena! Job ignorato.\n");
    job_free(j);
    return 1;
  }

  /* inserisco nella coda */
  pq_insert(sch->pq, j);
  printf("Job messo in attesa.\n");

  return 0;
}

int sch_extract(scheduler_t *sch)
{
  int i, min, ct;
  job_t *wait;

  while (sch->num > 0) {
    min = NUM;
    for (i=0; i<NUM; i++) {
      if (sch->pc[i]<sch->pc[min]) {
        min = i;
      }
    }
    ct = sch->pc[min];

    /* termina un job */
    printTime(stdout, ct);
    printf(" > Job ");
    job_print(stdout, sch->job[min]);
    printf(" terminato. ");
    job_free(sch->job[min]);
    sch->pc[min] = INT_MAX;
    sch->job[min] = NULL;
    sch->num--;
    /* vedo se c'e' un altro job da lanciare */
    if (!pq_empty(sch->pq)) {
      wait = pq_extract(sch->pq);
      printf("Job ");
      job_print(stdout, wait);
      printf(" lanciato su pc%d.", min);
      sch->pc[min] = endTime(wait, ct);
      sch->job[min] = wait;
      sch->num++;
    }
    printf("\n");
  }

  return 0;
}

void sch_free(scheduler_t *sch)
{
  job_t *j;

  while (!pq_empty(sch->pq)) {
    j = pq_extract(sch->pq);
    job_free(j);
  }
  pq_free(sch->pq);
  free(sch);
}
