#include "grafo_matrizadj.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool inicializaGrafo(Grafo* grafo, int nv) {
	int i, j;
	if(nv > MAX_NUM_VERTICES) {
		fprintf(stderr, "ERRO: grafo inicializado com nv maior que MAX_NUM_VERTICES\n");
		return false;
	}
	if(nv <= 0) {
		fprintf(stderr, "ERRO: grafo inicializado com nv menor ou igual a 0");
		return false;
	}
	grafo->numVertices = nv;
	grafo->numArestas = 0;
	for(i = 0; i < grafo->numVertices; i++) {
		for(j = 0; j < grafo->numArestas; j++) {
			grafo->mat[i][j] = AN;
		}
	}

	return true;
}

int obtemNrVertices(Grafo* grafo);

int obtemNrArestas(Grafo* grafo);

bool verificaValidadeVertice(int v, Grafo *grafo) {
	if(v > grafo->numArestas) {
		fprintf(stderr, "ERRO: vertice invalido\n");
		return false;
	}

	if(v < 0) {
		fprintf(stderr, "ERRO: numero do vertice < 0");
		return false;
	}

	return true;
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo) {
	if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return;
	
	grafo->mat[v1][v2] = peso;
	grafo->numArestas++;
}

bool existeAresta(int v1, int v2, Grafo *grafo) {
	return grafo->mat[v1][v2] != AN;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo) {
	return grafo->mat[v1][v2];
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo *grafo) {
	if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return;

	if(existeAresta(v1,v2,grafo)) {
		*peso = grafo->mat[v1][v2];
		grafo->mat[v1][v2] = AN;
		grafo->numArestas--;
		return true;
	}

	return false;
}

bool removeAresta(int v1, int v2, Grafo *grafo) {
	if(!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return;

	if(existeAresta(v1,v2,grafo)) {
		grafo->mat[v1][v2] = AN;
		grafo->numArestas--;
		return true;
	}

	return false;
}

bool listaAdjVazia(int v, Grafo* grafo) {
	if(!(verificaValidadeVertice(v, grafo)))
		return true;

	int i;
	for(i = 0; i < grafo->numVertices; i++)
		if(grafo->mat[v][i] != AN) return false;
	return true;
}

Apontador primeiroListaAdj(int v, Grafo* grafo);

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);

int obtemVerticeDestino(Apontador p, Grafo* grafo);

void imprimeGrafo(Grafo* grafo);

void liberaGrafo(Grafo* grafo); 