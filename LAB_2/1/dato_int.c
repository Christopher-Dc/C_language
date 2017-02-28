/********************************************************************/
/**                      DATO.C per int                            **/
/********************************************************************/

#include <stdio.h>
#include "dato.h"

#define MAXC 30

int data_read(FILE *fp, dato_t *dp)
{
  char riga[MAXC];

  if (fgets(riga, MAXC, fp) == NULL)
    return DATA_ERROR;
  sscanf(riga, "%d", dp);
  return !DATA_ERROR;
}

void data_print(FILE *fp, dato_t dato)
{
  fprintf(fp, "%d\n", dato);
}

int data_compare(dato_t d1, dato_t d2, int key)
{
  return d1-d2;
}

void data_free(dato_t dato)
{
  return;
}
