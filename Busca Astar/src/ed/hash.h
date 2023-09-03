#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "list.h"

typedef struct HashTableItem HashTableItem;
struct HashTableItem{
    void * cel;
    void * idx_heap;
};
typedef struct BlockHash BlockHash;
typedef struct HashTable HashTable;
typedef int (*HashFunction)(HashTable *, void *);
typedef int (*CmpFunction)(void *k1, void *k2);

typedef struct NodeIterador NodeIterador;
typedef struct HashTableIterator HashTableIterator;


HashTable * hash_table_construct(int, HashFunction celula_hash, CmpFunction celula_cmp);
void * hash_table_set(HashTable *, void*, void*);
int hash_table_size(HashTable*);
void *hash_table_get(HashTable *, void *);
void hash_table_destroy(HashTable *);
void hash_remove(HashTable *, void*);
void * hash_return_cel(HashTableItem *);
void *hash_table_find(HashTable *, void *);   
void hash_table_att(void *, HashTable*, void*);
void *hash_recupera_index(void *, HashTable*);



HashTableIterator * hash_table_iterator(HashTable*);
void hash_table_iterator_destroy(HashTableIterator*);
HashTableItem* hash_table_iterator_next(HashTableIterator * );
int hash_table_iterator_is_over(HashTableIterator*);
#endif
