#include <stdio.h>
#include <stdlib.h>
#include "node.h"

Node * node_construir(int linha, int coluna, data_type value){
    Node * n = (Node *)malloc(sizeof(Node));
    n->coluna = coluna;
    n->linha = linha;
    n->valor = value;
    n->next_na_coluna = NULL;
    n->next_na_linha = NULL;
    return n;
}

void node_printf(Node * n){
    printf("Valor= %f\nLinha= %d\nColuna= %d\n", n->valor, n->linha, n->coluna);
}

void node_destruir(Node * n){
    free(n);
}
