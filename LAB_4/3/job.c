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
  int receive;
  int duration;
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
    job->receive = readTime(fp);
    job->duration = readTime(fp);
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

int readTime(FILE *fp)
{
  int h, m, s;

  fscanf(fp, "%d:%d:%d", &h, &m, &s);

  return 3600*h + 60*m +s;
}

void printTime(FILE *fp, int t)
{
  int h, m, s;

  h = t / 3600;
  m = (t / 60) % 60;
  s = t % 60;
  fprintf(fp, "%02d:%02d:%02d", h, m, s);
}

int startTime(job_t *j)
{
  return j->receive;
}

int endTime(job_t *j, int start)
{
  return start + j->duration;
}

int cmpTime(int t1, int t2)
{
  return t1 - t2;
}

