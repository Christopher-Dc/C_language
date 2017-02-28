#ifndef EDGE_H_DEFINED
#define EDGE_H_DEFINED

typedef struct Edge_ {
    int v;
    int w;
} Edge;

typedef struct EdgeWeight_ {
   int cost;       /* costo del viaggio */
   float time;     /* durata del viaggio */
} EdgeWeight;

Edge EDGE(int, int);
void EDGEshow(Edge x);

#define NULLEdge {0, 0.0}
#define isEdgeNULL(a) (a.cost == 0 && a.time == 0.0)

#endif
