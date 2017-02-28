/*
 *
 *  Calcolo del determinante di una matrice.
 *  In questa soluzione, viene materialmente generata una nuova matrice
 *  (la matrice minore) a ogni passo della ricorsione, su cui si
 *  ricorre per calcolare il suo determinante.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX  200

int **leggi_matrice(int *dim);
int **malloc2d(int dim);
void free2d(int **m, int dim);
int determinante(int **matrice, int dim);
void riduci(int **matrice, int dim, int **ridotta, int c);
void stampa2d(int **m, int dim);

int main(void)
{
  int **matrice, delta, dim;

  matrice = leggi_matrice(&dim);
  delta = determinante(matrice, dim);
  printf("Valore del determinante: %d\n", delta);
  free2d(matrice, dim);

  return 0;
}

int **leggi_matrice(int *dim)
{
  int **matrice, i, j, n=0;
  char nomefile[20], line[MAX];
  FILE *fp;

  printf("Nome del file contenente la matrice: ");
  scanf("%s", nomefile);
  fp = fopen(nomefile, "r");
  if (fp == NULL) {
    printf("Errore di apertura del file.\n");
    exit(1);
  }

  /* lettura iniziale per contare le righe della matrice */
  while (fgets(line, MAX, fp) != NULL) {
    n++;
  }
  fclose(fp);

  /* lettura e memorizzazione della matrice */
  fp = fopen(nomefile, "r");
  if (fp == NULL) {
    printf("Errore di apertura del file.\n");
    exit(1);
  }

  matrice = malloc2d(n);
  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
      fscanf(fp, "%d", &matrice[i][j]);
    }
  }
  fclose(fp);

  *dim = n;
  return matrice;
}

int **malloc2d(int dim)
{
  int i, **m;

  m = (int **)malloc(dim * sizeof(int *));
  if (m == NULL) {
    printf("Errore di allocazione della memoria.\n");
    exit(1);
  }

  for (i=0; i<dim; i++) {
    m[i] = (int *)malloc(dim * sizeof(int));
    if (m[i] == NULL) {
      printf("Errore di allocazione della memoria.\n");
      exit(1);
    }
  }

  return m;
}

void free2d(int **m, int dim)
{
  int i;

  for (i=0; i<dim; i++) {
    free(m[i]);
  }
  free(m);
}

int determinante(int **matrice, int dim)
{
  int j, delta=0, segno=1;
  int **ridotta, det;

  if (dim == 1) {
    return matrice[0][0];
  }

  ridotta = malloc2d(dim-1);
  for (j=0; j<dim; j++) {
    riduci(matrice, dim, ridotta, j);
    det = determinante(ridotta, dim-1);
    delta += segno*matrice[0][j]*det;
    segno = -segno;
  }
  free2d(ridotta, dim-1);
  return delta;
}

void riduci(int **matrice, int dim, int **ridotta, int c)
{
  int i, j, k;

  /* sviluppo sulla prima riga, eliminando la colonna c */
  for (i=1; i<dim; i++) {
    k = 0;
    for (j=0; j<dim; j++) {
      if (j != c) {
        ridotta[i-1][k++] = matrice[i][j];
      }
    }
  }
}

void stampa2d(int **m, int dim)
{
  int i, j;

  for (i=0; i<dim; i++) {
    for (j=0; j<dim; j++) {
      printf("%2d ", m[i][j]);
    }
    printf("\n");
  }
}
