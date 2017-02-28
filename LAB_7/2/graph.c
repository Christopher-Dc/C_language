#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>

#include "Graph.h"
#include "Queue.h"

struct graph {
    int V, E;
    EdgeWeight **adj;
    int time;
    int *visited;
    float *weight;
    int *father;
};

EdgeWeight NULLEdgeWeight = NULLEdge;

EdgeWeight **MATRIXinit(int r, int c, int val)
{
    int i, j;
    EdgeWeight **t = (EdgeWeight **) malloc(r * sizeof(EdgeWeight *));
    if (t == NULL) {
        printf("Errore di allocazione della memoria\n");
        exit(-1);
    }
    for (i=0; i < r; i++)
        t[i] = (EdgeWeight *) malloc(c * sizeof(EdgeWeight ));
    for (i=0; i < r; i++)
        for (j=0; j < c; j++)
            t[i][j] = NULLEdgeWeight;
    return t;
}

Graph GRAPHinit(int V)
{
    Graph G = malloc(sizeof(*G));
    if (G == NULL) {
      printf("Errore di allocazione della memoria\n");
      exit(-1);
    }

    G->V = V;
    G->E = 0;
    G->time = 0;

    G->adj = MATRIXinit(V, V, 0);
    G->weight = NULL;
    G->father = NULL;
    G->visited = NULL;

    return G;
}

void  GRAPHshow(Graph G, TS vertices)
{
    int i, j;
    printf("Graph has %d vertices, %d edges \n", G->V, G->E);
    for (i=0; i < G->V; i++)
    {
        printf("%s: ", TSleggiIndice(vertices, i));
        for (j=0; j < G->V; j++)
            printf("%2d", isEdgeNULL(G->adj[i][j]) ? 0 : 1);
        printf("\n");
    }
}

void GRAPHInsertE(Graph G, int v, int w, EdgeWeight E) {
    if (v >= G->V || w >= G->V)
        return;
    if (isEdgeNULL(G->adj[v][w]))
        G->E++;
    G->adj[v][w] = E;
}

static void bfs(Graph G, TS vertices, Edge e)
{
    int v;
    Q q = QUEUEinit();
    QUEUEput(q, e);

    G->father[e.v] = e.v;
    G->visited[e.v] = 1;
    G->weight[e.v] = 0;

    while (!QUEUEempty(q)) {
        e = QUEUEget(q);
        for (v = 0; v < G->V; v++) {
            if (!isEdgeNULL(G->adj[e.w][v]))
                if (G->visited[v] == 0) {
                    G->visited[v] = 1;
                    G->father[v] = e.w;
                    G->weight[v] = G->weight[e.w] +1;
                    QUEUEput(q, EDGE(e.w, v));
                }
        }
    }
}

static void INITvisit(Graph G) {
    int v;

    G->father = malloc (G->V * sizeof(int));
    G->visited = malloc (G->V * sizeof(int));
    G->weight = malloc (G->V * sizeof(float));

    G->time = 0;
    for (v=0; v < G->V; v++)
    {
        G->father[v] = -1;
        G->visited[v] = 0;
        G->weight[v] = FLT_MAX;
    }
}

static void CLEANvisit(Graph G) {
    free(G->father);
    G->father = NULL;
    free(G->visited);
    G->visited = NULL;
    free(G->weight);
    G->weight = NULL;
}

void GRAPHbfs(Graph G, TS v, int start)
{
    INITvisit(G);
    bfs(G, v, EDGE(start,start));
}

void GRAPHfree(Graph G) {
    int v;
    for (v=0; v < G->V; v++) {
        free(G->adj[v]);
    }
    free(G->adj);

    if (G->visited != NULL)
        free(G->visited);
    if (G->weight != NULL)
        free(G->weight);
    if (G->father != NULL)
        free(G->father);

    free(G);
}

/********************************************************************
  Funzione di calcolo dei cammini minimi (algoritmo di Bellman-Ford).
  Il parametro "which" viene utilizzato per indicare il campo del
  volo (tempo o prezzo) da guardare.
********************************************************************/
void GRAPHShortestPathBF(Graph G, int start, int which)
{
   int i, j, k, again, nNodes=G->V;
   float newdist, currdist;

   INITvisit(G);

   G->weight[start] = 0;
   G->father[start] = start;

   again = 1;
   for (k=0; k<nNodes && again; k++) {
      again = 0;
      for (i=0; i<nNodes; i++) {
         currdist = G->weight[i];
         if (currdist != FLT_MAX) {
            for (j=0; j<nNodes; j++) {
               if (!isEdgeNULL(G->adj[i][j])) { /* se esiste l'arco */
                  /* calcolo della nuova distanza ... */
                  newdist = currdist + (which ? G->adj[i][j].cost : G->adj[i][j].time);
                  /* ... e relax sul nodo di arrivo */
                  if (newdist < G->weight[j]) {
                     G->weight[j] = newdist;
                     G->father[j] = i;
                     again = 1;
                  }
               }
            }
         }
      }
   }
}

/********************************************************************
  Funzione (ricorsiva) di stampa del cammino minimo tra due citta'.
  La funzione risale albero dei cammini fino alla radice, e stampa
  le varie tratte del cammino ridiscendendo verso il nodo scelto
  (ovvero, uscendo dalla ricorsione).
********************************************************************/
static int printPathR(Graph G, TS vertices, int current, int *cost, float *time)
{
   int stop, father;

   father = G->father[current];
   if (current == father){
      return 0;
   }

   stop = printPathR(G, vertices, father, cost, time);
   printf("Da %s a %s ", TSleggiIndice(vertices, father), TSleggiIndice(vertices, current));
   printf("(prezzo: %d, ", G->adj[father][current].cost);
   printf("durata: %.1f)\n", G->adj[father][current].time);

   *cost += G->adj[father][current].cost;
   *time += G->adj[father][current].time;
   return stop+1;
}

/********************************************************************
  Funzione di stampa del cammino minimo tra due citta'. Si tratta
  di un semplice "wrapper" per una funzione ricorsiva che fa il
  lavoro vero e proprio.
********************************************************************/
void  GRAPHPrintPath(Graph G, TS vertices, int destination)
{
   int cost=0, stop=0;
   float time=0;

   if (G->weight[destination] == FLT_MAX){
      printf("Le due citta' inserite non sono collegate!\n");
      return;
   }

   /* stampa il cammino minimo */
   stop = printPathR(G, vertices, destination, &cost, &time) - 1;

   /* stampa le statistiche totali */
   printf("*** TOTALE\n");
   printf("Prezzo: %d\n", cost);
   printf("Durata: %.1f\n", time);
   printf("Cambi: %d\n\n", stop);

   CLEANvisit(G);
}
