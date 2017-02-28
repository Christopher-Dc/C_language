#include <stdio.h>
#include <stdlib.h>
#include "Edge.h"

void EDGEshow(Edge e)
{
    printf("\n node = %d  node = %d \n", e.v, e.w);
}

Edge EDGE(int v, int w)
{
    Edge e;
    e.v = v;
    e.w = w;
    return e;
}

