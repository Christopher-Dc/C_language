#ifndef _QUEUE_INCLUDED
#define _QUEUE_INCLUDED

#include "Edge.h"

typedef struct queue *Q;

Q       QUEUEinit();
int     QUEUEempty(Q q);
void    QUEUEput (Q q, Edge e);
Edge    QUEUEget(Q q);
void    QUEUEdisplay(Q q, void (*visit) (Edge));
int     QUEUEsize(Q q);
void    QUEUEfree(Q q);

#endif
