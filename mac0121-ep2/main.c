#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz.h"
#include "pilha.h"

/* COMO COMPILAR
// gcc -Wall -ansi -pedantic -O0 -g -std=c0 099 main.c pilha.c matriz.c -o a

quando houver apenas os arquivos necessários,
gcc -Wall -ansi -pedantic -O2 *.c -o a

./a
*/

/* NOTAÇÃO
voc => vocabulário - array dos inputs das palavras
*/



/* DECLARAÇÃO DOS PROTÓTIPOS */
int cruzada (int direcao, char **matrizH, char **matrizV, int *linAtual, int *colAtual, int linhas, int colunas, int nPal, pal *voc, pal *palAntes, pilha *pilhaPal);
int admitePalH (char **matrizH, int linAtual, int colAtual, int colunas);
int admitePalV (char **matrizV, int linAtual, int colAtual, int linhas);
int admitePal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas);
int contaEspH (char **matrizH, int linAtual, int colAtual, int colunas);
int contaEsp (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas);
pal *procuraPalH (char **matrizH, char **matrizV, int linAtual, int colAtual, int colunas, int tamLiv, pal *voc, int nPal, pal *palAntes);
pal *procuraPal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas, int tamLiv, pal *voc, int nPal, pal *p);
void encaixaPalH (char **matrizH, int linAtual, int colAtual, pal *p);
void encaixaPal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, pal *p);
void deletaPalMatH (char **matrizH, pal *p);
void deletaPalMat (int direcao, char **matrizH, char **matrizV, pal *p);
void saltaInds (int *linAtual, int *colAtual, pal *p);
void incInds (int *linAtual, int *colAtual, int linhas, int colunas);




/* DECLARAÇÃO DAS FUNÇÕES */
int main(){
    int instancia = 1;
	FILE *file = fopen("input.txt", "r");
	if (file == NULL){
		printf("ERRO\n");
		return 0;
	}

    while(1){
        int i, m, n, p, haSolucao;
        int linAtual, colAtual;
        char **matrizH, **matrizV;
        pal *voc; pal *palAntes;
        pilha *pilhaPal;


        /* INPUT DO USUARIO */
        /*scanf("%d %d", &m, &n);*/
		fscanf(file, "%d %d", &m, &n);
        if (m == 0 && n == 0)
            break;

        
        /* Alocacao das matrizes */
        matrizH = alocaMatriz(m, n);
        matrizV = alocaMatriz(m, n);
        leMatriz(matrizH, matrizV, m, n, &file);
        imprimeMatriz(matrizH, m, n);

        /* Alocacao do voc */
        fscanf(file, "%d", &p);
        voc = malloc(p * sizeof(pal));
        lePalavras(voc, p, &file);
        ordenaVoc(voc, p);
		for (i = 0; i < p; i++)
			voc[i].posvoc = i;
        imprimeVoc(voc, p);

        /* Criacao da pilha */
        pilhaPal = criaPilha(p);

		linAtual = colAtual = 0;
		palAntes = NULL;

        haSolucao = cruzada(0, matrizH, matrizV, &linAtual, &colAtual, m, n, p, voc, palAntes, pilhaPal);


        /* FINALIZACAO DA RODADA*/
        printf("Instancia %d:\n", instancia);
        if (haSolucao){
			/*imprimeMatriz(matrizH, m, n);
			imprimeMatriz(matrizV, m, n);*/
			mesclaMatrizes(matrizH, matrizV, m, n);
            imprimeMatriz(matrizH, m, n);
		}
        else
            printf("nao ha solucao\n");

        destroiPilha(pilhaPal);
        free(voc);
        liberaMatriz(matrizH, m);
		liberaMatriz(matrizV, m);

		instancia++;
    }

    return 0;
}


/* direção == 0: horizontal   direção == 1: vertical */
int cruzada (int direcao, char **matrizH, char **matrizV, int *linAtual, int *colAtual, int linhas, int colunas, int nPal, pal *voc, pal *palAntes, pilha *pilhaPal){
	int tamLiv, admite;
	pal *palAtual; item palTopo;

	printf("\ndirecao: %d\n", direcao);
	printf("linAtual: %d   colAtual: %d\n", *linAtual, *colAtual);

	if (*linAtual == linhas){
		return 1;
	}
	
	admite = admitePal(direcao, matrizH, matrizV, *linAtual, *colAtual, linhas, colunas);
	printf("admite: %d\n", admite);

	if (admite == 1){
		tamLiv = contaEsp(direcao, matrizH, matrizV, *linAtual, *colAtual, linhas, colunas);
		printf("tamLiv: %d\n", tamLiv);

		palAtual = procuraPal(direcao, matrizH, matrizV, *linAtual, *colAtual, linhas, colunas, tamLiv, voc, nPal, palAntes);

		if (palAtual != NULL){
			empilha(pilhaPal, palAtual);
			encaixaPal(direcao, matrizH, matrizV, *linAtual, *colAtual, palAtual);
			imprimeMatriz(matrizH, linhas, colunas);
			printf("\n");
			imprimeMatriz(matrizV, linhas, colunas);
			if (direcao == 1)
				incInds(linAtual, colAtual, linhas, colunas);
			
			return cruzada((direcao+1)%2, matrizH, matrizV, linAtual, colAtual, linhas, colunas, nPal, voc, NULL, pilhaPal);
		}
		else{ /* Backtracking */
			if (pilhaVazia(pilhaPal))
				return 0;
			else{
				palTopo = desempilha(pilhaPal);
				deletaPalMat(direcao, matrizH, matrizV, palTopo);
				saltaInds(linAtual, colAtual, palTopo);

				return cruzada(palTopo->dir, matrizH, matrizV, linAtual, colAtual, linhas, colunas, nPal, voc, palTopo, pilhaPal);	
			}
		}
	}
	else if (admite == 0){
		if (direcao == 1)
			incInds(linAtual, colAtual, linhas, colunas);
		return cruzada((direcao+1)%2, matrizH, matrizV, linAtual, colAtual, linhas, colunas, nPal, voc, NULL, pilhaPal);
	}
	else{ /* admite == -1, ou seja, a célula tem um '*' */
		incInds(linAtual, colAtual, linhas, colunas);
		return cruzada(0, matrizH, matrizV, linAtual, colAtual, linhas, colunas, nPal, voc, NULL, pilhaPal);
	}
}


/* Verifica se a célula da matriz admite palavra horizontalmente
Admite quando:
- a célula não é um '*' e não está na borda direita
e
- a célula da direita não é um '*'
e
(
	- a célula está na borda direita
	ou
	- a célula não está na borda direita, mas a célula à esquerda é um '*'
)
*/
int admitePalH (char **matrizH, int linAtual, int colAtual, int colunas){
	if (matrizH[linAtual][colAtual] != '*' && colAtual != colunas){
		if (matrizH[linAtual][colAtual+1] != '*'){
			if (colAtual == 0)
				return 1;
			else{
				if (matrizH[linAtual][colAtual-1] == '*')	
					return 1;
				else
					return 0;
			}
		}
		else
			return 0;
	}
	else
		return 0; 
}

int admitePalV (char **matrizV, int linAtual, int colAtual, int linhas){
	if (matrizV[linAtual][colAtual] != '*' && linAtual != linhas){
		if (matrizV[linAtual+1][colAtual] != '*'){
			if (linAtual == 0)
				return 1;
			else{
				if (matrizV[linAtual-1][colAtual] == '*')	
					return 1;
				else
					return 0;
			}
		}
		else
			return 0;
	}
	else
		return 0; 
}

int admitePal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas){
	if (matrizH[linAtual][colAtual] == '*')
		return -1;
	if (direcao == 0){
		if (colAtual != colunas - 1){
			if (matrizH[linAtual][colAtual+1] != '*'){
				if (colAtual == 0)
					return 1;
				else{
					if (matrizH[linAtual][colAtual-1] == '*')	
						return 1;
					else
						return 0;
				}
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else{
		if (linAtual != linhas - 1){
			if (matrizV[linAtual+1][colAtual] != '*'){
				if (linAtual == 0)
					return 1;
				else{
					if (matrizV[linAtual-1][colAtual] == '*')	
						return 1;
					else
						return 0;
				}
			}
			else
				return 0;
		}
		else
			return 0;
	}

}


/* Conta o espaço horizontal disponível desde a célula atual até um '*' ou até a linha acabar */
int contaEspH (char **matrizH, int linAtual, int colAtual, int colunas){
    int j, tamanho = 1;

    for (j = colAtual; j < colunas - 1; j++){
        if (matrizH[linAtual][j] == '*')
			break;
		tamanho++;
	}
	return tamanho;
}

int contaEspV (char **matrizV, int linAtual, int colAtual, int linhas){
    int i, tamanho = 1;

    for (i = linAtual; i < linhas - 1; i++){
        if (matrizV[i][colAtual] == '*')
			break;
		tamanho++;
	}
	return tamanho;
}

int contaEsp (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas){
    int i, tamanho = 1;

	if (direcao == 0){
		for (i = colAtual + 1; i < colunas; i++){
			if (matrizH[linAtual][i] == '*')
				break;
			tamanho++;
		}
	}
	else{
		for (i = linAtual + 1; i < linhas; i++){
			if (matrizV[i][colAtual] == '*')
				break;
			tamanho++;
		}
	}

	return tamanho;
}


/* Procura uma palavra com um tamanho específico no array vocabulario para inserí-la horizontalmente */
pal *procuraPalH (char **matrizH, char **matrizV, int linAtual, int colAtual, int colunas, int tamLiv, pal *voc, int nPal, pal *palAntes){
	int k, j, encaixa = 1;

	/* Procura todas as palavras com tamanho igual ao tamLiv à partir de p */
	if (palAntes == NULL) k = 0; else k = (palAntes->posvoc) + 1;

	for (; k < nPal; k++){
		if (!voc[k].naPilha && voc[k].len == tamLiv){
			for (j = colAtual; j < colAtual + tamLiv && encaixa; j++){
				if (matrizV[linAtual][j] != '0' && voc[k].string[j] != matrizV[linAtual][j]) 
					encaixa = 0;
			}

			if (encaixa){
				voc[k].naPilha = 1;
				voc[k].dir = 0;
				voc[k].posmat[0] = linAtual;
				voc[k].posmat[1] = colAtual;		
				return &voc[k];
			}
		}
		else if (voc[k].len > tamLiv)
			return NULL;
	}

	return NULL;
}


pal *procuraPal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, int linhas, int colunas, int tamLiv, pal *voc, int nPal, pal *palAntes){
	int j, k, l, encaixa;

	/*
	j: índice para percorrer a matriz
	k: índice para percorrer o vocabulário
	l: índice para percorrer a string da palavra do vocabulário
	*/

	/* Procura todas as palavras com tamanho igual ao tamLiv à partir de da próxima palavra de p*/
	if (palAntes == NULL) k = 0; else k = palAntes->posvoc + 1;

	printf("k: %d\n", k);

	for (; k < nPal; k++){
		printf("voc[k]: %s\n", voc[k].string);
		encaixa = 1;
		if (!voc[k].naPilha && voc[k].len == tamLiv){
			if (direcao == 0){
				j = colAtual; l = 0;
				while(j < colAtual + tamLiv && encaixa){
					if (matrizV[linAtual][j] != '0' && voc[k].string[l] != matrizV[linAtual][j]) 
						encaixa = 0;
					l++; j++;
				}
			}
			else{
				j = linAtual; l = 0;
				while(j < linAtual + tamLiv && encaixa){
					if (matrizH[j][colAtual] != '0' && voc[k].string[l] != matrizH[j][colAtual]) 
						encaixa = 0;
					l++; j++;
				}
			}

			if (encaixa){
				voc[k].naPilha = 1;
				voc[k].dir = direcao;		
				voc[k].posmat[0] = linAtual;
				voc[k].posmat[1] = colAtual;		
				return &voc[k];
			}
		}
		else if (voc[k].len > tamLiv)
			return NULL;
	}

	return NULL;
}

/* Insere uma palavra na matriz horizontal */
void encaixaPalH (char **matrizH, int linAtual, int colAtual, pal *p){
	int j, k;
	j = colAtual;
	k = 0;
	while (k < p->len){
		matrizH[linAtual][j] = p->string[k];
		j++; k++;
	}
}

/* Insere uma palavra na matriz horizontal */
void encaixaPal (int direcao, char **matrizH, char **matrizV, int linAtual, int colAtual, pal *p){
	int j, k;
	if (direcao == 0){
		j = colAtual;
		k = 0;
		while (k < p->len){
			matrizH[linAtual][j] = p->string[k];
			j++; k++;
		}
	}
	else{
		j = linAtual;
		k = 0;
		while (k < p->len){
			matrizV[j][colAtual] = p->string[k];
			j++; k++;
		}
	}
}

/* Substitui uma palavra escrita na matriz por '0's */
void deletaPalMatH (char **matrizH, pal *p){
	int i;
	for (i = p->posmat[0]; i < p->posmat[0] + p->len; i++)
		matrizH[i][p->posmat[1]] = '0';
}

/* Substitui uma palavra escrita na matriz por '0's */
void deletaPalMat (int direcao, char **matrizH, char **matrizV, pal *p){
	int i;
	p->naPilha = 0;
	if (direcao == 0){
		for (i = p->posmat[0]; i < p->posmat[0] + p->len; i++)
			matrizH[i][p->posmat[1]] = '0';
	}
	else{
		for (i = p->posmat[1]; i < p->posmat[1] + p->len; i++)
			matrizH[p->posmat[0]][i] = '0';
	}
} 


/* Muda os índices que percorrem a matriz para a posição da palavra desempilhada no caso de backtracking */
void saltaInds (int *linAtual, int *colAtual, pal *p){
	*linAtual = p->posmat[0];
	*colAtual = p->posmat[1];
}

/* Muda os índices que percorrem a matriz para a próxima célula */
void incInds (int *linAtual, int *colAtual, int linhas, int colunas){
	*colAtual = (*colAtual + 1) % colunas;

	if (*colAtual == 0)
	 	*linAtual = *linAtual + 1;
}