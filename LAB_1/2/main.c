/*
 *
 *  Sviluppo di un sistema del totocalcio in schedine singole.
 *
 *  Il valore del numero N viene viene determinato attraverso
 *  una lettura preventiva del file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sviluppo(char **sistema, char *schedina, int n, int riga, FILE *fp);
char **leggi_file(int *dim);

int main(void)
{
  char *schedina, **sistema;
  char nomefile[20];
  FILE *fp;
  int n, i;

  sistema = leggi_file(&n);
  schedina = (char *)malloc((n+1) * sizeof(char));
  if (schedina == NULL) {
    printf("Errore di allocazione della memoria.\n");
    exit(1);
  }
  schedina[n] = '\0';

  printf("Nome del file per lo sviluppo: ");
  scanf("%s", nomefile);
  fp = fopen(nomefile, "w");
  if (fp == NULL) {
    printf("Errore di apertura del file.\n");
    exit(1);
  }

  sviluppo(sistema, schedina, n, 0, fp);

  fclose(fp);
  for (i=0; i<n; i++) {
    free(sistema[i]);
  }
  free(sistema);
  free(schedina);

  return 0;
}

char **leggi_file(int *dim)
{
  int i, n=0;
  char nomefile[20], line[100];
  char **sistema;
  FILE *fp;

  printf("Nome del file con il sistema: ");
  scanf("%s", nomefile);
  fp = fopen(nomefile, "r");
  if (fp == NULL) {
    printf("Errore di apertura del file.\n");
    exit(1);
  }

  /* lettura iniziale per contare le righe del sistema */
  while (fscanf(fp, "%s", line) != EOF) {
    n++;
  }
  fclose(fp);

  /* lettura e memorizzazione del sistema */
  fp = fopen(nomefile, "r");
  if (fp == NULL) {
    printf("Errore di apertura del file.\n");
    exit(1);
  }

  sistema = (char **)malloc(n * sizeof(char *));
  if (sistema == NULL) {
    printf("Errore di allocazione della memoria.\n");
    exit(1);
  }

  for (i=0; i<n; i++) {
    fscanf(fp, "%s", line);
    sistema[i] = strdup(line);
  }
  fclose(fp);

  *dim = n;
  return sistema;
}

void sviluppo(char **sistema, char *schedina, int n, int riga, FILE *fp)
{
  int i;

  if (riga >= n) {
    fprintf(fp, "%s\n", schedina);
    return;
  }

  for (i=0; i<strlen(sistema[riga]); i++) {
    schedina[riga] = sistema[riga][i];
    sviluppo(sistema, schedina, n, riga+1, fp);
  }
}



