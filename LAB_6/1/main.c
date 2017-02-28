/*
 *  Acquisizione e visita di un grafo corrispondente ad una rete
 *  di voli aerei memorizzati su un file.
 *
 *  Acquisito il grafo, il programma chiede di inserire il nome
 *  di una citta' e stampa tutti i collegamenti (diretti e
 *  indiretti) dalla citta' inserita. Viene, inoltre, verificata
 *  la presenza di cicli.
 *  La stampa delle citta' raggiungibili viene eseguita durante
 *  la visita, ma potrebbe essere effettuata anche dopo di essa.
 *
 *  Si fa uso di una rappresentazione tramite matrice di adiacenza.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ts.h"
#include "Graph.h"

/********************************************************************
  DEFINIZIONE STRUTTURA DATI E COSTANTI
********************************************************************/

#define SUCCESS         1
#define FAILURE         2
#define MAX_LEN       100

/********************************************************************
  PROTOTIPI
********************************************************************/

Graph graph_read(FILE *fin, TS cities, int nCities);
TS TS_read(FILE *fin, int *nCities);

/********************************************************************
  Funzione di lettura (grafo): carica le informazioni in
  una struttura di tipo grafo.
********************************************************************/
Graph graph_read(FILE *fin, TS cities, int nC)
{
   char src_name[MAX_LEN], dst_name[MAX_LEN];
   int i, nV, cost, src, dst;
   EdgeWeight ew;
   Graph graph;
   float time;

   graph = GRAPHinit(nC);

   while (fscanf(fin, "%d", &nV) != EOF) {
      for (i=0; i<nV; i++) {
         fscanf(fin, "%s %s", src_name, dst_name);
	     fscanf(fin, "%d %f", &cost, &time);
	     ew.cost = cost;
	     ew.time = time;
         src = TScerca(cities, src_name);
         dst = TScerca(cities, dst_name);
         if (src == -1 || dst == -1) {
	           printf("Errore di inserzione nel grafo\n");
	           fclose(fin);
               exit(FAILURE);
         }
         GRAPHInsertE(graph, src, dst, ew);
      }
   }
   return graph;
}

/********************************************************************
  Funzione di lettura del file (lista di città): carica tutte le
  informazioni in una symbol table
********************************************************************/
TS TS_read(FILE *fin, int *nC)
{
   TS st = NULL;
   char name[MAX_LEN];
   int i;

   if (fscanf(fin, "%d", nC) != 1) {
      printf("Errore di lettura del file\n");
      fclose(fin);
      exit(FAILURE);
   }
   st = TScrea(*nC);

   for(i=0;i<*nC;i++) {
      fscanf(fin, "%s", name);
      TSaggiungi(st,name);
   }
   return st;
}

/********************************************************************
  MAIN
********************************************************************/
int main (void)
{
   char fname[MAX_LEN], cname[MAX_LEN];
   Graph graph = NULL;
   TS cities = NULL;
   int idx, nCities = 0;

   printf("Nome del file di ingresso: ");
   scanf("%s", fname);

   FILE *fin;
   fin = fopen(fname, "r");
   if (fin == NULL) {
      printf("Errore di apertura file %s\n", fname);
      exit(FAILURE);
   }

   cities = TS_read(fin, &nCities);
   if (cities == NULL)
      return FAILURE;

   graph = graph_read(fin, cities, nCities);
   if (graph == NULL)
      return FAILURE;

   fclose(fin);

   // GRAPHshow(graph, cities);

   printf("Citta' di partenza: ");
   scanf("%s", cname);
   idx = TScerca(cities, cname);

   if (idx >= 0) {

      printf("Citta' raggiungibili da %s (DFS)\n", cname);
      GRAPHdfs(graph, cities, idx);

      printf("\nCitta' raggiungibili da %s (BFS)\n", cname);
      GRAPHbfs(graph, cities, idx);
   } else {
      printf("Errore: citta' non trovata!\n");
   }

   if (GRAPHloops(graph))
      printf("\nIl grafo contiene almeno un ciclo.\n");
   else
      printf("\nIl grafo NON contiene nessun ciclo.\n");

   GRAPHfree(graph);
   TSlibera(cities);

   system("pause");
   return SUCCESS;
}
