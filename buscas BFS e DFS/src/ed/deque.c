#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

typedef struct NodeDeque NodeDeque;

struct NodeDeque{
    NodeDeque * next;
    NodeDeque * prev;
    void * data;
};

struct Deque{
    NodeDeque * head;
    NodeDeque * last;
    int qtd;
};

Deque *deque_create(){
    Deque * d = (Deque *)malloc(sizeof(Deque));
    d->head = NULL;
    d->last = NULL;
    d->qtd = 0;
    return d;
}

NodeDeque * _node_construir(void * item, NodeDeque * prev, NodeDeque * next){
    NodeDeque * n = (NodeDeque*)malloc(sizeof(NodeDeque));
    n->next = next;
    n->prev = prev;
    n->data = item;
    return n;
}

void deque_push_back(Deque *d, void *val){
    //inserir no final
    NodeDeque * n = d->last;

    if(n == NULL){
        NodeDeque * new = _node_construir(val, NULL, NULL);
        d->head = d->last = new;
    }
    else{
        NodeDeque * new = _node_construir(val, d->last, d->head);
        d->last->next = new;
        d->head->prev = new;
        d->last = new;
    }
    d->qtd++;

}

void deque_push_front(Deque *d, void *val){
    //inserir no final
    NodeDeque * n = d->head;

    if(n == NULL){
        NodeDeque * new = _node_construir(val, NULL, NULL);
        d->head = d->last = new;
    }
    else{
        NodeDeque * new = _node_construir(val, d->last,  d->head);
        d->last->next = new;
        d->head->prev = new;
        d->head = new;
    }
    d->qtd++;
}

void _node_destruir(NodeDeque* n){
    free(n);
}


void *deque_pop_back(Deque *d){
    NodeDeque * n = d->last;
    void * item = NULL;
    
    if(n == NULL){
        exit(printf("Fila Vazia!\n"));
    }else{
        if(n == d->head){ // FILA COM 01 ITEM
            item = n->data;
            _node_destruir(n);
            d->head = NULL;
            d->last = NULL;
            d->qtd--;
        }else{
            item = n->data;
            d->last = n->prev;
            d->last->next = d->head;
            d->head->prev = d->last;
            _node_destruir(n);
            d->qtd--;
        }
    }
    return item;
}


void *deque_pop_front(Deque *d){
    NodeDeque * n = d->head;
    void * item = NULL;
    
    if(n == NULL){
        exit(printf("DEQUE Vazia!\n"));
    }else{
        if(n == d->last){ // FILA COM 01 ITEM
            item = n->data;
            _node_destruir(n);
            d->head = NULL;
            d->last = NULL;
            d->qtd--;
        }else{
            item = n->data;
            d->head = n->next;
            d->head->prev = d->last;
            d->last->next = d->head;
            _node_destruir(n);
            d->qtd--;
        }
    }
    return item;
}

void deque_destroy(Deque *d){
    NodeDeque * n = d->head;

    while(d->qtd > 0){

        NodeDeque * aux = n->next;
        free(n->data);
        free(n);
        d->qtd--;
        if(d->qtd == 0)
            break;
        
        n = aux;
    }

    free(d);
}

int deque_size(Deque *d){
    return d->qtd;
}

void *deque_get(Deque *d, int idx){
    NodeDeque * n = d->head;
    int i;

    for(i = 0; i < idx; i++){
        n = n->next;
        
        if(n == NULL)
            break;
    }
    
    if(idx == i)
        return n->data;
    else
        return NULL;
}