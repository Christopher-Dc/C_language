/*
 *  Gestore (modulare) di job.
 *  Il gestore e' rappresentato dal puntatore sch a una
 *  struttura di tipo scheduler, che contiene una coda
 *  prioritaria (gestita come ADT di prima categoria da
 *  una libreria esterna) e il numero di pc liberi.
 *  Il main contiene un solo ciclo con cui l'utente puo'
 *  richiamare le due funzioni proprie del gestore, cioe'
 *  la richiesta di esecuzione di un nuovo job e la
 *  comunicazione che uno dei job in esecuzione su uno
 *  dei pc e' terminata, facendo agire il gestore di
 *  conseguenza.
 *  ATTENZIONE: l'utente NON specifica quale job termina,
 *  ma solo che uno di essi si e' concluso. Per tale motivo,
 *  lo stato dei singoli pc (libero/occupato, con il job
 *  eventualmente in esecuzione) non viene registrato.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "job.h"
#include "pq.h"

#define NUM   3
#define MAX 100

typedef struct {
  int num;
  pq_t *pq;
} scheduler_t;

scheduler_t *sch_alloc(void);
int sch_insert(scheduler_t *, job_t *);
int sch_extract(scheduler_t *);
void sch_free(scheduler_t *);

int main(void)
{
  scheduler_t *sch=sch_alloc();
  char cmd[3];
  job_t *j;
  int end=0;

  while (!end) {
    printf("\nOperazioni disponibili:\n");
    printf("  (I)nserisci nuovo job da eseguire\n");
    printf("  (T)ermina un job in esecuzione\n");
    printf("  (F)ine\nScelta: ");
    scanf("%s", cmd);

    switch (tolower(cmd[0])) {
      case 'i':
	    printf("Inserisci job: ");
	    j = job_read(stdin);
        if (j == NULL)
          printf("Errore in lettura\n");
        else
          sch_insert(sch, j);
        break;
      case 't':
        sch_extract(sch);
        break;
      case 'f':
        end = 1;
        break;
      default:
        printf("Opzione sconosciuta\n");
    }
  }

  sch_free(sch);
  return 0;
}

scheduler_t *sch_alloc(void)
{
  scheduler_t *sch=(scheduler_t *)malloc(sizeof(scheduler_t));
  if (sch == NULL) {
    printf("Errore di allocazione di memoria\n");
    return NULL;
  }
  sch->num = NUM;
  sch->pq = pq_alloc(MAX);
  if (sch->pq == NULL) {
    printf("Errore di allocazione di memoria\n");
    free(sch); /* libera memoria gia' allocata per sch */
    return NULL;
  }

  return sch;
}

int sch_insert(scheduler_t *sch, job_t *j)
{
  printf("Job ");
  job_print(stdout, j);
  printf(" acquisito. ");

  if (sch->num > 0) {
    /* almeno un pc disponibile */
    sch->num--;
    printf("Job lanciato su pc.\n");

    /* qui si dovrebbe eseguire materialmente il comando: */
    /* dealloco solo la memoria associata al job in esecuzione */
    job_free(j);
    return 0;
  }

  if (pq_full(sch->pq)) {
    /* pc tutti occupati e coda piena */
    printf("Coda piena! Job ignorato.\n");
    job_free(j);
    return 1;
  }

  /* inserisco nella coda */
  pq_insert(sch->pq, j);
  printf("Job messo in coda.\n");

  return 0;
}

int sch_extract(scheduler_t *sch)
{
  job_t *j;

  if (sch->num >= NUM) {
    /* tutti i pc erano gia' liberi: non dovrebbe mai accadere */
    printf("Errore: nessun job in esecuzione!\n");
    return 1;
  }

  printf("Job terminato. ");
  sch->num++;

  if (pq_empty(sch->pq)) {
    /* coda vuota: nulla da fare */
    printf("Nessun job in attesa.\n");
    return 0;
  }

  /* almeno un job in attesa: estraggo dalla coda ed eseguo */
  j = pq_extract(sch->pq);
  printf("Job ");
  job_print(stdout, j);
  printf(" lanciato su pc.\n");
  sch->num--;

  /* come prima, dealloco la memoria associata al job lanciato */
  job_free(j);
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
