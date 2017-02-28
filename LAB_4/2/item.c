#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"

Item item_read(FILE *fp)
{
   Item d = EMPTYitem;
   char name[MAX], code[KEY];

   if (fp == stdin)
      printf("Inserire nome e codice: ");
   if (fscanf(fp, "%s %s", name, code) != 2) {
      return d;
   }
   d.name = strdup(name);
   strcpy(d.code, code);
   return d;
}

Item item_read_key(FILE *fp)
{
   Item d = EMPTYitem;
   char code[KEY];

   if (fp == stdin)
      printf("Inserire codice: ");
   if (fscanf(fp, "%s", code) != 1) {
      return d;
   }

   strcpy(d.code, code);
   return d;
}

void item_print(Item d, FILE *fp)
{
   fprintf(fp, "%s %s\n", d.name, d.code);
}

void item_free(Item d)
{
   free(d.name);
}

