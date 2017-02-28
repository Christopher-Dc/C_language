#ifndef GRAPH_H
#define GRAPH_H

#include "Edge.h"
#include "ts.h"

typedef struct graph *Graph;

Graph GRAPHinit(int);
void  GRAPHbfs(Graph G, TS vertices, int start);
void  GRAPHdfs(Graph G, TS vertices, int start);
void  GRAPHInsertE(Graph G, int v, int w, EdgeWeight E);
void  GRAPHshow(Graph G, TS vertices);
void  GRAPHfree(Graph G);
int  GRAPHloops(Graph G);

#endif
