/********************************************************************/
/**                            JOB.C                               **/
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job.h"

#define MAXC 50

struct job {
  char *name;
  int prio;
};

job_t *job_read(FILE *fp)
{
  job_t *job;
  char name[MAXC];
  int prio;

  if (fscanf(fp, "%s %d", name, &prio) != EOF) {
    job = (job_t *)malloc(sizeof(job_t));
    if (job == NULL) {
      return NULL;
    }
    job->name = strdup(name);
    if (job->name == NULL) {
      free(job);
      return NULL;
    }
    job->prio = prio;
  } else {
    return NULL;
  }

  return job;
}

void job_print(FILE *fp, job_t *j)
{
  fprintf(fp, "%s (p=%d)", j->name, j->prio);
}

int job_compare(job_t *j1, job_t *j2)
{
  return j1->prio - j2->prio;
}

void job_free(job_t *j)
{
  free(j->name);
  free(j);
}
