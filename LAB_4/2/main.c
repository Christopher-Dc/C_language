#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Item.h"
#include "BST.h"

#define MAXC 80

Item NULLItem = EMPTYitem;

int main(void)
{
   BST tree = BSTinit();
   int min, max;
   Item din, dout;
   int num, end = 0;
   char riga[MAXC];
   FILE *fp = NULL;

  while (!end) {
     printf("\nOperazioni disponibili:\n  (N)uovo BST\n  (I)nserisci dato\n");
     printf("  (C)erca dato\n  (P)redecessore e Successore\n");
     printf("  (E)limina dato\n  (V)isualizza\n  (S)alva su file\n  (L)eggi da file\n");
     printf("  (F)ine\nFai una scelta: ");
     scanf("%s", riga);

    riga[0] = tolower(riga[0]);
    switch (riga[0]) {
      case 'n':
        BSTfree(tree);
	    tree = BSTinit();
	    break;

      case 'i':
	    din = item_read(stdin);
	    if (!eq(key(din), key(NULLItem)))
	       BSTinsert_leafR(tree, din);
        break;

      case 'e':
	    din = item_read_key(stdin);
        if (!eq(key(din), key(NULLItem))) {
	       BSTdelete(tree, din);
        }
	    break;

      case 'c':
	    din = item_read_key(stdin);
	    if (!eq(key(din), key(NULLItem))) {
	       dout = BSTsearch(tree, din.code);
	       if (!eq(key(dout), key(NULLItem))) {
	          printf("Dato trovato: ");
	          item_print(dout, stdout);
           } else {
	          printf("Dato non trovato\n");
           }
        }
	    break;

      case 'f':
	    end = 1;
	    break;

      case 'l':
	    printf("Nome del file: ");
	    scanf("%s", riga);

	    fp = fopen(riga, "r");
	    if (fp == NULL) {
	       printf("ERRORE in apertura di %s\n", riga);
        } else {
	       BSTfree(tree);
	       tree = BSTread(fp);
	       fclose(fp);
        }
        break;

      case 'v':
        BSTsortinorder(tree, item_print, stdout);
	    break;

      case 's':
	    printf("Nome del file: ");
	    scanf("%s", riga);

	    fp = fopen(riga, "w");
	    if (fp == NULL) {
	       printf("ERRORE in apertura di %s\n", riga);
        } else {
            BSTsortpreorder(tree, item_print, fp);
            fclose(fp);
        }
	    break;

      case 'p':
	    din = item_read_key(stdin);
	    if (!eq(key(din), key(NULLItem))) {
	       dout = BSTsucc(tree, din);
	       if (!eq(key(dout), key(NULLItem))) {
	          printf("Successore: ");
	          item_print(dout, stdout);
           } else {
	          printf("Successore non trovato\n");
           }
	       dout = BSTpred(tree, din);
	       if (!eq(key(dout), key(NULLItem))) {
	          printf("Predecessore: ");
	          item_print(dout, stdout);
           } else {
	          printf("Predecessore non trovato\n");
           }
        }
	    break;

      default:
	    printf("Opzione sconosciuta\n");
	    break;
    }

    if (riga[0] == 'i' || riga[0] == 'e') {
       /* inserzione/estrazione: stampa statistiche */
       num = BSTdepth(tree, &min, &max);
       if (num > 0) {
	      printf("Numero di nodi : %d\n", num);
	      printf("Distanza MASSIMA dalla radice: %d\n", max);
	      printf("Distanza MINIMA dalla radice : %d\n", min);
       }
    }
  }

  BSTfree(tree);

  return 0;
}
