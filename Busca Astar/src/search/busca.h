#ifndef _H_BUSCA_H
#define _H_BUSCA_H

#include "algorithms.h"
#include "labirinto.h"
#include "../ed/queue.h"
#include "../ed/deque.h"
#include "../ed/stack.h"
#include "../ed/hash.h"
#include "../ed/heap.h"

void enfileirar_celulas_adjacentes(Celula*, Queue*, Labirinto*, Deque*);
void empilhando_celulas_adjacentes(Celula *, Stack *, Labirinto*, Deque*);
void heap_celulas_adjacentes(Celula *, Heap *, Labirinto*, Deque*, Celula);
int verificando_celulas(Celula*, Labirinto*);
void deque_salvar_caminho(Deque*, Celula*, Celula*);
void *retornar_pai(void *prev);
void *retornar_filho(void *prev);
double calcular_distancia(Celula , Celula );
void eliminar_todo_o_deque(Deque *);
void eliminar_hash(HashTable*);
double calcular_custo_provisorio(Deque *, Celula, Celula);
Celula *celula_inicializacao(int, int);


#endif