#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

BinaryTree * binary_tree_construct(int cmp_fn(void *, void*), void(*key_destroy_fn)(void*), void(*val_destroy_fn)(void*))
{
    BinaryTree * bt = (BinaryTree*)malloc(sizeof(BinaryTree));
    bt->root = NULL;
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;
    return bt;
}

NodeTree * _node_tree_construct(void *key, void *value, NodeTree* right, NodeTree* left, NodeTree * parent){
    NodeTree * n = (NodeTree*)malloc(sizeof(NodeTree));
    n->key = key;
    n->value = value;
    n->right = right;
    n->left = left;
    n->parent = parent;
    return n;
}

NodeTree * _add_recursive(NodeTree* node, NodeTree* parent, void* key, void* value, BinaryTree * bt){
    if(node == NULL)
        return _node_tree_construct(key, value, NULL, NULL, parent);

    int cmp_result = bt->cmp_fn(key, node->key);

    if(cmp_result < 0)
        node->left = _add_recursive(node->left, node, key, value, bt);
    else if(cmp_result > 0)
        node->right = _add_recursive(node->right, node, key , value, bt);
    else{
        void * lixo = node->value;
        node->value = value;
        bt->val_destroy_fn(lixo);
        bt->key_destroy_fn(key);
    }
    
    return node;
}

void binary_tree_add(BinaryTree * bt, void* key, void * value){
    bt->root = _add_recursive(bt->root, NULL, key, value, bt);
}

void * _get_recursive(NodeTree* node, void* key, int (*cmp_fn)(void * key1, void * key2)){
    if (node == NULL) {
        return NULL;  // Nó não encontrado na árvore
    }

    int cmp_result = cmp_fn(key, node->key);
    
    if(cmp_result == 0)
        return node->value;  // Nó encontrado
    else if(cmp_result < 0)
        return _get_recursive(node->left, key, cmp_fn);  // Procurar no lado esquerdo
    else
        return _get_recursive(node->right, key, cmp_fn);  // Procurar no lado direito

}

void * binary_tree_get(BinaryTree* bt, void* key){
    return _get_recursive(bt->root, key, bt->cmp_fn);
}

void _transplante_arvore(BinaryTree * bt, NodeTree* u, NodeTree * v){
    if(u->parent == NULL){
        bt->root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }

    if(v != NULL)
        v->parent = u->parent;
}

NodeTree * _find_minimo(NodeTree * node){
    if(node->left == NULL){
        return node;
    }
    return _find_minimo(node->left);
}

NodeTree * _find_maximo(NodeTree * node){
    if(node->right == NULL){
        return node;
    }
    return _find_maximo(node->right);
}

void binary_tree_remove(BinaryTree * bt, void * key){
    NodeTree * node = bt->root;
    while(node != NULL)
    {
        int cmp_result = bt->cmp_fn(key, node->key);

        if(cmp_result == 0)
        {
            if(node->left == NULL)
            {
                _transplante_arvore(bt, node, node->right);
            }
            else if(node->right == NULL)
            {
                _transplante_arvore(bt, node, node->left);
            }
            else 
            {   
                NodeTree * y = _find_minimo(node->right);
                if(y->parent != node){
                    _transplante_arvore(bt, y, y->right);
                    y->right = node->right;
                    y->right->parent = y;
                }
                
                _transplante_arvore(bt, node, y);
                y->left = node->left; 
                y->left->parent = y;
            }

            bt->key_destroy_fn(node->key);
            bt->val_destroy_fn(node->value);
            free(node);
            break;
        }
        else if(cmp_result > 0){
            node = node->right;
        }
        else if(cmp_result < 0){
            node = node->left;
        }
    }
}

KeyValPair * binary_tree_max(BinaryTree* bt){
    NodeTree * node = bt->root;
    NodeTree * prev = NULL;
    while(node != NULL){    
        prev = node;
        node = node->right;
    }
    return key_val_pair_construct(prev->key, prev->value);   
}   

KeyValPair *key_val_pair_construct(void *key, void *val){
    KeyValPair * x =(KeyValPair*)malloc(sizeof(KeyValPair));
    x->key = key;
    x->value = val;
    return x;
}

KeyValPair * binary_tree_min(BinaryTree* bt){
    NodeTree * node = bt->root;
    NodeTree * prev = NULL;
    while(node != NULL){    
        prev = node;
        node = node->left;
    }
    return key_val_pair_construct(prev->key, prev->value);    
}

void key_val_pair_destroy(KeyValPair* p){
    free(p);
}

KeyValPair * binary_tree_pop_max(BinaryTree* bt){
    KeyValPair * max = binary_tree_max(bt);

    NodeTree * node = bt->root;
    while(node != NULL){    
        if(bt->cmp_fn(max->key, node->key) == 0)
            break;
        node = node->right;
    }
    
    _transplante_arvore(bt, node, node->left);
    free(node);
    return max;
}

KeyValPair * binary_tree_pop_min(BinaryTree* bt){
    KeyValPair * min = binary_tree_min(bt);

    NodeTree * node = bt->root;

        while(node != NULL){    
            if(bt->cmp_fn(min->key, node->key) == 0)
                break;
            node = node->left;
        }
        
        _transplante_arvore(bt, node, node->right);
        free(node);
    return min;
}

void _destroy_recursive(NodeTree* node, BinaryTree * bt) {
    if (node == NULL)
        return;

    _destroy_recursive(node->left, bt);
    _destroy_recursive(node->right, bt);

    bt->key_destroy_fn(node->key);
    bt->val_destroy_fn(node->value);
    free(node);
}

void binary_tree_destroy(BinaryTree* bt) {
    if (bt != NULL) {
        _destroy_recursive(bt->root, bt);
    }
    free(bt);
}


int binary_tree_empty(BinaryTree *bt){
    if(bt->root == NULL)
        return 1;
    else
        return 0;
}

Vector *binary_tree_inorder_traversal(BinaryTree * bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    Vector * pilha = vector_construct();
    
    while(1){
        while(atual != NULL){
            vector_push_back(pilha, atual);
            atual = atual->left;
        }
        if(vector_size(pilha) == 0)
            break;
        else{
            NodeTree * new = vector_pop_back(pilha);
            vector_push_back(saida, new);
            atual = new->right;
        }
    }
    vector_destroy(pilha);
    return saida;
}

Vector *binary_tree_preorder_traversal(BinaryTree *bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    Vector * pilha = vector_construct();
    vector_push_back(pilha, atual);
    while(vector_size(pilha) != 0){
        NodeTree * item = vector_pop_back(pilha);
        vector_push_back(saida, item);
        if(item->right != NULL)
            vector_push_back(pilha, item->right);
        if(item->left != NULL)
            vector_push_back(pilha, item->left);
    }
    vector_destroy(pilha);
    return saida;
}

Vector *binary_tree_postorder_traversal(BinaryTree *bt){
    Vector * saida = vector_construct();
    Vector * pilha1 = vector_construct();
    Vector * pilha2 = vector_construct();

    NodeTree * atual = bt->root;
    vector_push_back(pilha1, atual);

    while(vector_size(pilha1) != 0){
        atual = vector_pop_back(pilha1);
        vector_push_back(pilha2, atual);

        if(atual->left != NULL)
            vector_push_back(pilha1, atual->left);
        if(atual->right != NULL)
            vector_push_back(pilha1, atual->right);
        
    }
    
    while(vector_size(pilha2) != 0){
        atual = vector_pop_back(pilha2);
        vector_push_back(saida, atual);
    }

    vector_destroy(pilha1);
    vector_destroy(pilha2);
    
    return saida;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    Vector * pilha = vector_construct();
    vector_push_back(pilha, atual);

    while(vector_size(pilha) != 0){
        NodeTree * item = vector_pop_front(pilha);

        if(item != NULL){
            vector_push_back(saida, item);
            if(item->left != NULL)
                vector_push_back(pilha, item->left);
            if(item->right != NULL)
                vector_push_back(pilha, item->right);
        }
    }
    vector_destroy(pilha);
    return saida;
}

Vector * _recursive_inorder_traversal(NodeTree* node, Vector *saida){
    if(node == NULL)
        return saida;

    _recursive_inorder_traversal(node->left, saida);
    vector_push_back(saida, node);
    _recursive_inorder_traversal(node->right, saida);

    return saida;
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    return _recursive_inorder_traversal(atual, saida);
}

Vector * _recursive_preorder_traversal(NodeTree * node, Vector * saida){
    if(node == NULL)
        return saida;

    vector_push_back(saida, node);
    _recursive_preorder_traversal(node->left, saida);
    _recursive_preorder_traversal(node->right, saida);

    return saida;
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    return _recursive_preorder_traversal(atual, saida);
}


Vector * _recursive_postorder_traversal(NodeTree * node, Vector * saida){
    if(node == NULL)
        return saida;

    _recursive_postorder_traversal(node->left, saida);
    _recursive_postorder_traversal(node->right, saida);
    vector_push_back(saida, node);

    return saida;
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt){
    Vector * saida = vector_construct();
    NodeTree * atual = bt->root;
    return _recursive_postorder_traversal(atual, saida);
}