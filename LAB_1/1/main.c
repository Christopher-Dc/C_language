/*
 *
 *  Generazione di numeri binari mediante una funzione ricorsiva
 *
 */

#include <stdio.h>
#include <stdlib.h>

void binary(int i, int *vet, int n);

int main(void)
{
  int n, *vet;

  printf("Numero di bit: ");
  scanf("%d", &n);

  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Errore di allocazione.\n");
    exit(1);
  }
  printf("Numeri Binari\n");
  binary(0, vet, n);
  free(vet);

  return 0;
}

void binary(int i, int *vet, int n)
{
  int j;

  if (i >= n) {
    for (j=0; j<n; j++) {
      printf("%d", vet[j]);
    }
    printf("\n");
    return;
  }

  vet[i] = 0;
  binary(i+1, vet, n);

  vet[i] = 1;
  binary(i+1, vet, n);
}



