/*
 *  Libreria modulare sugli stack. Lo stack e' gestito
 *  come ADT di prima categoria, e opera su un tipo di
 *  dato del tutto generico.
 *  Occorre linkare il main con i file opportuni per
 *  utilizzare l'implementazione su vettore o su lista,
 *  nonche' per selezionare il tipo di dato desiderato.
 *  Il formato del file su cui lo stack viene salvato
 *  cambia leggermente con l'implementazione: se si usa
 *  quella su vettore, sulla prima riga viene scritta
 *  la dimensione dello stack.
 *
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dato.h"
#include "stack.h"

#define MAXC 20

int main(void)
{
  int end=0, size;
  char riga[MAXC];
  FILE *fp=NULL;
  stack_t *sp=NULL;
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
	    stack_free(sp);
	    printf("Dimensione dello stack: ");
	    fgets(riga, MAXC, stdin);
	    sscanf(riga, "%d", &size);
	    sp = stack_alloc(size);
	    if (sp == NULL) {
	      printf("Errore nella creazione dello stack\n");
	      return 0;
	    }
	    break;
      case 'i':
	    printf("Dato: ");
	    if (data_read(stdin, &d) == DATA_ERROR)
	      printf("Errore in lettura\n");
	    else if (stack_push(sp, d) == STACK_ERROR)
	      printf("Errore in inserzione\n");
	    break;
      case 'e':
	    if (stack_pop(sp, &d) == STACK_ERROR)
	      printf("Errore in estrazione\n");
	    else {
	      printf("Dato estratto: ");
	      data_print(stdout, d);
	    }
	    break;
      case 'v':
	    stack_print(stdout, sp);
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
	      stack_print(fp, sp);
	      fclose(fp);
	      fp = NULL;
	    } else
          stack_print(stdout, sp);
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
	    stack_free(sp);
	    sp = stack_load(fp);
        fclose(fp);
	    break;
      case 'f':
	    end = 1;
	    break;
      default:
	    printf("Opzione sconosciuta\n");
    }
  }

  stack_free(sp);
  return 1;
}
