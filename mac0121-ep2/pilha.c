#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"
#include <string.h>


/* Funcoes pilha */
pilha * criaPilha (int tam){
  pilha * p = malloc(sizeof(pilha));
  p->tam = tam;
  p->topo = 0;
  p->v = malloc(tam * sizeof(item));
  if (p->v == NULL)
	return NULL;
  return p;
}

void destroiPilha (pilha * p) {
  free (p->v);
  free (p);
}

void empilha (pilha * p, item x) {
  if (p->topo == p->tam)
	p = resize (p);
  p->v[p->topo] = x;
  p->topo ++;
}

item desempilha (pilha * p) {
  if (!pilhaVazia (p)){
	p->topo --;
	return (p->v[p->topo]);
  }
}

int pilhaVazia (pilha * p) {
  return (p->topo == 0);
}

item topoDaPilha (pilha *p) {
  if (!pilhaVazia(p)){
	return (p->v[p->topo - 1]);
  }
}

/* dobra o tamanho da pilha */
pilha * resize (pilha * p) {
	pilha * q = criaPilha (p->tam * 2);
	int i;
	for (i = 0; i < p->topo; i++)
		q->v[i] = p->v[i];
	q->topo = p->topo;
	destroiPilha (p);
	return (q);
}



/* Funcoes palavra-cruzada */
void lePalavras(pal *voc, int npal, FILE **input){
	int i;
	char pal[46];

	for (i = 0; i < npal; i++){
		fscanf(*input, "%s", pal);
		voc[i].len = strlen(pal);
		strcpy(voc[i].string, pal);
	}
}

void imprimeVoc(pal *voc, int npal){
	int i;
	for (i = 0; i < npal; i++){
		printf("VOCABULARIO\n");
		printf("%d %s %d\n", voc[i].posvoc, voc[i].string, voc[i].len);
	}
}

void ordenaVoc(pal *voc, int npal){
	int i, j;
	pal chave;
   
	for (i = 1; i < npal; i++) {
    	chave = voc[i];
    	j = i - 1;

    	while (j >= 0 && voc[j].len > chave.len) {
      		voc[j + 1] = voc[j];
      		j = j - 1;
    	}

    	voc[j + 1] = chave;
  	}
}	