#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"

struct NodeHeap{
    void *cel;
    double *priority;
};

struct Heap{
    NodeHeap *nodes;
    int size;
    int capacity;
    HashTable * hash;
};

#define inicializacao_vetor 5

Heap *heap_construct(HashTable * h){
    Heap * heap = (Heap*)malloc(sizeof(Heap));
    heap->capacity = inicializacao_vetor;
    heap->size = 0;
    heap->nodes = (NodeHeap*)calloc(heap->capacity, sizeof(NodeHeap));
    heap->hash = h;
    return heap;
}

void heapify_up(Heap * heap, int idx_atual){
    int idx_pai = (idx_atual - 1) / 2;
    NodeHeap * n = heap->nodes;
    while(idx_pai >= 0){
        if((*n[idx_atual].priority) < (*n[idx_pai].priority)){
            NodeHeap aux = n[idx_atual];
            n[idx_atual] = n[idx_pai];
            n[idx_pai] = aux;

            int *new_idx_pai = (int *)malloc(sizeof(int));
            (*new_idx_pai) = idx_pai;
            int *new_idx_atual = (int *)malloc(sizeof(int));
            (*new_idx_atual) = idx_atual;

            int *idx_antigo = hash_table_set(heap->hash, heap->nodes[idx_pai].cel, new_idx_pai);
            free(idx_antigo);
            idx_antigo = hash_table_set(heap->hash, heap->nodes[idx_atual].cel, new_idx_atual);
            free(idx_antigo);
            
            idx_atual = idx_pai;
            idx_pai = (idx_atual - 1) / 2;

        }else
            break;
    }
    heap->nodes = n;
}

NodeHeap * _heap_realloc_vetor(NodeHeap * n, int tam, int total){
    total *= 2;
    NodeHeap * new = (NodeHeap*)calloc(total, sizeof(NodeHeap));

    for (int i = 0; i < tam; i++){
        new[i].cel = n[i].cel;
        new[i].priority = n[i].priority;
    }
    free(n); 
    n = new;
    return n;
}

void * heap_push(Heap *heap, void *data, double * priority){
        
    void * prev = hash_table_find(heap->hash, data); 

    if(heap->size == heap->capacity){
        heap->nodes = _heap_realloc_vetor(heap->nodes, heap->size, heap->capacity);  
        heap->capacity *= 2;
    }

    if(prev){
        int * idx_heap = (int*)prev;
        NodeHeap x = heap->nodes[*idx_heap];

        double * num1 = x.priority;          //cadastro antigo
        double * num2 = priority;  
            
            if((*num1) > (*num2)){   
                *(heap->nodes[*idx_heap].priority) = *priority;
                heapify_up(heap, *idx_heap);
            }
        free(priority);
        return data;
    }else{
        heap->nodes[heap->size].cel = data;
        heap->nodes[heap->size].priority = priority;
        int *idx = (int *)malloc(sizeof(int));
        *idx = heap->size;
        hash_table_set(heap->hash, data, idx);
        heapify_up(heap, heap->size);
        heap->size++;
    }    
    return NULL;
}

double *heap_min_priority(Heap *heap){
    return heap->nodes->priority;
}

void *heap_pop(Heap *heap){

    int *idx_antigo = hash_table_find(heap->hash, heap->nodes[0].cel); 

    if(heap_empty(heap) || idx_antigo == NULL)
        return NULL;

    void * cel = heap->nodes[0].cel;
    if(heap->size - 1 == 0){
        hash_remove(heap->hash, cel);
        heap->size--;
    }else{
        heap->nodes[0] = heap->nodes[heap->size - 1];
        heap->size--;
        *idx_antigo = 0;
        
        hash_remove(heap->hash, cel);
        heapify_down(heap);
    }
    
    return cel;
}

void heapify_down(Heap* heap){
    int idx_current = 0;//analizando da raiz
    NodeHeap * n = heap->nodes;
    
    while(1){
        int idx_filho_esquerda = 2 * idx_current + 1;
        int idx_filho_direita = 2 * idx_current + 2;
        int idx_menor = idx_current;
        
        if (idx_filho_esquerda < heap->size && *(n[idx_current].priority) > *(n[idx_filho_esquerda].priority)) {
            idx_menor = idx_filho_esquerda;
        }

        if (idx_filho_direita < heap->size && *(n[idx_menor].priority) > *(n[idx_filho_direita].priority)) {
            idx_menor = idx_filho_direita;
        }

        if (idx_menor == idx_current) {
            break;
        }

        NodeHeap aux = n[idx_current];
        n[idx_current] = n[idx_menor];
        n[idx_menor] = aux;

        int *idx_current_heap = (int *)malloc(sizeof(int));
        int *idx_menor_heap = (int *)malloc(sizeof(int));

        (*idx_current_heap) = idx_current;
        (*idx_menor_heap) = idx_menor;

        int *lixo = hash_table_set(heap->hash, heap->nodes[idx_current].cel, idx_current_heap);
        free(lixo);
    
        lixo = hash_table_set(heap->hash, heap->nodes[idx_menor].cel, idx_menor_heap);
        free(lixo);

        idx_current = idx_menor; 
    }

}

bool heap_empty(Heap *heap){
    if(heap->size == 0)
        return 1;
    else
        return 0;
}

void _node_heap_destruct(NodeHeap * node){
    free(node);
}

void *heap_max(Heap *heap){
    return heap->nodes[0].priority;
}

void heap_destroy(Heap *heap){
    for(int i = 0; i < heap->size; i++){
        free(heap->nodes[i].priority);
    }
    free(heap->nodes);        
    free(heap);
}
