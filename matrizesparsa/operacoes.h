#ifndef SOMA_MATRIZ_H
#define SOMA_MATRIZ_H
#include "matriz.h"

/**
 * Recebe um ponteiro para uma matriz;
 * Realiza a leitura de um valor;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas (não nulas)
 * m = quantidade de linhas(não nulas);
*/
void multiplicacao_por_escalar(Matriz*);

/**
 * Recebe três ponteiro para matrizes, p1 + p2 = p3;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas;
 * m = quantidade de linhas;
*/
void soma_matrizes(Matriz *, Matriz *, Matriz *);

/**
 * Recebe três ponteiro para matrizes, p1 X p2 = p3;
 * Funcao de complexidade de tempo = O(n1 x m2)(n2 x m1);
 * n1 = quantida de colunas da matriz 1;
 * n2 = quantida de colunas da matriz 2;
 * m1 = quantidade de linhas da matriz 1; 
 * m2 = quantidade de linhas da matriz 2;
*/
void multiplicacao_matriz_x_matriz(Matriz *, Matriz *, Matriz *);  

/**
 * Recebe três ponteiro para matrizes, p1 X p2 = p3;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas;
 * m = quantidade de linhas;
*/
void multiplicacao_matriz_ponto_a_ponto(Matriz *, Matriz *, Matriz *);        


/**
 * Recebe 2 ponteiro para matrizes;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas da parte retirada;
 * m = quantidade de linhas da parte retirada;
*/
void recortar_matriz(Matriz*, Matriz*);




#endif