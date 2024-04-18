#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef MATRIZ
#include "grafo_matrizadj.h"
#else
#include "grafo_listaadj.h"
#endif

//TODO IMPLEMENTAR BUSCA EM LARGURA
//TODO IMPLEMENTAR IDENTIFICAÇÃO DE COMPONENTES CONECTADOS
//TODO IMPLEMENTAR IDENTIFICAÇÃO DE VERTICES DE ARTICULAÇÃO
typedef enum e_cor
{
	BRANCO,
	CINZA,
	PRETO
} Cor;

typedef struct str_no_pilha {
	int ch;
	struct str_no_pilha* prox;
} NO_PILHA;

typedef struct {
	NO_PILHA* topo;
} PILHA;

PILHA* inicializaPilha() {
	PILHA* pilha = (PILHA*) malloc(sizeof(PILHA));
	pilha->topo = NULL;

	return pilha;
}

void inserePilha(PILHA* pilha, int ch) {
	NO_PILHA* no = (NO_PILHA*) malloc(sizeof(NO_PILHA));
	no->ch = ch;

	if(!pilha->topo) {
		pilha->topo = no;
		no->prox = NULL;
	} else {
		no->prox = pilha->topo;
		pilha->topo = no;
	}
}

NO_PILHA* removePilha(PILHA* pilha) {
	if(!pilha->topo) return NULL;

	NO_PILHA* no = pilha->topo;
	pilha->topo = no->prox;
	no->prox = NULL;

	return no;
}

void visitaBP(int v, Grafo *grafo, int *tempo, Cor *cor, int *tdesc, int *tterm, int *antecessor, FILE* fp_saida)
{
	cor[v] = CINZA;
	tdesc[v] = ++(*tempo);
	printf("%d ", v);

	Apontador aresta = primeiroListaAdj(v, grafo);

	while (aresta != AN && aresta != NULL)
	{
		int u = obtemVerticeDestino(aresta, grafo);
		
		if (cor[u] == BRANCO)
		{
			antecessor[u] = v;
			visitaBP(u, grafo, tempo, cor, tdesc, tterm, antecessor, fp_saida);
		}

		tterm[v] = ++(*tempo);
		cor[v] = PRETO;
		
		aresta = proxListaAdj(v, grafo, aresta);
	}
}

void buscaProfundidade(Grafo *grafo, char* arq_saida)
{
	FILE* fp_saida = fopen(arq_saida, "w");
	puts("BP:");
	int *tdesc = (int *)malloc(obtemNrVertices(grafo) * sizeof(int));
	int *tterm = (int *)malloc(obtemNrVertices(grafo) * sizeof(int));
	int *antecessor = (int *)malloc(obtemNrVertices(grafo) * sizeof(int));
	Cor *cor = (Cor *)malloc(obtemNrVertices(grafo) * sizeof(Cor));

	int tempo = 0;

	for (int v = 0; v < obtemNrVertices(grafo); v++)
	{
		cor[v] = BRANCO;
		tdesc[v] = 0;
		tterm[v] = 0;
		antecessor[v] = VERTICE_INVALIDO;
	}


	for (int v = 0; v < obtemNrVertices(grafo); v++)
		if (cor[v] == BRANCO)
			visitaBP(v, grafo, &tempo, cor, tdesc, tterm, antecessor, fp_saida);
	
	puts("");
	puts("");

	puts("Caminhos BP:");

	PILHA* pilha = inicializaPilha();
	printf("ANT 1: %d\n", antecessor[1]);
	for(int v = 0; v < obtemNrVertices(grafo); v++) {
		int a = antecessor[v];

		while(a != VERTICE_INVALIDO) {
			inserePilha(pilha, a);
			a = antecessor[a];
		}


		NO_PILHA* no = removePilha(pilha);
		while(no != NULL) {
			printf("%d ", no->ch);

			no = removePilha(pilha);
		}

		printf("%d ", v);
		puts("");
	}
}

Grafo *ler_grafo(char *arq_entrada)
{
	Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));

	FILE *fp = fopen(arq_entrada, "r");

	if (fp == NULL)
	{
		printf("ERRO: não foi possível abrir o arquivo %s", arq_entrada);
		return 1;
	}

	int nVert;
	int nArestas;

	fscanf(fp, "%d %d ", &nVert, &nArestas);

	inicializaGrafo(grafo, nVert);

	for (int i = 0; i < nArestas; i++)
	{
		int v1, v2, peso;
		fscanf(fp, "%d %d %d ", &v1, &v2, &peso);
		insereAresta(v1, v2, peso, grafo);
	}

	return grafo;
}

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Missing arguments!");
		return 1;
	}

	char *arq_entrada = argv[1];
	char *arq_saida = argv[2];

	Grafo *grafo = ler_grafo(arq_entrada);
	imprimeGrafo(grafo);
	puts("-----------------------");
	buscaProfundidade(grafo, arq_saida);
}
