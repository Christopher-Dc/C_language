/*
 *  Acquisizione e visita di un grafo corrispondente ad una rete
 *  di calcolatori memorizzati su un file.
 *
 *  Acquisito il grafo, il programma offre all'utente un insieme di
 *  possibili operazioni, fra cui: verifica della connettività, con-
 *  nectivity enforcement, rimozione di un elaboratore (e di tutte le
 *  connessioni che lo coinvolgono), inserzione/rimozione di connessioni,
 *  identificazione delle connessioni critiche, visualizzazione e/o
 *  salvataggio su file della rete.
 *
 *  Si fa uso di una rappresentazione tramite matrice di adiacenza.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Graph.h"
#include "Edge.h"
#include "Ts.h"

/********************************************************************
  DEFINIZIONE STRUTTURA DATI E COSTANTI
********************************************************************/

/*TODO
#define MAX_NODES 1024
#define INPUT_FILE "network.txt"
*/

#define SUCCESS         1
#define FAILURE         2

#define MAX_LEN       64
#define MAX_NAME    11 //10 + 1

/********************************************************************
  PROTOTIPI
********************************************************************/

Graph graph_read(FILE *fin, TS computers, int nC);
void graph_critic_conn(Graph G, TS computers);
void graph_conn_enforce(Graph G, TS computers);

/********************************************************************
  MAIN
********************************************************************/
int main (int argc, char** argv)
{
    char cname[MAX_NAME];
    char input_line[MAX_LEN];
    int v,w;
    char* fname;
    Graph graph = NULL;
    EdgeWeight ew;
    TS computers = NULL;
    int nComputers = 0;
    int end = 0;

    FILE *fin = NULL;
    FILE *fout = NULL;

    printf("Laboratorio 12 - Esercizio 1\n\n");

    if (argc != 3){
      printf("Errore sul numero di argomenti passati al programma\n");
      printf("Usage:\n%s input_file n_nodi_max\n",argv[0]);
      exit(FAILURE);
    }

    fname = strdup (argv[1]);
    nComputers = atoi(argv[2]);

    /*/
    fname = strdup (INPUT_FILE);
    nComputers = MAX_NODES;
    */

    fin = fopen(fname, "r");
    if (fin == NULL)
    {
        printf("Errore di apertura file %s\n", fname);
        exit(FAILURE);
    }

    computers = TScrea(nComputers);
    if (computers == NULL)
        return FAILURE;

    graph = graph_read(fin, computers, nComputers);
    if (graph == NULL)
        return FAILURE;

    fclose(fin);

    while (!end)
    {
        printf("Operazioni disponibili:\n");
        printf("0 -> Uscita\n");
        printf("1 -> Verifica di connettivita'\n");
        printf("2 -> Connectivity enforcement\n");
        printf("3 -> Rimozione di un computer\n");
        printf("4 -> Inserzione di una connessione\n");
        printf("5 -> Rimozione di una connessione\n");
        printf("6 -> Identificazione connessioni critiche\n");
        printf("7 -> Visualizzazione della rete\n");
        printf("8 -> Stampa su file della rete\n");
        printf("Inserire numero operazione: ");

        scanf("%s", input_line);

        switch (input_line[0])
        {

        case '1':
            if( GRAPHconnCheck(graph) == 1)  //verifica connettivita
            {
                printf("\nVerifica della connettivita' superata\n\n");
            }
            else
            {
                printf("\nVerificata della connettivita' FALLITA\n\n");
            }
            break;

        case '2':
            if( GRAPHconnCheck(graph) == 1 )  //conn. enforcement
            {
                printf("\nErrore, il grafo e' gia' connesso\n\n");
                break;
            }
            graph_conn_enforce(graph, computers);
            break;

        case '3':
            printf("\nNome del computer da rimuovere: "); //rimozione computer
            scanf("%s",cname);
            v = TScerca(computers,cname);
            if (v < 0)
            {
                printf("Nodo non trovato\n\n");
                break;
            }
            GRAPHremoveV(graph,v);
            break;

        case '4':
            printf("\nNome del primo estremo della connessione da inserire: "); //inserzione connessione
            scanf("%s",cname);
            v = TScercaOAggiungi(computers,cname);
            if (v < 0)
            {
                printf("Impossibile inserire il nuovo nodo\n\n");
                break;
            }
            printf("Nome del seconde estremo della connessione da inserire: ");
            scanf("%s",cname);
            w = TScercaOAggiungi(computers,cname);
            if (w < 0)
            {
                printf("Impossibile inserire il nuovo nodo\n\n");
                break;
            }
            printf("Velocita' della connessione: ");
            scanf("%d", &ew.speed);
            if (ew.speed < 0)
            {
                printf("Errore, velocita' minore di 0\n\n");
                break;
            }
            GRAPHinsertE(graph,v,w,ew);
            break;

        case '5':
            printf("\nNome del primo estremo della connessione da rimuovere: "); //rimozione connessione
            scanf("%s",cname);
            v = TScerca(computers,cname);
            if (v < 0)
            {
                printf("Nodo non trovato\n\n");
                break;
            }
            printf("Nome del secondo estremo della connessione da rimuovere: ");
            scanf("%s",cname);
            w = TScerca(computers,cname);
            if (w < 0)
            {
                printf("Nodo non trovato\n\n");
                break;
            }
            GRAPHremoveE(graph,v,w);
            break;

        case '6':
            if( GRAPHconnCheck(graph) != 1 )  		//connessioni critiche
            {
                printf("\nErrore, il grafo non e' connesso\n\n");
                break;
            }
            graph_critic_conn(graph,computers);
            break;

        case '7':
            GRAPHshow(graph, computers);	//visualizzazione rete
            break;

        case '8':
            printf("\nNome del file in output: "); //stampa rete su file
            scanf("%s",cname);
            fout=fopen(cname,"w");
            if (fout == NULL)
            {
                printf("Errore di apertura file %s\n\n", fname);
                exit(FAILURE);
            }
            GRAPHprint(graph, computers, fout);
            fclose(fout);
            break;

        case '0':
            printf("\nProgramma terminato\n");
            end = 1;
            break;

        default:
            printf("Operazione sconosciuta\n\n");
        }
    }

    GRAPHfree(graph);
    TSlibera(computers);

    return SUCCESS;
}


/********************************************************************
  Lettura del file di input ed allocazione del grafo
********************************************************************/
Graph graph_read(FILE *fin, TS computers, int nC)
{
    char input_line[MAX_LEN];
    char src_name[MAX_NAME], dst_name[MAX_NAME];
    int i, speed, src, dst;
    EdgeWeight ew;
    Graph graph;

    graph = GRAPHinit(nC);

    while ( fgets(input_line, MAX_LEN, fin) != NULL)
    {
        for (i=0; i<nC; i++)
        {
            sscanf(input_line, "%s %s %d", src_name, dst_name, &speed);
            ew.speed = speed;
            src = TScercaOAggiungi(computers, src_name);
            dst = TScercaOAggiungi(computers, dst_name);
            if (src == -1 || dst == -1)
            {
                printf("Errore di inserzione nel grafo\n");
                fclose(fin);
                exit(FAILURE);
            }
            GRAPHinsertE(graph, src, dst, ew);
        }
    }
    return graph;
}

/********************************************************************
  Gestione delle richieste relative alle connessioni critiche
********************************************************************/
void graph_critic_conn(Graph G, TS computers)
{
    EdgeWeight ew;
    Edge* bridges;
    int nBridges = 0;
    int *cc = NULL;
    int nCc, c0,c1, i, j,  vNew, wNew, flag;

    //calcolo dei bridge della rete
    bridges = GRAPHbridge(G,&nBridges);
    if (nBridges <= 0)
    {
        printf("Il grafo non ha connessioni critiche\n\n");
        return;
    }

    printf("Il grafo ha %d connessioni critiche\n",nBridges);
    for (i=0; i<nBridges; i++)
    {
        printf("%s - %s\n",TSleggiIndice(computers,bridges[i].v),TSleggiIndice(computers,bridges[i].w));
    }

    for (i=0; i<nBridges; i++)
    {
        //rimozione del bridge, così che il grafo sia disconnesso e ci siano due componenti connesse
        ew = GRAPHew(G, bridges[i].v, bridges[i].w);
        GRAPHremoveE(G, bridges[i].v, bridges[i].w);

        //calcolo delle componenti connesse
        nCc = 0;
        cc = GRAPHcc(G, &nCc);

        if (nCc != 2)
        {
            printf("Errore nel calcolo delle componenti connesse\n\n");
            exit(FAILURE);
        }

        //conteggio del numero di nodi di ogni cc
        c0 = c1 = 0;
        for(j=0 ; j<GRAPHmaxSize(G) && (cc[j]==-1 || cc[j]!= 0); j++);
        c0 = GRAPHcountNode(G,j);

        for(j=0 ; j<GRAPHmaxSize(G) && (cc[j]==-1 || cc[j]!= 1); j++);
        c1 = GRAPHcountNode(G,j);

        if(c0 > 1 && c1 > 1)
        {
            //ricerca di un nodo qualsiasi della prima cc, diverso dagli estremi del bridge
            for(j=0,flag=0; j<GRAPHmaxSize(G)  && flag == 0 ; j++)
                if (cc[j] == 0 && j != bridges[i].v && j != bridges[i].w)
                {
                    vNew = j;
                    flag = 1;
                }

            //ricerca di un nodo qualsiasi della seconda cc, diverso dagli estremi del bridge
            for(j=0,flag=0; j<GRAPHmaxSize(G)  && flag == 0 ; j++)
                if (cc[j] == 1 && j != bridges[i].v && j != bridges[i].w)
                {
                    wNew = j;
                    flag = 1;
                }

            printf("Inserimento arco ridondato fra i nodi: %s - %s\n",TSleggiIndice(computers,vNew),TSleggiIndice(computers,wNew));
            GRAPHinsertE(G, vNew, wNew, ew);
        }

        //reinserimento del bridge iniziale
        GRAPHinsertE(G, bridges[i].v, bridges[i].w, ew);
        free(cc);
    }

    free(bridges);
    return;
}

/********************************************************************
  Gestione delle funzionalità relative al connection enforcement
********************************************************************/
void graph_conn_enforce(Graph G, TS computers)
{
    int *cc = NULL;
    int nCc = 0;
    int *nNodesCc = NULL;
    int *startCc = NULL;
    int *cardStart = NULL;
    EdgeWeight ew;
    int i, j, min, sw, sw2, cnt;

    //calcolo delle componenti connesse del grafo
    cc = GRAPHcc(G, &nCc);
    if (nCc <= 0)
    {
        printf("Nessuna componente connessa trovata\n");
        return;
    }

    printf("Nella rete sono presenti %d componenti connesse\n",nCc);

    nNodesCc = malloc (nCc * sizeof(int));
    startCc = malloc (nCc * sizeof(int));
    cardStart = malloc (nCc * sizeof(int));

    for (i=0; i<nCc; i++)
    {
        nNodesCc[i] = -1;
        startCc[i] = -1;
        cardStart[i] = -1;
    }

    //ricerca del nodo di grado massimo per ogni cc
    for(i=0; i < GRAPHmaxSize(G); i++)
    {
        if( cc[i] > -1 )
        {
            cnt = GRAPHcardV(G,i);
            if( cardStart[ cc[i] ] < cnt )
            {
                cardStart[cc[i]] = cnt;
                startCc[cc[i]] = i;
            }
        }
    }

    //conteggio dei nodi di ogni cc
    for (i=0; i<nCc; i++)
        nNodesCc[i] = GRAPHcountNode(G, startCc[i] );

    //selection sort crescente sul numero di nodi di ogni cc
    for (i=0; i<nCc-1; i++)
    {
        min = i;
        for (j=i+1; j<nCc; j++)
        {
            if (nNodesCc[j] < nNodesCc[min])
            {
                min = j;
            }
        }
        //swap di entrambi gli array (numero nodi cc e nodo di grado massimo di ogni cc)
        sw = nNodesCc[i];
        sw2 = startCc[i];
        nNodesCc[i] = nNodesCc [min];
        startCc[i] =startCc[min];
        nNodesCc[min] = sw;
        startCc[min] = sw2;
    }

    for(i=nCc-1; i>0; i--)
    {
        printf("Creazione arco fra i nodi %s e %s\nVelocita' connessione: ",
               TSleggiIndice(computers, startCc[i] ), TSleggiIndice(computers, startCc[i-1] ));
        scanf("%d",&ew.speed);
        GRAPHinsertE(G,startCc[i],startCc[i-1],ew);
    }

    free(nNodesCc);
    free(startCc);
    free(cardStart);

    return;
}
