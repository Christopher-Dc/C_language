/********************************************************************/
/**                            JOB.H                               **/
/********************************************************************/

#ifndef _JOB_INCLUDED
#define _JOB_INCLUDED

typedef struct job job_t;

job_t *job_read(FILE *fp);
void job_print(FILE *fp, job_t *j);
int job_compare(job_t *j1, job_t *j2);
void job_free(job_t *j);

int readTime(FILE *fp);
void printTime(FILE *fp, int t);
int startTime(job_t *j);
int endTime(job_t *j, int start);
int cmpTime(int t1, int t2);

#endif
