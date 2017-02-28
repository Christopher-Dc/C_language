#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

typedef struct QUEUEnode *link;

struct QUEUEnode
{
    Edge item;
    link next;
};
struct queue
{
    link head;
    link tail;
    int size;
};

link NEW(Edge e, link next)
{
    link x = malloc(sizeof *x) ;
    x->item = e;
    x->next = next;
    return x;
}

Q QUEUEinit()
{
    Q q = malloc(sizeof *q) ;
    q->head = NULL;
    q->size = 0;
    return q;
}

int QUEUEempty(Q q)
{
    return q->head == NULL;
}

int QUEUEsize(Q q)
{
    link x;
    int n = 0;
    x = q->head;
    while (x != NULL)
    {
        n++;
        x = x->next;
    }
    return n;
}

void QUEUEput (Q q, Edge e)
{
    if (q->head == NULL)
    {
        q->tail = NEW(e, q->head) ;
        q->head = q->tail;
        q->size += 1;
        return;
    }
    q->tail->next = NEW(e, q->tail->next) ;
    q->tail = q->tail->next;
    q->size += 1;

}

Edge QUEUEget(Q q)
{
    Edge e = q->head->item;
    link t = q->head->next;
    free(q->head);
    q->head = t;
    q->size--;
    return e;
}

void QUEUEdisplay(Q q, void (*visit) (Edge))
{
    link x;
    x = q->head;
    while (x != NULL)
    {
        visit(x->item);
        x = x->next;
    }
}

static void LINKfree(link l)
{
    if (l != NULL)
    {
        LINKfree(l->next);
        free(l);
    }
}

void QUEUEfree(Q q)
{
    LINKfree(q->head);
    free(q);
}
