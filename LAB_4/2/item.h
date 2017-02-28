#ifndef ITEM_H_DEFINED
#define ITEM_H_DEFINED
#include <stdio.h>

#define key(A) (A.code)
#define name(A) (A.name)
#define eq(A, B) (!strcmp(A,B))
#define neq(A, B) (strcmp(A,B))
#define less(A, B) (strcmp(A,B) < 0)
#define EMPTYitem {NULL, ""}

#define MAX 1000+1
#define KEY 8+1

typedef struct Item_t {
   char *name;
   char code[KEY];
} Item;

Item item_read(FILE *fp);
Item item_read_key(FILE *fp);
void item_print(Item i, FILE *fp);
void item_free(Item i);

#endif
