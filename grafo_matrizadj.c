#include "grafo_matrizadj.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool inicializaGrafo(Grafo *grafo, int nv)
{
	int i, j;
	if (nv > MAX_NUM_VERTICES)
	{
		fprintf(stderr, "ERRO: grafo inicializado com nv maior que MAX_NUM_VERTICES\n");
		return false;
	}
	if (nv <= 0)
	{
		fprintf(stderr, "ERRO: grafo inicializado com nv menor ou igual a 0");
		return false;
	}
	grafo->numVertices = nv;
	grafo->numArestas = 0;
	for (i = 0; i < grafo->numVertices; i++)
	{
		for (j = 0; j < grafo->numArestas; j++)
		{
			grafo->mat[i][j] = AN;
		}
	}

	return true;
}

int obtemNrVertices(Grafo *grafo)
{
	return grafo->numVertices;
}

int obtemNrArestas(Grafo *grafo)
{
	return grafo->numArestas;
}

bool verificaValidadeVertice(int v, Grafo *grafo)
{

	if (v > grafo->numVertices)
	{
		fprintf(stderr, "ERRO: vertice invalido\n");
		return false;
	}

	if (v < 0)
	{
		fprintf(stderr, "ERRO: numero do vertice < 0");
		return false;
	}

	return true;
}

void insereAresta(int v1, int v2, Peso peso, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	grafo->mat[v1][v2] = peso;
	grafo->mat[v2][v1] = peso;
	grafo->numArestas++;
}

bool existeAresta(int v1, int v2, Grafo *grafo)
{
	return grafo->mat[v1][v2] != AN;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))

		return VERTICE_INVALIDO;

	return grafo->mat[v1][v2];
}

bool removeArestaObtendoPeso(int v1, int v2, Peso *peso, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	if (existeAresta(v1, v2, grafo))
	{
		*peso = grafo->mat[v1][v2];
		grafo->mat[v1][v2] = AN;
		grafo->mat[v2][v1] = AN;

		grafo->numArestas--;
		return true;
	}

	return false;
}

bool removeAresta(int v1, int v2, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	if (existeAresta(v1, v2, grafo))
	{
		grafo->mat[v1][v2] = AN;
		grafo->mat[v2][v1] = AN;
		grafo->numArestas--;
		return true;
	}

	return false;
}

bool listaAdjVazia(int v, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v, grafo)))
		return true;

	int i;
	for (i = 0; i < grafo->numVertices; i++)
		if (grafo->mat[v][i] != AN)
			return false;
	return true;
}

Apontador primeiroListaAdj(int v, Grafo *grafo)
{
	return proxListaAdj(v, grafo, -1);
}

Apontador proxListaAdj(int v, Grafo *grafo, Apontador atual)
{
	int atual = (int) atual;
	if (!verificaValidadeVertice(v, grafo))
		return VERTICE_INVALIDO;

	Peso lista[] = grafo->mat[v];
	atual++;

	while (lista[atual] != AN && atual < grafo->numVertices)
	{
		atual++;
	}

	if (atual >= grafo->numVertices)
		return VERTICE_INVALIDO;

	return atual;
}

int obtemVerticeDestino(Apontador p, Grafo *grafo)
{
	if (!(verificaValidadeVertice(p, grafo)))
	{
		return VERTICE_INVALIDO;
	}
}

void imprimeGrafo(Grafo *grafo)
{
	for (int i = 0; i < grafo->numVertices; i++)
	{
		for (int j = i+1; j < grafo->numVertices; j++)
		{
			if(grafo->mat[i][j] != AN)
				printf("%d %d %d\n", i, j, grafo->mat[i][j]);
		}
	}
}

void liberaGrafo(Grafo *grafo) {
	free(grafo);
}