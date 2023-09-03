#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "busca.h"

float calcular_distancia(Celula ponto1, Celula ponto2){
    int x = ponto1.x - ponto2.x;
    int y = ponto1.y - ponto2.y;
    return sqrt((pow(x, 2) + pow(y, 2)));
}

void deque_salvar_caminho(Deque* d, Celula* filho, Celula* pai){
    DirecaoCelula * percurso = (DirecaoCelula*)malloc(sizeof(DirecaoCelula));
    percurso->filho = filho;
    percurso->pai = pai;
    deque_push_front(d, percurso);
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
        (labirinto_obter(l, new->y, new->x) != EXPANDIDO) && 
        (labirinto_obter(l, new->y, new->x) != INICIO) && 
        (labirinto_obter(l, new->y, new->x) != FRONTEIRA)){         
                return 1;      
        }
    }
    return 0;
    
}   

void emqueue_celulas_ao_redor(Celula *a, Queue *q, Labirinto* lab, Deque* deque){
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

void stackando_celulas_ao_redor(Celula *a, Stack *s, Labirinto* lab, Deque* deque){
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



