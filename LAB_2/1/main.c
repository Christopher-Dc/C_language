/*
 *  Libreria modulare sulle code. La coda e' gestita
 *  come ADT di prima categoria e opera su un tipo di
 *  dato del tutto generico.
 *  Occorre linkare il main con i file opportuni per
 *  utilizzare l'implementazione su vettore o su lista,
 *  nonche' per selezionare il tipo di dato desiderato.
 *  Il formato del file su cui la coda viene salvata
 *  cambia leggermente con l'implementazione: se si usa
 *  quella su vettore, sulla prima riga viene scritta
 *  la dimensione della coda.
 *
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dato.h"
#include "queue.h"

#define MAXC 20

int main(void)
{
  int end=0, size;
  char riga[MAXC];
  FILE *fp=NULL;
  queue_t *qp=NULL;
  dato_t d;

  while (!end) {
    printf("\nOperazioni disponibili:\n");
    printf("  (N)uovo\n  (I)nserisci\n");
    printf("  (E)strai\n  (V)isualizza\n");
    printf("  (S)alva\n  (C)arica\n");
    printf("  (F)ine\nScelta: ");
    fgets(riga, MAXC, stdin);

    switch (tolower(riga[0])) {
      case 'n':
  	    queue_free(qp);
	    printf("Dimensione della coda: ");
	    fgets(riga, MAXC, stdin);
	    sscanf(riga, "%d", &size);
	    qp = queue_alloc(size);
	    if (qp == NULL) {
	      printf("Errore nella creazione della coda\n");
	      return 0;
	    }
	    break;
      case 'i':
	    printf("Dato: ");
	    if (data_read(stdin, &d) == DATA_ERROR)
	      printf("Errore in lettura\n");
	    else if (queue_insert(qp, d) == QUEUE_ERROR)
	      printf("Errore in inserzione\n");
	    break;
      case 'e':
	    if (queue_extract(qp, &d) == QUEUE_ERROR)
	      printf("Errore in estrazione\n");
	    else {
	      printf("Dato estratto: ");
	      data_print(stdout, d);
	    }
	    break;
      case 'v':
	    queue_print(stdout, qp);
	    break;
      case 's':
	    printf("Nome del file: ");
	    fgets(riga, MAXC, stdin);
	    if (riga[strlen(riga)-1] == '\n')
	      riga[strlen(riga)-1]='\0';
	    if (strcmp(riga, "stdout") != 0) {
	      fp = fopen(riga, "w");
	      if (fp == NULL) {
	        printf("Errore di apertura del file\n");
	        break;
	      }
	      queue_print(fp, qp);
	      fclose(fp);
	      fp = NULL;
	    } else
          queue_print(stdout, qp);
	    break;
      case 'c':
	    printf("Nome del file: ");
	    fgets(riga, MAXC, stdin);
	    if (riga[strlen(riga)-1] == '\n')
	      riga[strlen(riga)-1]='\0';
        fp = fopen(riga, "r");
        if (fp == NULL) {
          printf("Errore di apertura del file\n");
          break;
        }
	    queue_free(qp);
	    qp = queue_load(fp);
        fclose(fp);
	    break;
      case 'f':
	    end = 1;
	    break;
      default:
	    printf("Opzione sconosciuta\n");
    }
  }

  queue_free(qp);
  return 1;
}
