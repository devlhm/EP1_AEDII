#ifndef GRAFO_MATRIZADJ_H_
#define GRAFO_MATRIZADJ_H_

#include <stdbool.h>

typedef int Peso;
typedef int Apontador;

#define MAX_NUM_VERTICES 100
#define AN -1
#define VERTICE_INVALIDO -1

typedef struct {
	Peso mat[MAX_NUM_VERTICES][MAX_NUM_VERTICES];
	int numVertices;
	int numArestas;
} Grafo;


bool inicializaGrafo(Grafo* grafo, int nv);

int obtemNrVertices(Grafo* grafo);

int obtemNrArestas(Grafo* grafo);

bool verificaValidadeVertice(int v, Grafo *grafo);

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo);

bool existeAresta(int v1, int v2, Grafo *grafo);

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo);

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo);

bool removeAresta(int v1, int v2, Grafo *grafo);

bool listaAdjVazia(int v, Grafo* grafo);

Apontador primeiroListaAdj(int v, Grafo* grafo);

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);

int obtemVerticeDestino(Apontador p, Grafo* grafo);

void imprimeGrafo(Grafo* grafo);

void liberaGrafo(Grafo* grafo); 

#endif
