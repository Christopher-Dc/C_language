/*
 *  Concatenazione di stringhe.
 *
 *  Dato un vettore di stringhe (letto da file), occorre trovare
 *  la stringa piu' lunga ottenibile concatenando le parole
 *  lette (due parole sono concatenabili se le ultime lettere
 *  della prima corrispondono alle prime due della seconda; nella
 *  concatenazione, queste lettere appaiono solo una volta).
 *  Ogni parola e' utilizzabile un numero massimo di volte deciso
 *  dall'utente.
 *  La stringa risultato NON viene costruita materialmente: essa,
 *  infatti, puo' essere ricavata semplicemente conoscendo la
 *  sequenza di stringhe di partenza che occorre concatenare.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/********************************************************************
  DEFINIZIONE STRUTTURA DATI, COSTANTI E MACRO
********************************************************************/

typedef struct {
   char *nome;
   int nRip;
} stringa;

#define DEBUG           0
#define SUCCESS         1
#define FAILURE         2
#define MAX_LEN       101
#define NCC             2  /* numero caratteri di confronto */

#define strend(nome)                   ((nome) + strlen(nome) - NCC)
#define strmid(nome)                   ((nome) + NCC)
#define is_usable(array, i)            (array[i].nRip > 0)
#define is_chainable(array, i, j)  \
         (strncmp(strend(array[i].nome), array[j].nome, NCC)==0)

/********************************************************************
  PROTOTIPI
********************************************************************/
stringa *read_file(char *filename, int *nelem, int nrip);
void save_solution(int *final, int *partial, int dim);
int chain_recur(stringa *array, int nelem, int *partial, int curr_len,
		        int *final, int max_len, int level);
int print_result(stringa *array, int *final, int max_len, char *filename);
void print_max_word(stringa *array, int *sol, FILE *fp);


/********************************************************************
  Funzione di lettura del vettore di stringhe
********************************************************************/
stringa *read_file(char *filename, int *nelem, int nrip)
{
   int n, i, len;
   char word[MAX_LEN];
   stringa *array;
   FILE *fin;

   fin = fopen(filename, "r");
   if (fin == NULL) {
      printf("Errore di apertura file %s\n", filename);
      return NULL;
   }

   fgets(word, MAX_LEN-1, fin);
   sscanf(word, "%d", &n);

   /* allocazione del vettore di stringhe */
   array = (stringa *)malloc(n * sizeof(stringa));
   if (array == NULL) {
      printf("Errore di allocazione memoria\n");
      fclose(fin);
      return NULL;
   }

   /* lettura del file */
   for (i=0; i<n; i++) {
      fscanf(fin, "%s", word);
      len = strlen(word) + 1;
      array[i].nome = (char *)malloc(len * sizeof(char));
      if (array[i].nome == NULL) {
	     printf("Errore di allocazione memoria\n");
	     fclose(fin);
	     return NULL;
      }
      strcpy(array[i].nome, word);
      array[i].nRip = nrip;
   }

   fclose(fin);
   *nelem = n;
   return array;
}


/********************************************************************
  Funzione di salvataggio della soluzione corrente
********************************************************************/
void save_solution(int *final, int *partial, int dim)
{
   int i;

   for (i=0; i<dim; i++)
      final[i] = partial[i];
   final[dim] = -1;
}


/********************************************************************
  Funzione di concatenazione delle stringhe (ricorsiva)
********************************************************************/
int
chain_recur (
   stringa *array,
   int nelem,
   int *partial,
   int curr_len,
   int *final,
   int max_len,
   int level
)
{
   int i, last;

   last = (level == 0) ? -1 : partial[level-1];
   for (i=0; i<nelem; i++) {
      if ((last==-1) || (is_usable(array, i) && is_chainable(array, last, i))) {
	     partial[level] = i;
	     array[i].nRip--;
	     curr_len += strlen(strmid(array[i].nome));
	     if (curr_len > max_len) {
	        max_len = curr_len;
	        save_solution(final, partial, level+1);
         }
	     if (DEBUG) {
	        printf("Chaining result: ");
	        print_max_word(array, partial, stdout);
         }
	     max_len = chain_recur(array, nelem, partial, curr_len, final, max_len, level+1);
	     partial[level] = -1;
	     array[i].nRip++;
	     curr_len -= strlen(strmid(array[i].nome));
      }
   }
   return max_len;
}

/********************************************************************
  Funzione di stampa della stringa piu' lunga
********************************************************************/
void print_max_word(stringa *array, int *sol, FILE *fp)
{
   int i, j;

   fprintf(fp, "%s", array[sol[0]]);
   i = 1;
   while (sol[i] != -1) {
      j = sol[i++];
      fprintf(fp, "%s", strmid(array[j].nome));
   }
   fprintf(fp, "\n");
}

/********************************************************************
  Funzione di stampa del risultato
********************************************************************/
int print_result(stringa *array, int *final, int max_len, char *filename)
{
   FILE *fp;
   int i, j;

   if (strcmp(filename, "stdout") == 0) {
      fp = stdout;
   } else {
      fp = fopen(filename, "w");
      if (fp == NULL) {
	     fprintf (stderr, "Errore di apertura file %s.\n", filename);
	     return 0;
      }
   }

   /* cerco il sottoinsieme di parole che danno la soluzione */
   fprintf (fp, "\nSottoinsieme di parole da cui si ha la soluzione:\n");
   i=0;
   while (final[i] != -1) {
      j = final[i++];
      if (array[j].nRip > 0) {
	     fprintf(fp, "  %s\n", array[j].nome);
	     /* sporco il campo nRip della struttura, ma tanto non serve piu' ... */
	     array[j].nRip = -1;
      }
   }

   /* stampa della soluzione */
   fprintf(fp, "\nParola piu' lunga ottenibile (%d caratteri):\n", max_len);
   print_max_word(array, final, fp);

   if (strcmp(filename, "stdout") != 0) {
      fclose (fp);
   }
   system("pause");
   return 1;
}


/********************************************************************
  MAIN
********************************************************************/
int main(void)
{
   char fname[MAX_LEN], word[MAX_LEN];
   stringa *array;
   int i, nelem, nrip, *final, *partial, len;

   printf("Nome del file di ingresso: ");
   fgets(word, MAX_LEN-1, stdin);
   sscanf(word, "%s", fname);
   printf("Numero di ripetizioni permesse: ");
   fgets(word, MAX_LEN-1, stdin);
   sscanf(word, "%d", &nrip);

   array = read_file(fname, &nelem, nrip);
   if (array == NULL)
      return FAILURE;

   /* allocazione di due vettori temporanei per la funzione
      ricorsiva: conterranno la soluzione corrente e quella finale */
   partial = (int *)malloc((nelem * nrip +1) * sizeof(int));
   final = (int *)malloc((nelem * nrip +1) * sizeof(int));
   for (i=0; i<=nelem*nrip; i++) {
      partial[i] = final[i] = -1;
   }

   len = chain_recur(array, nelem, partial, NCC, final, NCC, 0);

   printf("Nome del file di uscita: ");
   scanf("%s", fname);
   if (print_result(array, final, len, fname) == 0) {
      return FAILURE;
   }

   /* deallocazione della memoria allocata */
   for (i=0; i<nelem; i++) {
      free(array[i].nome);
   }
   free(array);
   free(final);
   free(partial);
   return SUCCESS;
}
