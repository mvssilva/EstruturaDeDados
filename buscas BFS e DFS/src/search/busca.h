#ifndef _H_BUSCA_H
#define _H_BUSCA_H

#include "algorithms.h"
#include "labirinto.h"
#include "../ed/queue.h"
#include "../ed/deque.h"
#include "../ed/stack.h"

void emqueue_celulas_ao_redor(Celula*, Queue*, Labirinto*, Deque*);
void stackando_celulas_ao_redor(Celula *, Stack *, Labirinto*, Deque*);
int verificando_celulas(Celula*, Labirinto*);
void deque_salvar_caminho(Deque*, Celula*, Celula*);
void *retornar_pai(void *prev);
void *retornar_filho(void *prev);
float calcular_distancia(Celula , Celula );
void eliminar_todo_o_deque(Deque *);

#endif