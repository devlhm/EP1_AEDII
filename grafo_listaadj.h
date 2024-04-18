#ifndef GRAFO_LISTAADJ_H_
#define GRAFO_LISTAADJ_H_

#include <stdbool.h>
typedef int Peso;

typedef struct str_aresta {
	int vdest;
	Peso peso;
	struct str_aresta *prox;
} Aresta;

#define MAX_NUM_VERTICES 100
#define AN -1
#define VERTICE_INVALIDO -1
typedef struct {
	Aresta** listaAdj;
	int numVertices;
	int numArestas;
} Grafo;

typedef Aresta* Apontador;


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