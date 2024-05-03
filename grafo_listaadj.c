#include "grafo_listaadj.h"

#include <stdio.h>
#include <stdlib.h>

bool inicializaGrafo(Grafo *grafo, int nv)
{
	if (nv <= 0 || grafo == NULL)
	{
		fprintf(stderr, "ERRO: numero de vertices menor ou igual a 0 ou ponteiro nulo na funcao inicializaGrafo");
		return false;
	}

	grafo->numArestas = 0;
	grafo->numVertices = nv;

	if (!(grafo->listaAdj = (Aresta **)calloc(nv, sizeof(Aresta *))))
	{
		fprintf(stderr, "ERRO: erro na alocação da lista de adjacência na função inicializaGrafo");
		return false;
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
		return;

	Aresta *novo = (Aresta *)malloc(sizeof(Aresta));
	novo->peso = peso;
	novo->vdest = v2;
	novo->prox = NULL;

	Aresta *p = grafo->listaAdj[v1];

	if (!p)
		grafo->listaAdj[v1] = novo;
	else
	{
		novo->prox = p;
		grafo->listaAdj[v1] = novo;
	}

	Aresta *novo2 = (Aresta *)malloc(sizeof(Aresta));
	novo2->peso = peso;
	novo2->vdest = v1;
	novo2->prox = NULL;

	Aresta *p2 = grafo->listaAdj[v2];

	if (!p2)
	{
		grafo->listaAdj[v2] = novo2;
	}
	else
	{
		novo2->prox = p2;
		grafo->listaAdj[v2] = novo2;
	}

	grafo->numArestas++;
}

bool existeAresta(int v1, int v2, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	Aresta *p = grafo->listaAdj[v1];
	if (!p)
		return false;

	while (p->prox)
	{
		if (p->vdest == v2)
			return true;
		p = p->prox;
	}

	return false;
}

Peso obtemPesoAresta(int v1, int v2, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return VERTICE_INVALIDO;

	Aresta *p = grafo->listaAdj[v1];
	if (!p)
		return AN;
	while (p->prox)
	{
		if (p->vdest == v2)
			return p->peso;
		p = p->prox;
	}

	return AN;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso *peso, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	if (!existeAresta(v1, v2, grafo))
	{
		fprintf(stderr, "ERRO: aresta não existe na função removeAresta");
		return false;
	}

	Aresta *p = grafo->listaAdj[v1];
	Aresta *ant;
	while (p->prox)
	{
		if (p->vdest == v2)
			break;

		ant = p;
		p = p->prox;
	}

	if (ant)
		ant->prox = p->prox;
	else
		grafo->listaAdj[v1] = NULL;

	*peso = p->peso;
	free(p);

	Aresta *p2 = grafo->listaAdj[v2];
	Aresta *ant2;
	while (p2->prox)
	{
		if (p2->vdest == v1)
			break;

		ant2 = p2;
		p2 = p2->prox;
	}

	if (ant2)
		ant2->prox = p2->prox;
	else
		grafo->listaAdj[v2] = NULL;

	free(p2);

	grafo->numArestas--;

	return true;
}

bool removeAresta(int v1, int v2, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
		return false;

	if (!existeAresta(v1, v2, grafo))
	{
		fprintf(stderr, "ERRO: aresta não existe na função removeAresta");
		return false;
	}

	Aresta *p = grafo->listaAdj[v1];
	Aresta *ant;
	while (p->prox)
	{
		if (p->vdest == v2)
			break;

		ant = p;
		p = p->prox;
	}

	if (ant)
		ant->prox = p->prox;
	else
		grafo->listaAdj[v1] = NULL;

	free(p);

	Aresta *p2 = grafo->listaAdj[v2];
	Aresta *ant2;
	while (p2->prox)
	{
		if (p2->vdest == v1)
			break;

		ant2 = p2;
		p2 = p2->prox;
	}

	if (ant2)
		ant2->prox = p2->prox;
	else
		grafo->listaAdj[v2] = NULL;

	free(p2);

	grafo->numArestas--;
}

bool listaAdjVazia(int v, Grafo *grafo)
{
	if (!(verificaValidadeVertice(v, grafo)))
		return false;
	return grafo->listaAdj[v] == NULL;
}

Apontador primeiroListaAdj(int v, Grafo *grafo)

{
	if (!(verificaValidadeVertice(v, grafo)))
		return VERTICE_INVALIDO;
	Aresta *p = grafo->listaAdj[v];
	if (p) {
		return p;
	}
	else {
		return AN;
	}
}

Apontador proxListaAdj(int v, Grafo *grafo, Apontador atual)
{
	if (!(verificaValidadeVertice(v, grafo)))
		return VERTICE_INVALIDO;

	if (!atual)
	{
		//fprintf(stderr, "ERRO: atual == NULL em proxListaAdj");
		return AN;
	}

	return atual->prox;
}

int obtemVerticeDestino(Apontador p, Grafo *grafo)
{
	if (!p) {
		return AN;
	}
	printf("%d", p->vdest);
	return p->vdest;
}

void imprimeGrafo(Grafo *grafo)
{
	for (int i = 0; i < grafo->numVertices; i++)
	{
		Aresta *p = grafo->listaAdj[i];

		while (p)
		{
			printf("%d %d %d\n", i, p->vdest, p->peso);

			p = p->prox;
		}
	}
}

void liberaGrafo(Grafo *grafo)
{
	for (int i = 0; i < grafo->numVertices; i++)
	{
		Aresta *p = grafo->listaAdj[i];

		while (p)
		{
			Aresta *atual = p;
			p = p->prox;

			free(atual);
		}
	}

	free(grafo);
}