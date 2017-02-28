/*
 *  Tour del Cavaliere
 *
 *  Trova una possibile soluzione al problema di far percorrere a
 *  un cavallo tutte le caselle di una scacchiera una sola volta.
 *  Per ogni passo della ricorsione occorre "provare" tutte le
 *  caselle raggiungibili in un passo dalla posizione corrente.
 *
 */


#include <stdio.h>

#define DIM 5
#define KO 0
#define OK 1

/********************************************************************
  Prototipi
********************************************************************/
int muovi(int mossa, int posx, int posy, int offsetX[], int offsetY[], int scacc[DIM][DIM]);
void stampa_scacchiera(int scacc[DIM][DIM]);

/********************************************************************
  Main
********************************************************************/
int main(void)
{
   int offsetX[8], offsetY[8], scacc[DIM][DIM];
   int i, j, x, y;

   offsetX[0] =  2; offsetY[0] =  1;
   offsetX[1] =  1; offsetY[1] =  2;
   offsetX[2] = -1; offsetY[2] =  2;
   offsetX[3] = -2; offsetY[3] =  1;
   offsetX[4] = -2; offsetY[4] = -1;
   offsetX[5] = -1; offsetY[5] = -2;
   offsetX[6] =  1; offsetY[6] = -2;
   offsetX[7] =  2; offsetY[7] = -1;

   for (i=0; i<DIM; i++) {
      for (j=0; j<DIM; j++) {
	     scacc[i][j] = 0;
      }
   }
   printf("Posizione di partenza: ");
   scanf("%d %d", &x, &y);
   scacc[x][y] = 1;
   if (muovi(2, x, y, offsetX, offsetY, scacc) == OK) {
      stampa_scacchiera(scacc);
   } else {
      printf("Soluzione non trovata\n");
   }
   return 0;
}

/********************************************************************
  Funzione di percorrimento della scacchiera (ricorsiva)
********************************************************************/
int
muovi (
  int mossa,
  int posx,
  int posy,
  int offsetX[],
  int offsetY[],
  int scacc[DIM][DIM]
)
{
   int i, newposx, newposy;

   if (mossa == (DIM*DIM+1)) {
      return OK;
   }

   for (i=0; i<8; i++) {
      newposx = posx + offsetX[i];
      newposy = posy + offsetY[i];
      if ((newposx<DIM) && (newposx>=0) && (newposy<DIM) && (newposy>=0)) {
	     if (scacc[newposx][newposy] == 0) {
	        scacc[newposx][newposy] = mossa;
	        if (muovi(mossa+1, newposx, newposy, offsetX, offsetY, scacc) == OK) {
	           return OK;
	        }
            scacc[newposx][newposy] = 0;
	     }
      }
   }

   return KO;
}

/********************************************************************
  Funzione di stampa della scacchiera
********************************************************************/

void stampa_scacchiera (int scacc[DIM][DIM])
{
   int i, j;
   FILE *fp = stdout;

   for (i=0; i<DIM; i++) {
      for (j=0; j<DIM; j++) {
         fprintf(fp, "%2d ", scacc[i][j]);
      }
      fprintf(fp, "\n");
   }
}
