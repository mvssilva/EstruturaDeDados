#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "busca.h"

double calcular_distancia(Celula ponto1, Celula ponto2){
    int x = ponto1.x - ponto2.x;
    int y = ponto1.y - ponto2.y;
    return sqrt((pow(x, 2) + pow(y, 2)));
}

void deque_salvar_caminho(Deque* d, Celula* filho, Celula* pai){
    DirecaoCelula * percurso = (DirecaoCelula*)malloc(sizeof(DirecaoCelula));
    percurso->filho = filho;
    percurso->pai = pai;
    deque_push_back(d, percurso);
}

void *retornar_pai(void* prev){
    DirecaoCelula* temp = (DirecaoCelula*)prev;
    
    if(temp == NULL)
        return NULL;
    
    Celula * pai = (Celula*)temp->pai;

        if(pai == NULL)
            return NULL;
    
    return pai;
}

void *retornar_filho(void* prev){
    DirecaoCelula* temp = (DirecaoCelula*)prev;
    
    if(temp == NULL)
        return NULL;
    
    Celula * filho = (Celula*)temp->filho;

        if(filho == NULL)
            return NULL;
    
    return filho;
}

void eliminar_todo_o_deque(Deque *d){
    while(deque_size(d) > 0){
            void * prev = deque_pop_front(d);
                if(prev == NULL)
                    continue;
            Celula * filho = retornar_filho(prev);
                if(filho != NULL)
                    free(filho);
            free(prev);
        }
}

int verificando_celulas(Celula * new, Labirinto *l){
    if((new->x >= 0 && new->x < labirinto_n_colunas(l)) && (new->y >= 0 && new->y < labirinto_n_linhas(l))){
        if ((labirinto_obter(l, new->y, new->x) != OCUPADO) && 
        (labirinto_obter(l, new->y, new->x) != FRONTEIRA) &&
        (labirinto_obter(l, new->y, new->x) != EXPANDIDO)){         
                return 1;      
        }
    }
    return 0;
    
}   

void enfileirar_celulas_adjacentes(Celula *a, Queue *q, Labirinto* lab, Deque* deque){
    int coluna = a->x;
    int linha = a->y;
    int i, j;
    // Definir a ordem desejada para as células vizinhas
    int ordem[8][2] = {
        {-1, 0}, // Cima
        {-1, 1}, // Canto superior direito
        {0, 1},  // Direita
        {1, 1},  // Canto inferior direito
        {1, 0},  // Baixo
        {1, -1}, // Canto inferior esquerdo
        {0, -1}, // Esquerda
        {-1, -1} // Canto superior esquerdo
    };

    for (int k = 0; k < 8; k++)
    {
        i = ordem[k][0];
        j = ordem[k][1];

        Celula *new = (Celula *)malloc(sizeof(Celula));
        new->x = coluna + j;
        new->y = linha + i;

        int aux = verificando_celulas(new, lab);
        if(aux){
            //printf("%d x %d\n", new->x, new->y);
            queue_push(q, new);
            deque_salvar_caminho(deque, new, a);
            labirinto_atribuir(lab, new->y, new->x, FRONTEIRA);
        }else{
            free(new);
        }
    }
}

void empilhando_celulas_adjacentes(Celula *a, Stack *s, Labirinto* lab, Deque* deque){
    int coluna = a->x;
    int linha = a->y;
    int i, j;
    // Definir a ordem desejada para as células vizinhas
    int ordem[8][2] = {
        {-1, 0}, // Cima
        {-1, 1}, // Canto superior direito
        {0, 1},  // Direita
        {1, 1},  // Canto inferior direito
        {1, 0},  // Baixo
        {1, -1}, // Canto inferior esquerdo
        {0, -1}, // Esquerda
        {-1, -1} // Canto superior esquerdo
    };

    for (int k = 0; k < 8; k++)
    {
        i = ordem[k][0];
        j = ordem[k][1];

        Celula *new = (Celula *)malloc(sizeof(Celula));
        new->x = coluna + j;
        new->y = linha + i;

        int aux = verificando_celulas(new, lab);
        if(aux){
            //printf("%d x %d\n", new->x, new->y);
            stack_push(s, new);
            deque_salvar_caminho(deque, new, a);
            labirinto_atribuir(lab, new->y, new->x, FRONTEIRA);
        }else{
            free(new);
        }
    }
}

void heap_celulas_adjacentes(Celula * a, Heap * h, Labirinto* lab, Deque* deque, Celula fim){
    int coluna = a->x;
    int linha = a->y;
    int i, j;
    // Definir a ordem desejada para as células vizinhas
    int ordem[8][2] = {
        {-1, 0}, // Cima
        {-1, 1}, // Canto superior direito
        {0, 1},  // Direita
        {1, 1},  // Canto inferior direito
        {1, 0},  // Baixo
        {1, -1}, // Canto inferior esquerdo
        {0, -1}, // Esquerda
        {-1, -1} // Canto superior esquerdo
    };

    for (int k = 0; k < 8; k++)
    {
        i = ordem[k][0];
        j = ordem[k][1];

        Celula *new = celula_inicializacao(coluna + j, linha + i);
        int aux = verificando_celulas(new, lab);
        if(aux){
            labirinto_atribuir(lab, new->y, new->x, FRONTEIRA);
            new->dist_g = a->dist_g + calcular_distancia(*a, *new);
            new->dist_h = calcular_distancia(*new, fim); 
            double * prio = (double*)malloc(sizeof(double));
            *prio = new->dist_g + new->dist_h;
            void * prev = heap_push(h, new, prio);
            
            if(prev){
                free(new);
            }
            else{
                deque_salvar_caminho(deque, new, a);
            }
            
        }else{
            free(new);
        }
    }
}

double calcular_custo_provisorio(Deque * d, Celula fim, Celula inicio){
    double custo;
    int i = 0;
    Celula * atual = (Celula * )malloc(sizeof(Celula));
    atual->x = fim.x;
    atual->y = fim.y;
    while(deque_size(d) > i){
        void * prev = deque_get(d, i - 1);
        Celula * pai = retornar_pai(prev);
        Celula * filho = retornar_filho(prev);
        
        if(prev){
            if(filho->x == inicio.x && filho->y == inicio.y){
                break;
            }
            else if(filho->x == atual->x && filho->y == atual->y){
                atual = (Celula*)pai;  
                custo += calcular_distancia(*filho, *pai);
            }
        }
        i++;
    }

    return custo;
}


void eliminar_hash(HashTable* h){
    HashTableIterator *it = hash_table_iterator(h);

    while (!hash_table_iterator_is_over(it))
    {   
        HashTableItem *item = hash_table_iterator_next(it);
        //Celula *cel = (Celula *)item->cel;
        //free(cel);
        int * idx = item->idx_heap;
        free(idx);
        free(item);
    }
    hash_table_iterator_destroy(it);   
    hash_table_destroy(h);
}

Celula *celula_inicializacao(int x, int y){
    Celula * c = (Celula*)malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    c->dist_g = 0;
    c->dist_h = 0;
    return c;
}