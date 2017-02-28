#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

#define YES 1
#define NO 0

struct graph
{

    int E;
    int currV;
    int maxV;
    int *enabled;

    EdgeWeight **adj;

    int time;
    int bcnt;
    int *pre;
    int *post;
    int *low;
    Edge *bridges;
    int *cc;
};

EdgeWeight NULLEdgeWeight = NULLEdge;

/**************************
FUNZIONI STATICHE DELLA LIBRERIA
**************************/

static void dfs(Graph G, Edge e)
{
    int v;
    G->pre[e.w] = G->time++;

    for (v = 0; v < G->maxV; v++)
    {
        if (G->enabled[v] == YES &&  !isEdgeNULL(G->adj[e.w][v]))
            if (G->pre[v] == -1)
                dfs(G, EDGE(e.w, v));
    }
    G->post[e.w] = G->time++;

    return;
}

static void bfs(Graph G, Edge e)
{
    int v;
    Q q = QUEUEinit();
    QUEUEput(q, e);
    while (!QUEUEempty(q))
    {
        if (G->pre[(e = QUEUEget(q)).w] == -1)
        {
            G->pre[e.w] = G->time++;
            for (v = 0; v < G->maxV; v++)
                if (G->enabled[v] == YES &&  !isEdgeNULL(G->adj[e.w][v]))
                    if (G->pre[v] == -1)
                    {
                        QUEUEput(q, EDGE(e.w, v));
                    }
        }
    }

    return;
}

static void INITvisit(Graph G)
{
    int v;

    G->pre = malloc (G->maxV * sizeof(int));
    G->post = malloc (G->maxV * sizeof(int));
    G->low = malloc (G->maxV * sizeof(int));
    G->cc = malloc (G->maxV * sizeof(int));
    G->bridges = malloc(G->maxV * sizeof(Edge));

    G->time = 0;
    G->bcnt = 0;

    for (v=0; v < G->maxV; v++)
    {
        G->pre[v] = -1;
        G->post[v] = -1;
        G->low[v] = -1;
        G->cc[v] = -1;

        G->bridges[v].v =-1;
        G->bridges[v].w = -1;
    }

    return;
}

static void CLEANvisit(Graph G)
{
    free(G->pre);
    free(G->post);
    free(G->low);

    //free(G->bridges);
    //free(G->cc);
    return;
}

static void dfsRcc(Graph G, int v, int id)
{
    int w;

    G->cc[v] = id;
    for (w = 0; w < G->maxV; w++)
    {
        if (G->enabled[w] == YES &&  !isEdgeNULL(G->adj[v][w]))
            if (G->cc[w] == -1)
                dfsRcc(G, w, id);
    }

    return;
}

static void bridgeR(Graph G, Edge e )
{
    int v;
    int w = e.w;

    G->pre[w] = G->time++;
    G->low[w] = G->pre[w];

    for (v = 0; v < G->maxV; v++)
    {
        if( !isEdgeNULL(G->adj[w][v])  ){
            if (G->enabled[v] == YES && G->pre[v] == -1)
            {

                bridgeR(G, EDGE(w, v));

                if (G->low[w] > G->low[v])
                    G->low[w] = G->low[v];

                if (G->low[v] == G->pre[v])
                {
                    G->bridges[ G->bcnt ].w = w;
                    G->bridges[ G->bcnt ].v = v;
                    G->bridges[ G->bcnt ].v = v;
                    G->bcnt++;
                }

            }
            else if (v != e.v)
            {
                if(G->low[w] > G->pre[v])
                    G->low[w] = G->pre[v];
            }
        }
    }

    return;
}

EdgeWeight **MATRIXinit(int r, int c, int val)
{
    int i, j;
    EdgeWeight **t = (EdgeWeight **) malloc(r * sizeof(EdgeWeight *));
    if (t == NULL)
    {
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

/**************************
FUNZIONI PUBBLICHE DELLA LIBRERIA
**************************/

//Inizializzazione dell'adt
Graph GRAPHinit(int maxV)
{
    int i;

    Graph G = malloc( sizeof(*G) );
    if (G == NULL)
    {
        printf("Errore di allocazione della memoria\n");
        exit(-1);
    }

    G->currV = 0;
    G->maxV = maxV;
    G->E = 0;
    G->time = 0;

    G->adj = MATRIXinit(maxV, maxV, 0);
    G->pre = NULL;
    G->post = NULL;

    G->enabled = (int*)malloc(maxV*sizeof(int));
    for(i=0; i<maxV; i++)
        G->enabled[i] = NO;

    return G;
}

//Stampa a schermo
void  GRAPHshow(Graph G, TS vertices)
{

    GRAPHprint(G,vertices,stdout);
    return;
}

//Stampa su file
void  GRAPHprint(Graph G, TS vertices, FILE* fp)
{
    int i, j;

    if (fp == NULL) return;

    fprintf(fp,"Il grafo ha %d vertici, %d archi \n", G->currV, G->E);
    for (i=0; i < G->maxV; i++)
    {
        if(G->enabled[i]==YES)
        {
            fprintf(fp,"%s: ", TSleggiIndice(vertices, i));

            for (j=0; j < G->maxV; j++)
            {
                if(G->enabled[j]==YES)
                    fprintf(fp,"%2d", isEdgeNULL(G->adj[i][j]) ? 0 : 1);
            }
            fprintf(fp,"\n");

        }
    }

    return;
}

//Inserimento arco
void GRAPHinsertE(Graph G, int v, int w, EdgeWeight E)
{
    if (v >= G->maxV || w >= G->maxV)
        return;

    if (G->enabled[v] == NO)
    {
        G->enabled[v] = YES;
        G->currV++;
    }

    if(G->enabled[w] == NO)
    {
        G->enabled[w] = YES;
        G->currV++;
    }

    if (isEdgeNULL(G->adj[v][w]))
        G->E++;
    G->adj[v][w] = E;
    G->adj[w][v] = E;

    return;
}

//Rimozione arco
void GRAPHremoveE(Graph G, int v, int w)
{

    if ( !isEdgeNULL(G->adj[v][w]) )
        G->E--;
    G->adj[v][w] = NULLEdgeWeight;
    G->adj[w][v] = NULLEdgeWeight;

    return;
}

//Rimozione vertice
void  GRAPHremoveV(Graph G, int v)
{
    int w;

    if (G->enabled[v] == YES)
        G->currV--;

    G->enabled[v] = NO;
    for (w=0; w<G->maxV; w++)
        if (G->enabled[w] == YES)
            GRAPHremoveE(G,v,w);

    return;
}

//Visita in ampiezza del grafo
void GRAPHbfs(Graph G, int start)
{
    INITvisit(G);
    bfs(G, EDGE(start,start));
    CLEANvisit(G);

    return;
}

//Conteggio dei nodi visitati
int GRAPHcountNode(Graph G, int start)
{
    int cnt = 0;

    if( G->enabled[start] != YES) return -1;

    INITvisit(G);
    bfs(G, EDGE(start,start));
    cnt = G->time;
    CLEANvisit(G);

    return cnt;
}

//Visita in profondità
void GRAPHdfs(Graph G, int start)
{
    int v;
    INITvisit(G);

    if (start != -1)
    {
        dfs(G, EDGE(start, start));
    }
    else
    {
        for (v=0; v < G->maxV; v++)
            if (G->enabled[v] == YES && G->pre[v] == -1)
                dfs(G, EDGE(v,v));
    }

    CLEANvisit(G);
    return;
}

//Controllo della connettività del grafo
int GRAPHconnCheck(Graph G)
{
    int v;
    int cnt = 0;
    INITvisit(G);

    for (v=0; v < G->maxV; v++)
    {
        if (G->enabled[v] == YES && G->pre[v] == -1)
        {
            cnt++;
            dfs(G, EDGE(v,v));
        }
    }

    CLEANvisit(G);

    if (cnt >1)
        return 0;
    else
        return 1;
}

//Calcolo dei bridge del grafo
Edge* GRAPHbridge(Graph G, int *nBridges)
{
    int v;

    INITvisit(G);

    for (v=0; v < G->maxV; v++)
    {
        if (G->enabled[v] == YES && G->pre[v]== -1)
        {
            bridgeR(G, EDGE(v, v));
        }
    }

    *nBridges = G->bcnt;
    CLEANvisit(G);

    return G->bridges;
}

//Pulizia della struttura dati dell'adt
void GRAPHfree(Graph G)
{
    int v;
    for (v=0; v < G->maxV; v++)
    {
        free(G->adj[v]);
    }
    free(G->adj);

    if (G->pre != NULL)
        free(G->pre);
    if (G->post != NULL)
        free(G->post);
    if (G->low != NULL)
        free(G->low);
    if (G->cc != NULL)
        free(G->cc);
    if (G->bridges != NULL)
        free(G->bridges);

    free(G);
}

//Peso dell'arco
EdgeWeight GRAPHew(Graph G, int v, int w)
{
    return G->adj[v][w];
}

//Numero attuale di vertici nel grafo
int GRAPHsize(Graph G)
{
    return G->currV;
}

//Numero massimo di vertici nel grafo
int GRAPHmaxSize(Graph G)
{
    return G->maxV;
}

//Calcolo delle componenti connesse
int* GRAPHcc(Graph G, int *nCc)
{
    int v, id = 0;

    INITvisit(G);

    for (v = 0; v < G->maxV; v++)
    {
        if (G->enabled[v] == YES && G->cc[v] == -1)
        {
            dfsRcc(G, v, id++);
        }
    }

    *nCc = id;

    CLEANvisit(G);
    return G->cc;
}

//Grado di un vertice (numero di archi)
int GRAPHcardV(Graph G, int v)
{
    int w;
    int cnt = 0;

    if (G->enabled[v] != YES) return -1;

    for (w=0; w<G->maxV; w++)
        if (G->enabled[w] == YES && !isEdgeNULL(G->adj[v][w]))
            cnt++;

    return cnt;
}

