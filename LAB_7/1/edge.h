#ifndef EDGE_H_DEFINED
#define EDGE_H_DEFINED

typedef struct Edge_ {
    int v;
    int w;
} Edge;

typedef struct EdgeWeight_ {
   int speed;       /* velocità massima di comunicazione, kbit/s */
} EdgeWeight;

Edge EDGE(int, int);
void EDGEshow(Edge x);

#define NULLEdge {0}
#define isEdgeNULL(a) (a.speed == 0)

#endif
