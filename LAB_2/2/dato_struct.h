/********************************************************************/
/**                     DATO.H per struct                          **/
/********************************************************************/

#ifndef _DATO_INCLUDED
#define _DATO_INCLUDED

#define DATA_ERROR 0

typedef struct dato * dato_t;

int data_read(FILE *fp, dato_t *dp);
void data_print(FILE *fp, dato_t d);
int data_compare(dato_t d1, dato_t d2, int key);
void data_free(dato_t d);

#endif
