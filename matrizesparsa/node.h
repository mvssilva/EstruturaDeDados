#ifndef NODE_H_MATRIZ
#define NODE_H_MATRIZ

typedef float data_type;

typedef struct Node{
    data_type valor;
    int linha;
    int coluna;
    struct Node * next_na_coluna;
    struct Node * next_na_linha;
}Node;

Node * node_construir(int linha, int coluna, data_type value);
void node_printf(Node *);
void node_destruir(Node *);
#endif
