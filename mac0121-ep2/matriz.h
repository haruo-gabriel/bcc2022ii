#include <stdio.h>

char **alocaMatriz (int lin, int col);
void liberaMatriz (char ** mat, int lin);
void imprimeMatriz (char **a, int m, int n);
void leMatriz (char **matrizH, char **matrizV, int linhas, int colunas, FILE **input);
void mesclaMatrizes (char **matrizH, char **matrizV, int linhas, int colunas);