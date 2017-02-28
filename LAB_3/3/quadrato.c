#include <stdio.h>
#include <stdlib.h>

int check(int *perm, int n);
int permutazioni(int *perm, int *mark, int n, int pos);

int main(void) {

  int *perm, *mark, n, i, j;

  printf("Input n: ");
  scanf("%d", &n);

  perm = malloc(n*n*sizeof(int));
  mark = malloc(n*n*sizeof(int));

  for (i=0; i<n*n; i++) {
    perm[i] = 0;
    mark[i] = 0;
  }

  if ( permutazioni(perm, mark, n, 0)) {
    printf("Un quadrato magico e':\n");
    for (i=0; i<n; i++) {
      for (j=0; j<n; j++)
        printf("%d  ", perm[i*n+j]);
      printf("\n");
    }
  }
  return 0;


}

int permutazioni(int *perm, int *mark, int n, int pos) {

  int i;
  if (pos >= n*n) {
    if (check(perm, n) ==1)
      return 1;
  return 0;
  }

  for (i=0; i<n*n; i++) {
    if (mark[i] == 0) {
      mark[i] = 1;
      perm[pos] = i+1;
      if (permutazioni(perm, mark, n, pos+1))
        return 1;
      mark[i] = 0;
    }
  }
  return 0;
}

int check(int *perm, int n) {

  int sum, i, j, add;
  sum = n*(n*n+1)/2;

  for (i=0; i<n; i++) {
    add =0;
    for (j=0; j<n; j++)
      add += perm[i*n+j];
    if (add != sum)
      return 0;
  }

  for (j=0; j<n; j++) {
    add =0;
    for (i=0; i<n; i++)
      add += perm[i*n+j];
    if (add != sum)
      return 0;
  }

  add =0;
  for (i=0; i<n; i++)
    add += perm[i*n+i];
  if (add != sum)
    return 0;

  add =0;
  for (i=0; i<n; i++)
    add += perm[i*n+n-1-i];
  if (add != sum)
    return 0;

  return 1;
}
