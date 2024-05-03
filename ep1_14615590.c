#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#ifdef MATRIZ
#include "grafo_matrizadj.h"
#else
#include "grafo_listaadj.h"
#endif

// TODO CORRIGIR BUG LISTA DE ADJ VERT DE ARTICULACAO
// TODO COLOCAR A SAIDA NO ARQ
typedef enum e_cor
{
	BRANCO,
	CINZA,
	PRETO
} Cor;

typedef struct str_NO
{
	int ch;
	struct str_NO *prox;
} NO;

typedef struct
{
	NO *topo;
} PILHA;

typedef struct {
	NO* ini;
	NO* fim;
} FILA;

PILHA *inicializaPilha()
{
	PILHA *pilha = (PILHA *)malloc(sizeof(PILHA));
	pilha->topo = NULL;

	return pilha;
}

FILA *inicializaFila() {
	FILA* fila = (FILA*) malloc(sizeof(PILHA));
	fila->ini = NULL;
	fila->fim = NULL;

	return  fila;
}

void insereFila(FILA* fila, int ch) {
	NO* no = (NO*) malloc(sizeof(NO));
	no->ch = ch;
	no->prox = NULL;
	if(!fila->ini) {
		fila->ini = no;
		fila->fim = no;
	} else {
		fila->fim->prox = no;
		fila->fim = no;
	}
}

NO* removeFila(FILA* fila) {
	NO* no = fila->ini;
	if(!no) return NULL;

	fila->ini = no->prox;
	no->prox = NULL;

	return no;
}

void inserePilha(PILHA *pilha, int ch)
{
	NO *no = (NO *)malloc(sizeof(NO));
	no->ch = ch;

	if (!pilha->topo)
	{
		pilha->topo = no;
		no->prox = NULL;
	}
	else
	{
		no->prox = pilha->topo;
		pilha->topo = no;
	}
}

NO *removePilha(PILHA *pilha)
{
	if (!pilha->topo)
		return NULL;

	NO *no = pilha->topo;
	pilha->topo = no->prox;
	no->prox = NULL;

	return no;
}

void imprimeCaminho(Grafo *grafo, int *antecessor)
{
	PILHA *pilha = inicializaPilha();
	for (int v = 0; v < obtemNrVertices(grafo); v++)
	{
		int a = antecessor[v];

		while (a != VERTICE_INVALIDO)
		{
			inserePilha(pilha, a);
			a = antecessor[a];
		}

		NO *no = removePilha(pilha);
		while (no != NULL)
		{
			printf("%d ", no->ch);

			no = removePilha(pilha);
		}

		printf("%d ", v);
		puts("");
	}
}

void imprimeComponentesConectados(Grafo *grafo, bool **componentes_conectados, int qtd)
{
	puts("Componentes conectados:");
	for (int i = 0; i < qtd; i++)
	{
		printf("C%d: ", i + 1);
		for (int j = 0; j < obtemNrVertices(grafo); j++)
		{
			if (componentes_conectados[i][j])
			{
				printf("%d ", j);
			}
		}
		puts("");
	}
}

void visitaBP(int v, Grafo *grafo, int *tempo, Cor *cor, int *tdesc, int *tterm, int *antecessor, bool prt, FILE *fp_saida, bool *componentes_conectados)
{
	componentes_conectados[v] = true;
	cor[v] = CINZA;
	tdesc[v] = ++(*tempo);
	if(prt) printf("%d ", v);

	Apontador aresta = primeiroListaAdj(v, grafo);

	while (aresta != AN)
	
	{
		int u = obtemVerticeDestino(aresta, grafo);

		if (cor[u] == BRANCO)
		{
			antecessor[u] = v;
			visitaBP(u, grafo, tempo, cor, tdesc, tterm, antecessor, prt, fp_saida, componentes_conectados);
		}
		

		tterm[v] = ++(*tempo);
		cor[v] = PRETO;


		aresta = proxListaAdj(v, grafo, aresta);
	}
}

int* buscaProfundidade(Grafo *grafo, bool prt, FILE *arq_saida)
{
	if(prt) puts("BP:");
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

	int i = 0;
	bool *componentes_conectados[obtemNrVertices(grafo)]; // se o vertice v faz parte do componente conectado i, então componentes_conectados[i][v] = true, c.c. componentes_conectados[i][v] = false
	for (int v = 0; v < obtemNrVertices(grafo); v++)
		if (cor[v] == BRANCO)
		{
			componentes_conectados[i] = calloc(obtemNrVertices(grafo), sizeof(int));
			visitaBP(v, grafo, &tempo, cor, tdesc, tterm, antecessor, prt, arq_saida, componentes_conectados[i]);
			i++;
		}

	if(prt) {
		puts("");
		puts("");

		puts("Caminhos BP:");
		imprimeCaminho(grafo, antecessor);
		puts("");


		imprimeComponentesConectados(grafo, componentes_conectados, i);
		puts("");
	}

	return i;
}

void imprimeVerticesArticulacao(Grafo *grafo, int qtd_componentes)
{
	puts("Vertices de articulacao:");

	//VERIFICANDO SE I É VERTICE DE ARTICULACAO
	for(int i = 0; i < obtemNrVertices(grafo); i++) {
		Grafo* grafo_r = (Grafo*) malloc(sizeof(Grafo));
		inicializaGrafo(grafo_r, obtemNrVertices(grafo) - 1);
		
		for(int u = 0; u < obtemNrVertices(grafo); u++) {
			if(i == u) continue;
			int aresta = primeiroListaAdj(u, grafo);
			while(aresta != AN) {
				//TA CRASHANDO AQUI
				int v = obtemVerticeDestino(aresta, grafo);
				

				int u2 = u;
				int v2 = v;

				if(v != i) {
					if(u > i) u2--;
					if(v > i) v2--;
					insereAresta(u2, v2, obtemPesoAresta(u, v, grafo), grafo_r);	
				} 

				aresta = proxListaAdj(u, grafo, aresta);
			}
		}

		int novo_qtd = buscaProfundidade(grafo_r, false, NULL);
		if (novo_qtd > qtd_componentes) printf("%d ", i);
		liberaGrafo(grafo_r);
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

void visitaBL(int v, Grafo* grafo, Cor* cor, int* antecessor, int* distancia) {
	printf("%d ", v);
	cor[v] = CINZA;
	distancia[v] = 0;
	FILA* fila = inicializaFila();
	insereFila(fila, v);
	NO* no;
	while((no = removeFila(fila)) != NULL) {
		int w = no->ch;
		Apontador aresta = primeiroListaAdj(w, grafo);
		int u = obtemVerticeDestino(aresta, grafo);

		while(u != VERTICE_INVALIDO) {
			if(cor[u] == BRANCO) {
				printf("%d ", u);
				cor[u] = CINZA;
				antecessor[u] = w;
				distancia[u] = distancia[w] + 1;
				insereFila(fila, u);
			}

			aresta = proxListaAdj(w, grafo, aresta);
			u = obtemVerticeDestino(aresta, grafo);
		}

		cor[w] = PRETO;
	}

}

void buscaLargura(Grafo* grafo, FILE* fp_saida) {
	puts("BL:");
	int *antecessor = (int *)malloc(obtemNrVertices(grafo) * sizeof(int));
	int *distancia = (int *)malloc(obtemNrVertices(grafo) * sizeof(int));
	Cor *cor = (Cor *)malloc(obtemNrVertices(grafo) * sizeof(Cor));
	

	for (int v = 0; v < obtemNrVertices(grafo); v++)
	{
		cor[v] = BRANCO;
		antecessor[v] = VERTICE_INVALIDO;
		distancia[v] = INT_MAX;
	}

	for (int v = 0; v < obtemNrVertices(grafo); v++)
		if (cor[v] == BRANCO)
			visitaBL(v, grafo, cor, antecessor, distancia);

	puts("");
	puts("");

	puts("Caminhos BL:");
	imprimeCaminho(grafo, antecessor);
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

	FILE *fp_saida = fopen(arq_saida, "w");
	
	

	Grafo *grafo = ler_grafo(arq_entrada);


	buscaLargura(grafo, fp_saida);
	puts("");
	
	int* qtd_componentes = buscaProfundidade(grafo, true, fp_saida);
	imprimeVerticesArticulacao(grafo, qtd_componentes);
}
