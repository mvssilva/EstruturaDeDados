#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

struct BlockHash{
    ForwardList * block;
};

struct HashTable{
    BlockHash * tabela;
    int qtd_block;
    int total_aloc;
    CmpFunction comparar;
    HashFunction calculo;
};

struct NodeIterador{
    ForwardList * list;
    NodeIterador * next;
};

struct HashTableIterator{
    NodeIterador * current;
    int size;
};


HashTable * hash_table_construct(int tam, HashFunction celula_hash, CmpFunction celula_cmp){
    HashTable *hash = (HashTable*)calloc(1, sizeof(HashTable));
    hash->total_aloc = tam;
    hash->qtd_block = 0;
    hash->tabela = (BlockHash*)calloc(hash->total_aloc, sizeof(BlockHash));
        for(int i = 0; i<hash->total_aloc; i++){
            hash->tabela[i].block = forward_list_construct();
        }
    hash->comparar = celula_cmp;
    hash->calculo = celula_hash;

    return hash;
}

HashTableItem * _hash_table_item_construc(void * cel, void * idx_heap){
    HashTableItem * h = (HashTableItem*)malloc(sizeof(HashTableItem));
    h->cel = cel;
    h->idx_heap = idx_heap;
    return h;
}

void * hash_table_set(HashTable * h, void* cel, void* idx_heap){
    HashTableItem * item = _hash_table_item_construc(cel, idx_heap);
    int idx = h->calculo(h, item->cel);
    BlockHash l = h->tabela[idx];
    
    if(forward_return_size(l.block) == 0){
        forward_list_push_back(l.block, item);
        h->tabela[idx] = l;
        h->qtd_block++;
        return NULL;
    }
    else{

        int cont = 0;
        while(forward_return_size(l.block) > cont){
            HashTableItem * x = (HashTableItem*)forward_list_get(l.block, cont);

                if(x == NULL)
                    break;

            if((h->comparar(item->cel, x->cel)) == 0){
                void * lixo = x->idx_heap;
                x->idx_heap = idx_heap;
                free(item);
                return lixo;
            }
            cont++;
        }
        forward_list_push_back(l.block, item);
        h->tabela[idx] = l;
        return NULL;
    }  
}

int hash_table_size(HashTable* h){
    return h->total_aloc;
}

void *hash_table_get(HashTable *h, void *cel){
    int idx = h->calculo(h, cel);

    BlockHash l = h->tabela[idx];

        if(forward_return_size(l.block) == 0)
            exit(printf("Error - item nao encontrado!\n"));

    int cont = 0;
    HashTableItem * n = (HashTableItem*)forward_list_get(l.block, cont);

    while(forward_return_size(l.block) >= cont){
        if((h->comparar(n->cel, cel)) == 0){
        }
    }
    if(n == NULL)
        exit(printf("Error: item nao foi encontrado! (hash_table_get)"));
    return NULL;
}   

void hash_table_destroy(HashTable *h){
    free(h->tabela);
    free(h);
}

NodeIterador *_node_iterador_construct(ForwardList* atual){
    NodeIterador * n = (NodeIterador*)malloc(sizeof(NodeIterador));
    n->list = atual;
    n->next = NULL;
    
    return n;
}

HashTableIterator * hash_table_iterator(HashTable* h){
    HashTableIterator * it = (HashTableIterator*)malloc(sizeof(HashTableIterator));
    it->current = NULL;
    it->size = 0;

    for(int i = 0; i < h->total_aloc; i++){
        ForwardList * consultar = h->tabela[i].block;

        if(forward_return_size(consultar) == 0){
            forward_list_destruct(consultar);
            continue;
        }
        
        if(it->current == NULL){
            NodeIterador * n = _node_iterador_construct(consultar);
            it->current = n;
            it->size++;
        }
        else{
            NodeIterador * new = _node_iterador_construct(consultar);
            NodeIterador * aux = it->current;

            while(aux->next != NULL){
                aux = aux->next;
            }
            aux->next = new;
            it->size++;
        }      
          
    }

    return it;
}

int hash_table_iterator_is_over(HashTableIterator* it){
    if(it->size == 0)
        return 1;
    else
        return 0;
}

void _node_iterador_destruct(NodeIterador * n){
    forward_list_destruct(n->list);
    free(n);
}

HashTableItem* hash_table_iterator_next(HashTableIterator * it){
    NodeIterador * n = it->current;
    ForwardList * list = n->list;
    HashTableItem * item = (HashTableItem*)forward_list_pop_front(list);    

   // if(forward_return_size(list) == 0){
        it->current = n->next;
        it->size--;
        _node_iterador_destruct(n);  
    //}
    return item;

}

void hash_table_iterator_destroy(HashTableIterator* it){
    free(it);
}

void * hash_return_cel(HashTableItem * x){
    return x->cel;
}

void hash_remove(HashTable * h, void * cel){
    int idx = h->calculo(h, cel);
    BlockHash l = h->tabela[idx];

        if(forward_return_size(l.block) == 0)
            exit(printf("Error - item nao encontrado!(HEAP)\n"));

    int cont = 0;
    HashTableItem * node = (HashTableItem*)forward_list_get(l.block, cont);
    while(node != NULL){
            if((h->comparar(node->cel, cel)) == 0){
                    forward_list_remove(l.block, node);
                    free(node->idx_heap);
                    //free(node->cel);
                    free(node);
                    break;
            }
        cont++;
        node = (HashTableItem*)(HashTableItem*)forward_list_get(l.block, cont);  
    }
    if(forward_size_over(l.block)==0)
        h->qtd_block--;
    h->tabela[idx] = l;
}

void *hash_table_find(HashTable * h, void * cel){
    int idx = h->calculo(h, cel);
    BlockHash l = h->tabela[idx];

        if(forward_return_size(l.block) == 0)
            return NULL;

    int cont = 0;

    while(forward_return_size(l.block) > cont){
        HashTableItem * n = (HashTableItem*)forward_list_get(l.block, cont);
        if(n == NULL)
            return NULL;
        if((h->comparar(n->cel, cel)) == 0){
                return n->idx_heap;
        }
        cont++;
    }
    return NULL;

}
