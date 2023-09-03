
#ifndef _HEAP_H_
#define _HEAP_H_

#include "hash.h"
#include "types.h"

typedef struct Heap Heap;
typedef struct NodeHeap NodeHeap;

Heap *heap_construct(HashTable*);
void* heap_push(Heap *, void *, double *);
bool heap_empty(Heap *heap);
void *heap_max(Heap *heap);
double *heap_min_priority(Heap *heap);
void *heap_pop(Heap *heap);
void heapify_up(Heap *, int);
void heapify_down(Heap*);
void heap_destroy(Heap *heap);
void *heap_remove(Heap*, int);
void heap_printf_valores(Heap*);

#endif