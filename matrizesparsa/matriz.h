#ifndef MATRIZ_ESPARCA_H
#define MATRIZ_ESPARCA_H

#include "node.h"

typedef struct Coluna{
    Node * head;
    int coluna;
}Coluna;

typedef struct Linha{
    Node * head;
    int linha;
}Linha;

typedef struct Matriz{
    Coluna * list_coluna;
    Linha * list_linha;
    int size_l;
    int size_c;
    int alocado_c;
    int alocado_l;
}Matriz;

/**
 * Construção e inicialização de uma struct Matriz;
 * Alocação de memória para um ponteiro Matriz e duas listas encadeadas (colunas, linhas);
 * Função com tempo constante - O(1);
 * @return Matriz*
**/
Matriz * matriz_construir();

/**
 * Recebe um ponteiro de um novo node adicionado e insere na matriz;
 * Internamente realiza a verificacao da posição, se necessário aumenta o tamanho da matriz;
 * Salva somente node com valores diferentes de 0;
 * Funcao com complexidade de tempo = O(n+m);
 * n = quantida de colunas (não nulas)
 * m = quantidade de linhas(não nulas);
**/
void matriz_inserir_valores(Node *, Matriz *);

/**
 * Recebe a posição do valor solicitado e um ponteiro para a matriz;
 * Função de complexidade de tempo = O(n);
 * n = quantida de colunas não nulas;
*/
float *matriz_mostrar_valor(Matriz *, int, int);

/**
 * Recebe um ponteiro para a matriz;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas;
 * m = quantidade de linhas;
*/
void imprimir_matriz_denso(Matriz *);

/**
 * Recebe um ponteiro para a matriz;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas (não nulas)
 * m = quantidade de linhas(não nulas);
*/
void matriz_imprimir_esparco(Matriz*);

/**
 * Incompleta
 * Não tratei casos em que a troca de colunas ou linhas envolve posições zeradas(null);
*/
void matriz_swap_entre_linhas(Matriz *);
void matriz_swap_entre_colunas(Matriz *);

/**
 * Recebe um ponteiro para a matriz;
 * Funcao de complexidade de tempo = O(n x m);
 * n = quantida de colunas;
 * m = quantidade de linhas;
*/
void matriz_destruir(Matriz *);

#endif