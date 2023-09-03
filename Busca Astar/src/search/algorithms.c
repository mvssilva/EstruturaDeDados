
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "../ed/queue.h"
#include "../ed/deque.h"
#include "../ed/stack.h"
#include "../ed/hash.h"
#include "../ed/heap.h"
#include "busca.h"

int celula_hash(HashTable *h, void *key)
{
    Celula *c = (Celula *)key;
    return ((c->x * 83) ^ (c->y * 97)) % hash_table_size(h);
}

int celula_cmp(void *c1, void *c2)
{
    Celula *a = (Celula *)c1;
    Celula *b = (Celula *)c2;

    if (a->x == b->x && a->y == b->y)
        return 0;
    else
        return 1;
}


ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

ResultData a_star(Labirinto *l, Celula inicio, Celula fim)
{
    ResultData resposta = _default_result();

    HashTable * hash = hash_table_construct(577, celula_hash, celula_cmp);
    Heap * heap = heap_construct(hash);

    Celula * atual = celula_inicializacao(inicio.x, inicio.y);
    atual->dist_g = 0;
    atual->dist_h = calcular_distancia(inicio, fim);

    double * prio =(double *)malloc(sizeof(double));
    *prio = atual->dist_g + atual->dist_h; 
    heap_push(heap, atual, prio);
    
    Deque * caminho_atual = deque_create();
    deque_salvar_caminho(caminho_atual, atual, NULL);

    while (!heap_empty(heap)){
        double * min_pro = heap_min_priority(heap);
        free(min_pro);
        atual = (Celula*)heap_pop(heap);
        labirinto_atribuir(l, atual->y, atual->x, EXPANDIDO);
        resposta.nos_expandidos++;
                
        if(atual->x == fim.x && atual->y == fim.y){
            resposta.sucesso = 1;
            break;
        }
        heap_celulas_adjacentes(atual, heap, l, caminho_atual, fim);
    }

    if(resposta.sucesso == 0){
        eliminar_todo_o_deque(caminho_atual);
        eliminar_hash(hash);
        deque_destroy(caminho_atual);
        heap_destroy(heap);
        return _default_result();
    }

    Stack * s = stack_construct();
    while(deque_size(caminho_atual) > 0){
        void * prev = deque_pop_back(caminho_atual);
        Celula * pai = retornar_pai(prev);
        Celula * filho = retornar_filho(prev);
        if(prev){
            if(filho->x == inicio.x && filho->y == inicio.y){
                stack_push(s, atual);
            }
            else if(filho->x == atual->x && filho->y == atual->y){
                stack_push(s, filho);
                atual = (Celula*)pai;  
                resposta.custo_caminho += calcular_distancia(*filho, *pai);
            }else{
                free(filho);
            }
        }
        free(prev);
    }
    resposta.tamanho_caminho = stack_size(s);
    resposta.caminho = (Celula *)malloc(resposta.tamanho_caminho * sizeof(Celula));
    for(int i = 0; i < resposta.tamanho_caminho; i++){
        Celula * caminho = (Celula*)stack_pop(s);
        resposta.caminho[i] = (*caminho);
        free(caminho);
    }
    stack_destroy(s);
        
    eliminar_hash(hash);
    deque_destroy(caminho_atual);
    heap_destroy(heap);
    return resposta;
}

ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim)
{   
    ResultData resposta = _default_result();

    Queue * lista_validas = queue_construct();
    Celula * atual = (Celula*)malloc(sizeof(Celula));
    atual->x = inicio.x;
    atual->y = inicio.y;
    queue_push(lista_validas, atual);

    Deque * d = deque_create();
    deque_salvar_caminho(d, atual, NULL);

    while(1){
        if(atual->x == fim.x && atual->y == fim.y){
            resposta.sucesso = 1;
            resposta.nos_expandidos++;
            break;
        }

        if(atual->x == inicio.x && atual->y == inicio.y){
            labirinto_atribuir(l, atual->y, atual->x, INICIO);
            enfileirar_celulas_adjacentes(atual, lista_validas, l, d);
            atual = (Celula*)queue_pop(lista_validas);
        }
        else{
            enfileirar_celulas_adjacentes(atual, lista_validas, l, d);
            labirinto_atribuir(l, atual->y, atual->x, EXPANDIDO);
        }
        resposta.nos_expandidos++;
        atual = (Celula*)queue_pop(lista_validas);
        
        if(atual == NULL){
            resposta.sucesso = 0;
            break;
        }

    }

    if(resposta.sucesso == 0){
        eliminar_todo_o_deque(d);
        deque_destroy(d);
        queue_destroy(lista_validas);
        return _default_result();
    }

        Stack * s = stack_construct();
        while(deque_size(d) > 0){
            void * prev = deque_pop_front(d);
            Celula * pai = retornar_pai(prev);
            Celula * filho = retornar_filho(prev);

            if(prev){
                if(filho->x == inicio.x && filho->y == inicio.y){
                    stack_push(s, atual);
                }
                else if(filho->x == atual->x && filho->y == atual->y){
                    stack_push(s, filho);
                    atual = (Celula*)pai;  
                    resposta.custo_caminho += calcular_distancia(*filho, *pai);
                }else{
                    free(filho);
                }
            }
            free(prev);
        }
        resposta.tamanho_caminho = stack_size(s);
        resposta.caminho = (Celula *)malloc(resposta.tamanho_caminho * sizeof(Celula));
        for(int i = 0; i < resposta.tamanho_caminho; i++){
            Celula * caminho = (Celula*)stack_pop(s);
            resposta.caminho[i] = (*caminho);
            free(caminho);
        }
        stack_destroy(s);
    
    deque_destroy(d);
    queue_destroy(lista_validas);
    
    return resposta;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    
    ResultData resposta = _default_result();

    Stack * lista_validas = stack_construct();
    Celula * atual = (Celula*)malloc(sizeof(Celula));
    atual->x = inicio.x;
    atual->y = inicio.y;
    stack_push(lista_validas, atual);

    Deque * d = deque_create();
    deque_salvar_caminho(d, atual, NULL);

    while(1){
        if(atual->x == fim.x && atual->y == fim.y){
            resposta.sucesso = 1;
            resposta.nos_expandidos++;
            break;
        }

        if(atual->x == inicio.x && atual->y == inicio.y){
            labirinto_atribuir(l, atual->y, atual->x, INICIO);
            empilhando_celulas_adjacentes(atual, lista_validas, l, d);
        }
        else{
            empilhando_celulas_adjacentes(atual, lista_validas, l, d);
            labirinto_atribuir(l, atual->y, atual->x, EXPANDIDO);
        }
        atual = (Celula*)stack_pop(lista_validas);
        resposta.nos_expandidos++;
        
        if(atual->x == inicio.x && atual->y == inicio.y){
            resposta.sucesso = 0;
            break;
        }

    }

    if(resposta.sucesso == 0){
        eliminar_todo_o_deque(d);
        deque_destroy(d);
        stack_destroy(lista_validas);
        return _default_result();
    }

        Stack * s = stack_construct();
        while(deque_size(d) > 0){
            void * prev = deque_pop_front(d);
            Celula * pai = retornar_pai(prev);
            Celula * filho = retornar_filho(prev);

            if(prev){
                if(filho->x == inicio.x && filho->y == inicio.y){
                    stack_push(s, atual);
                }
                else if(filho->x == atual->x && filho->y == atual->y){
                    stack_push(s, filho);
                    atual = (Celula*)pai;  
                    resposta.custo_caminho += calcular_distancia(*filho, *pai);
                }else{
                    free(filho);
                }
            }
            free(prev);
        }
        resposta.tamanho_caminho = stack_size(s);
        resposta.caminho = (Celula *)malloc(resposta.tamanho_caminho * sizeof(Celula));
        for(int i = 0; i < resposta.tamanho_caminho; i++){
            Celula * caminho = (Celula*)stack_pop(s);
            resposta.caminho[i] = (*caminho);
            free(caminho);
        }
        stack_destroy(s);
    
    deque_destroy(d);
    stack_destroy(lista_validas);
    
    return resposta;
}

ResultData dummy_search(Labirinto *l, Celula inicio, Celula fim)
{
    int max_path_length = 0;
    float dx, dy;

    ResultData result = _default_result();

    max_path_length = abs(fim.x - inicio.x) + abs(fim.y - inicio.y);
    result.caminho = (Celula *)malloc(sizeof(Celula) * max_path_length);
    result.sucesso = 1;

    Celula atual = inicio;
    result.caminho[result.tamanho_caminho++] = atual;
    result.nos_expandidos++;

    while ((atual.x != fim.x) || (atual.y != fim.y))
    {
        dx = fim.x - atual.x;
        dy = fim.y - atual.y;

        if (dx != 0)
            dx /= fabs(dx);

        if (dy != 0)
            dy /= fabs(dy);

        atual.x += (int)dx;
        atual.y += (int)dy;

        if (labirinto_obter(l, atual.y, atual.x) == OCUPADO || (atual.x > labirinto_n_colunas(l) - 1) || (atual.y > labirinto_n_linhas(l) - 1) || (atual.x < 0) || (atual.y < 0))
        {
            result.sucesso = 0;
            free(result.caminho);
            result.caminho = NULL;
            return result;
        }

        result.caminho[result.tamanho_caminho++] = atual;
        result.nos_expandidos++;
        result.custo_caminho += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return result;
}
