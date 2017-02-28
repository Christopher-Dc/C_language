#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

#include "Edge.h"
#include "Ts.h"

typedef struct graph * Graph;

Graph GRAPHinit(int);
void  GRAPHfree(Graph G);
void  GRAPHbfs(Graph G, int start);
void  GRAPHdfs(Graph G, int start);
void  GRAPHinsertE(Graph G, int v, int w, EdgeWeight E);
void  GRAPHremoveE(Graph G, int v, int w);
void  GRAPHremoveV(Graph G, int v);
void  GRAPHprint(Graph G, TS vertices, FILE* fp);
void  GRAPHshow(Graph G, TS vertices);
int  GRAPHsize(Graph G);
int  GRAPHmaxSize(Graph G);
EdgeWeight GRAPHew(Graph G, int v, int w);

int  GRAPHconnCheck(Graph G);
int* GRAPHcc(Graph G, int *nCc);
int GRAPHcountNode(Graph G, int start);
int GRAPHcardV(Graph G, int v);
Edge* GRAPHbridge(Graph G, int* nBridges);

#endif
   


