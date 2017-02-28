/*
 *
 *  Calcolo del determinante di una matrice.
 *  In questa soluzione, il determinante viene calcolato lavorando
 *  sempre solo sulla matrice di partenza, saltando le righe e le
 *  colonne che risultano essere eliminate. Concettualmente, questo
 *  fatto complica un po' la gestione, anche se in realta' il codice
 *  risulta piu' compatto. E' necessario comunque utilizzare un
 *  vettore ausiliario per tenere traccia delle colonne eliminate,
 *  su cui occorre anche eseguire un backtrack uscendo dalla ricorsione.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX  200

int **leggi_matrice(int *dim);
int **malloc2d(int dim);
void free2d(int **m, int dim);
int determinante(int **matrice, int dim, int *eliminate, int i);

int main(void)
{
  int **matrice, *eliminate, delta, dim, i;

  matrice = leggi_matrice(&dim);
  eliminate = (int *)malloc(dim * sizeof(int));
  if (eliminate == NULL) {
    printf("Errore di allocazione della memoria.\n");
    exit(1);
  }
  for (i=0; i<dim; i++) {
    eliminate[i] = 0;
  }
  delta = determinante(matrice, dim, eliminate, 0);
  printf("Valore del determinante: %d\n", delta);
  free2d(matrice, dim);
  free(eliminate);

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

int determinante(int **matrice, int dim, int *eliminate, int i)
{
  int j, delta=0, segno=1, det;

  if (i >= dim) {
    return 1;
  }

  for (j=0; j<dim; j++) {
    if (eliminate[j] != 0) {
      continue;
    }
    eliminate[j] = 1;
    det = determinante(matrice, dim, eliminate, i+1);
    delta += segno*matrice[i][j]*det;
    segno = -segno;
    eliminate[j] = 0;
  }
  return delta;
}
