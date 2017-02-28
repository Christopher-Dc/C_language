/*
 *  Profitti di una pasticceria.
 *
 *  Risolve il problema di calcolare il profitto massimo realizzabile
 *  da una pasticceria, a partire da un insieme di ingredienti a
 *  disposizione (utilizzabili per preparare un insieme di ricette).
 *
 *  Gli ingredienti nella dispensa sono descritti per mezzo di una
 *  struttura composta da una stringa (nome dell'ingrediente) e un
 *  intero (disponibilita' dell'ingrediente).
 *  Le ricette sono descritte da una struttura composta da una
 *  stringa (nome del prodotto), due interi (prezzo del prodotto e
 *  numero di ingredienti necessari alla sua realizzazione), e un
 *  vettore per memorizzare i suoi ingredienti di base.
 *  Questi ultimi NON sono descritti con la stessa struttura usata
 *  per gli ingredienti della dispensa, ma attraverso una struttura
 *  di due interi: uno rappresenta l'indice dell'ingrediente richiesto
 *  all'interno del vettore degli ingredienti disponibili, l'altro
 *  la quantita' richiesta di tale ingrediente. Questo permette un
 *  accesso diretto durante la fase di ricerca, rendendo il programma
 *  piu' veloce (in caso contrario, per ogni ingrediente richiesto,
 *  si sarebbe dovuto eseguire una ricerca nel vettore degli ingredienti
 *  disponibili); inoltre, si evita la duplicazione in memoria delle
 *  stringhe per gli ingredienti.
 *  L'algoritmo di esplorazione funziona nel modo seguente: per ogni
 *  possibile prodotto, si verifica se ci sono in dispensa tutti gli
 *  ingredienti necessari alla sua realizzazione; se si', si aggiorna
 *  la dispensa (rimuovendo gli ingredienti utilizzati) e il valore
 *  del profitto, e si ricorre. In fase di backtrack, si provvede a
 *  rimettere gli ingredienti in dispensa e a riportare il profitto
 *  al suo valore originale. Se ci si trova ad un passo terminale di
 *  ricorsione (nessuna torta preparata), si provvede a confrontare il
 *  valore del profitto trovato con il massimo temporaneo, eventualmente
 *  sostituiendo quest'ultimo.
 *  Si noti che il parametro "start" della funzione ricorsiva serve per
 *  evitare di generare soluzioni simmetriche; il suo utilizzo, comunque,
 *  potrebbe portare a considerare alcuni valori di profitto "parziali"
 *  (ovvero, con la possibilita' di realizzare ancora alcuni prodotti)
 *  come valori finali. Di fatto, tali valori parziali sono sempre minori
 *  del massimo, quindi non interessano.
 *  A tal proposito, si provi a sostituire il ciclo
 *
 *        for (i=start; i<np; i++)
 *
 *  della funzione "esplora" con
 *
 *        for (i=0; i<np; i++)
 *
 *  Il risultato finale non dovrebbe cambiare, ma il tempo di esecuzione
 *  dovrebbe aumentare ...
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1

#define MAX           100

#define FILE_INGR     "ingredienti.txt"
#define FILE_RIC      "ricette.txt"

/********************************************************************
  Strutture dati utilizzate
********************************************************************/

/* struttura per la descrizione degli ingredienti a disposizione */
typedef struct {
   char *nome;
   int quantita;
} ingrediente;

/* struttura per la descrizione degli ingredienti richiesti */
typedef struct {
   int indice;
   int quantita;
} dose;

/* struttura per la descrizione delle ricette */
typedef struct {
   char *nome;
   int prezzo;
   int nn;
   dose *ingredienti;
   int num;       /* quantita' per l'ordine corrente */
   int numBest;   /* quantita' per l'ordine ottimo */
} ricetta;

/********************************************************************
  Prototipi
********************************************************************/
ingrediente *leggi_ingredienti(int *ni_ptr);
ricetta *leggi_prodotti(ingrediente *ingredienti, int ni, int *np_ptr);
int indice_ingrediente(ingrediente *ingredienti, int ni, char *nome);
int esplora(ingrediente *ingredienti, ricetta *prodotti, int np,
            int profitto, int max, int start);
int ingredienti_sufficienti(ricetta *prodotti, int idx, ingrediente *ingredienti);
void aggiorna_ingredienti(ricetta *prodotti, int idx, ingrediente *ingredienti, int segno);

/********************************************************************
  Main
********************************************************************/
int main (void)
{
   ricetta *prodotti;
   ingrediente *ingredienti;
   int i, ni, np, max;

   /* lettura file di ingresso */
   ingredienti = leggi_ingredienti(&ni);
   prodotti = leggi_prodotti(ingredienti, ni, &np);

   /* ricerca dei profitti */
   max = esplora(ingredienti, prodotti, np, 0, 0, 0);

   /* visualizzazione del risultato */
   printf("Profitto massimo: %d\n", max);
   printf("Ordine ideale:\n");
   for (i=0; i<np; i++) {
      if (prodotti[i].numBest != 0) {
         printf("- %d %s\n", prodotti[i].numBest, prodotti[i].nome);
      }
   }
   printf("\n");

   /* deallocazione memoria */
   for (i=0; i<ni; i++) {
      free(ingredienti[i].nome);
   }
   free(ingredienti);
   for (i=0; i<np; i++) {
      free(prodotti[i].nome);
      free(prodotti[i].ingredienti);
   }
   free(prodotti);

   system("PAUSE");
   return EXIT_SUCCESS;
}


/********************************************************************
  Funzione di lettura degli ingredienti
********************************************************************/
ingrediente *leggi_ingredienti(int *ni_ptr)
{
   int i, ni, misura;
   ingrediente *ingredienti;
   char nome[MAX];
   FILE *fp;

   fp = fopen(FILE_INGR, "r");
   if (fp == NULL) {
      printf("File ingredienti non trovato ...\n");
      exit(EXIT_FAILURE);
   }

   fscanf(fp, "%d", &ni);
   ingredienti = (ingrediente *)malloc(ni*sizeof(ingrediente));
   if (ingredienti == NULL) {
      printf("Errore di allocazione memoria ...\n");
      exit(EXIT_FAILURE);
   }

   for (i=0; i<ni; i++) {
      fscanf(fp, "%s %d", nome, &misura);
      ingredienti[i].nome = strdup(nome);
      ingredienti[i].quantita = misura;
   }

   fclose(fp);
   *ni_ptr = ni;
   return ingredienti;
}


/********************************************************************
  Funzione di lettura delle ricette
********************************************************************/
ricetta *leggi_prodotti(ingrediente *ingredienti, int ni, int *np_ptr)
{
   int i, j, np, nn, pos, prezzo, misura;
   ricetta *prodotti;
   char nome[MAX];
   FILE *fp;

   fp = fopen(FILE_RIC, "r");
   if (fp == NULL) {
      printf("File ricette non trovato ...\n");
      exit(EXIT_FAILURE);
   }

   fscanf(fp, "%d", &np);
   prodotti = (ricetta *)malloc(np*sizeof(ricetta));
   if (prodotti == NULL) {
      printf("Errore di allocazione memoria ...\n");
      exit(EXIT_FAILURE);
   }

   for (i=0; i<np; i++) {
      fscanf(fp, "%s %d %d", nome, &prezzo, &nn);
      prodotti[i].nome = strdup(nome);
      prodotti[i].prezzo = prezzo;
      prodotti[i].num = prodotti[i].numBest = 0;
      prodotti[i].nn = nn;
      prodotti[i].ingredienti = (dose *)malloc(nn*sizeof(dose));
      if (prodotti[i].ingredienti == NULL) {
	     printf("Errore di allocazione memoria ...\n");
         exit(EXIT_FAILURE);
      }
      for (j=0; j<nn; j++) {
	     fscanf(fp, "%s %d", nome, &misura);
	     pos = indice_ingrediente(ingredienti, ni, nome);
	     if (pos < 0) {
	        printf("Errore: ingrediente NON trovato ...\n");
	        exit(EXIT_FAILURE);
	     }
	     prodotti[i].ingredienti[j].indice = pos;
	     prodotti[i].ingredienti[j].quantita = misura;
      }
   }

   fclose(fp);
   *np_ptr = np;
   return prodotti;
}


/********************************************************************
  Funzione di ricerca di un ingrediente (dato il nome)
********************************************************************/
int indice_ingrediente(ingrediente *ingredienti, int ni, char *nome)
{
   int i;

   for (i=0; i<ni; i++) {
      if (strcmp(ingredienti[i].nome, nome) == 0)
	     return i;
   }

   return -1;
}

/********************************************************************
  Funzione di esplorazione ricorsiva
********************************************************************/
int esplora(ingrediente *ingredienti, ricetta *prodotti, int np,
            int profitto, int max, int start)
{
   int i, flag=1;

   for (i=start; i<np; i++) {
      if (ingredienti_sufficienti(prodotti, i, ingredienti)) {
	     aggiorna_ingredienti(prodotti, i, ingredienti, -1);
	     profitto += prodotti[i].prezzo;
	     prodotti[i].num++;
	     flag = 0;
	     max = esplora(ingredienti, prodotti, np, profitto, max, i);
	     aggiorna_ingredienti(prodotti, i, ingredienti, +1);
	     profitto -= prodotti[i].prezzo;
	     prodotti[i].num--;
      }
   }

   if (flag==1 && profitto>max) {
      max = profitto;
      for (i=0; i<np; i++) {
        prodotti[i].numBest = prodotti[i].num;
      }
   }

   return max;
}


/********************************************************************
  Funzione di controllo per la realizzabilita' di un prodotto
********************************************************************/
int ingredienti_sufficienti(ricetta *prodotti, int idx, ingrediente *ingredienti)
{
   int i, pos;

   for (i=0; i<prodotti[idx].nn; i++) {
      pos = prodotti[idx].ingredienti[i].indice;
      if (ingredienti[pos].quantita < prodotti[idx].ingredienti[i].quantita)
	     return 0;
   }

   return 1;
}


/********************************************************************
  Funzione di aggiornamento degli ingredienti della dispensa
********************************************************************/
void aggiorna_ingredienti(ricetta *prodotti, int idx, ingrediente *ingredienti, int segno)
{
   int i, pos;

   for (i=0; i<prodotti[idx].nn; i++) {
      pos = prodotti[idx].ingredienti[i].indice;
      ingredienti[pos].quantita += segno*prodotti[idx].ingredienti[i].quantita;
   }
}

