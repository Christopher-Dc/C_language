#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Graph.h"
#include "Queue.h"

struct graph {
    int V, E;
    EdgeWeight **adj;
    int time;
    int *pre;
    int *post;
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
    G->pre = NULL;
    G->post = NULL;

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
    while (!QUEUEempty(q)) {
        if (G->pre[(e = QUEUEget(q)).w] == -1)
        {
            G->pre[e.w] = G->time++;
            if (G->pre[e.w] > 0) printf ("* %s\n", TSleggiIndice(vertices, e.w));
            for (v = 0; v < G->V; v++)
                if (!isEdgeNULL(G->adj[e.w][v]))
                    if (G->pre[v] == -1) {
                        QUEUEput(q, EDGE(e.w, v));
                    }
        }
    }
}

static void INITvisit(Graph G) {
    int v;

    G->pre = malloc (G->V * sizeof(int));
    G->post = malloc (G->V * sizeof(int));

    G->time = 0;
    for (v=0; v < G->V; v++)
    {
        G->pre[v] = -1;
        G->post[v] = -1;
    }
}

static void CLEANvisit(Graph G) {
    free(G->pre);
    free(G->post);
}

void GRAPHbfs(Graph G, TS v, int start)
{
    INITvisit(G);
    bfs(G, v, EDGE(start,start));
    CLEANvisit(G);
}

static void dfs(Graph G, TS vertices, Edge e)
{
    int v;
    G->pre[e.w] = G->time++;
    if (G->pre[e.w] > 0) printf ("* %s\n", TSleggiIndice(vertices, e.w));
    for (v = 0; v < G->V; v++) {
        if (!isEdgeNULL(G->adj[e.w][v]))
            if (G->pre[v] == -1) dfs(G, vertices, EDGE(e.w, v));
    }
    G->post[e.w] = G->time++;
}

void GRAPHdfs(Graph G, TS vertices, int start)
{
    int v;
    INITvisit(G);

    if (start != -1) {
        dfs(G, vertices, EDGE(start, start));
    } else {
        for (v=0; v < G->V; v++)
            if (G->pre[v] == -1)
                dfs(G, vertices, EDGE(v,v));
    }

    CLEANvisit(G);

}

/********************************************************************
  Funzione di verifica di aciclicita' (ricorsiva):
  effettua una visita in profondita', marcando i nodi come scoperti
  (nodo "grigio") e completati (nodo "nero").
  Il ciclo viene rilevato in presenza di un arco backward (link
  tra due nodi grigi).
********************************************************************/
static int dfsLoops(Graph G, Edge e)
{
    int v;

    if (G->post[e.v] != -1) // È un nodo "nero"
        return 0;
    if (G->pre[e.v] != -1) // È un nodo "grigio". Si è individuato un arco back.
        return 1;

    G->pre[e.v] = 1; // Marca come scoperto
    for (v = 0; v < G->V; v++) {
        if (!isEdgeNULL(G->adj[e.v][v]))
            if (dfsLoops(G, EDGE(v, v)))
                return 1;
    }
    G->post[e.w] = 1; // Marca come completato
    return 0;
}

/********************************************************************
  Funzione di verifica di aciclicita':
  Per ogni nodo del grafo, se il nodo e' non visitato (nodo "bianco"), si
  richiama la funzione ricorsiva dfsLoops. Quest'ultima
  effettua una visita in profondita' a partire dal nodo ricevuto,
  rilevando la presenza di archi backward.
********************************************************************/
int GRAPHloops(Graph G) {
    int v;
    INITvisit(G);
    for (v=0; v < G->V; v++) {
        if (G->pre[v] == -1) {
            if (dfsLoops(G, EDGE(v,v))) {
                CLEANvisit(G);
                return 1;
            }
        }
    }
    CLEANvisit(G);
    return 0;
}

void GRAPHfree(Graph G) {
    int v;
    for (v=0; v < G->V; v++) {
        free(G->adj[v]);
    }
    free(G->adj);

    if (G->pre != NULL)
        free(G->pre);
    if (G->post != NULL)
        free(G->post);

    free(G);
}

