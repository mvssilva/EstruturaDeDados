#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "matriz.h"

int INICIALIZADOR = 5;

Matriz * matriz_construir(){
    Matriz * m = (Matriz*)malloc(sizeof(Matriz));
    m->size_c = 0;
    m->size_l = 0;

    m->alocado_c = INICIALIZADOR;
    m->alocado_l = INICIALIZADOR;

    m->list_coluna = (Coluna*)calloc(INICIALIZADOR, sizeof(Coluna));
    m->list_linha = (Linha*)calloc(INICIALIZADOR, sizeof(Linha));
    return m;
}

void _add_nova_coluna(int qtd_inicial, Coluna *c, int qtd_final){
    for(int i = qtd_inicial; i < qtd_final; i++){
        c[i].head = NULL;
        c[i].coluna = i;
    }
}

void _add_nova_linha(int qtd_inicial, Linha * l, int qtd_final){
    for(int i = qtd_inicial; i < qtd_final; i++){    
        l[i].head = NULL;
        l[i].linha = i;
    }
}

void _adicionar_list_colunas(Coluna * c, Node * new){
    int coluna = new->coluna;
    int linha = new->linha;
    Node * it = c[coluna - 1].head;
    Node * prev = NULL;
    int aux = 0;

    if(it == NULL){
        //PRIMEIRO ITEM DA LISTA
        new->next_na_coluna = NULL;
        c[coluna - 1].head = new;
    }
    else{
            //LISTA COM 1 OU MAIS ITENS
            while(it != NULL){

                if(it->linha < linha){
                //ITERANDO SOBRE OS ELEMENTOS ATÉ ACHAR A POSICAO 
                    prev = it;
                    it = it->next_na_coluna;
                }else{
                    if(prev == NULL){
                        //CASO EM QUE ADICIONA O ITEM COMO PRIMEIRO
                        c[coluna - 1].head = new;
                        new->next_na_coluna = it;
                    }else{
                        //CASOS EM QUE ADICIONA O NO MEIO DA LISTA
                        prev->next_na_coluna = new;
                        new->next_na_coluna = it;
                    }
                    aux = 1;
                    break;
                }
            }
            if(aux == 0){
                //CASO EM QUE ADICIONA NA ULTIMA POSICAO DA LISTA
                prev->next_na_coluna = new;
                new->next_na_coluna = NULL;
            }
    }

}

void _adicionar_list_linhas(Linha * l, Node * new){
    int coluna = new->coluna;
    int linha = new->linha;
    Node * it = l[linha - 1].head;
    Node * prev = NULL;
    int aux  = 0;

    if(it == NULL){
        //PRIMEIRO ITEM DA LISTA
        l[linha - 1].head = new;
        new->next_na_linha = NULL;

    }else{
            //LISTA COM 1 OU MAIS ITENS
            while(it != NULL){
                
                if(it->coluna < coluna){
                    //ITERANDO SOBRE OS ELEMENTOS ATÉ ACHAR A POSICAO 
                    prev = it;
                    it = it->next_na_linha;
                }
                else{
                    if(prev == NULL){
                        //CASO EM QUE ADICIONA O ITEM COMO PRIMEIRO (ATUALIZA A CABECA DA LISTA)
                        l[linha - 1].head = new;
                        new->next_na_linha = it;
                    }else{
                        //CASOS EM QUE ADICIONA O NO MEIO DA LISTA
                        prev->next_na_linha = new;
                        new->next_na_linha = it;
                    }
                aux = 1;
                break;
                }
            }
            if(aux == 0){
                //CASO EM QUE ADICIONA NA ULTIMA POSICAO DA LISTA
                prev->next_na_linha = new;
                new->next_na_linha = NULL;
            }
    }
}

void _incluir_novo_node(Node * new, Matriz *m){

    _adicionar_list_colunas(m->list_coluna, new);
    _adicionar_list_linhas(m->list_linha, new);
}

void _matriz_reajustando_dimensoes(Node * new, Matriz *m){
    int qtdlinhas = m->size_l;
    int qtdcolunas = m->size_c;
    
    Coluna * c = m->list_coluna;
    if(new->coluna > qtdcolunas){
            if(new->coluna >= m->alocado_c){
                m->alocado_c = new->coluna + 1;
                m->list_coluna = (Coluna*)realloc(c, m->alocado_c * sizeof(Coluna));
                c = m->list_coluna;
            }
        _add_nova_coluna(qtdcolunas, c, new->coluna);
        m->size_c = new->coluna;
    }

    Linha * l = m->list_linha;
    if(new->linha > qtdlinhas){

            if(new->linha >= m->alocado_l){
                m->alocado_l = new->linha + 1;
                m->list_linha = (Linha*)realloc(l, m->alocado_l * sizeof(Linha));
                l = m->list_linha;
            }
        _add_nova_linha(qtdlinhas, l, new->linha);
        m->size_l = new->linha;
    }

}

void matriz_inserir_valores(Node * new, Matriz *m){
    int qtdlinhas = m->size_l;
    int qtdcolunas = m->size_c;
    if(new->coluna > qtdcolunas || new->linha > qtdlinhas){
        _matriz_reajustando_dimensoes(new, m);
    }
    if(new->valor == 0){
        node_destruir(new);
    }else{
        _incluir_novo_node(new, m);
    }
}

void matriz_imprimir_esparco(Matriz * m){ 
    int total_linhas = m->size_l;

    for(int i = 0; i < total_linhas; i++){
        Node * n = m->list_linha[i].head;
            if(n == NULL){
                continue;
            }else{
                while(n != NULL){
                    printf("-Posicao %d x %d | Valor %.2f\n",n->linha, n->coluna, n->valor);
                    n = n->next_na_linha;
                }
            }   
    }
}


float * matriz_mostrar_valor(Matriz * m , int linha, int coluna){
    Linha *l = m->list_linha;
    Node * it = l[linha].head;

    while(it != NULL){
        if(it->coluna == coluna + 1){
            return &it->valor;
        }

        it = it->next_na_coluna;
    }
    return NULL;
}

void imprimir_matriz_denso(Matriz * m){
    int total_colunas = m->size_c;
    int total_linhas = m->size_l;
    Linha * l = m->list_linha;
    printf("- IMPRIMINDO MATRIZ - [%dx%d]\n", total_linhas, total_colunas);
    for(int i = 0; i < total_linhas; i++){
        Node * it = l[i].head;

        for(int j = 0; j < total_colunas; j++){
    
            if(it == NULL){
                printf("[    0.00]");
            }else if(it->coluna == j+1){
                printf("[%8.2f]", it->valor);
                it = it->next_na_linha;

            }else{
                printf("[    0.00]");
            }
    
        }
        printf("\n");

    }
}

void _organizando_ponteiros_anteriores(Node * it, Node * linha){
    while(it != NULL){
        if(it->coluna == linha->coluna){
            it->next_na_coluna = linha;
            it = it->next_na_linha;
            linha = linha->next_na_linha;
        }else if(it->coluna > linha->coluna){
            linha = linha->next_na_linha;
        }else if(it->coluna < linha->coluna){
            it = it->next_na_linha;
        }
    }
}

void _trocar_valores(float * a, float * b){
    float aux = *a;
    *a = *b;
    *b = aux;
}

void _alterando_list_coluna(int indice_coluna, Linha *l, Coluna *c, Node * lixo){
    Node * it = c[indice_coluna-1].head;
    Node * prev = NULL;
    while(it != NULL){
        if(it->linha == lixo->linha){
            if(prev == NULL){
                c[indice_coluna-1].head = it->next_na_coluna;
            }
            else{
                prev->next_na_coluna = it->next_na_coluna;
            }
            break;
        }
        prev = it;
        it = it->next_na_coluna;
    }
}

void _alterando_list_linha(int indice_linha, Linha * l, Coluna * c, Node *lixo){
    Node * it = l[indice_linha-1].head;
    Node * prev = NULL;
    while(it != NULL){
        if(it->coluna == lixo->coluna){
            if(prev == NULL){
                l[indice_linha-1].head = it->next_na_linha;
            }
            else{
                prev->next_na_linha = it->next_na_linha;
            }
            break;
        }
        prev = it;
        it = it->next_na_linha;
    }
}

void _desencadear_valor_trocado(Node * lixo, Matriz* m){
    int indice_coluna = lixo->coluna;
    int indice_linha = lixo->linha;

    //ACESSANDO AS LISTAS ANTERIORES 
    Linha * l = m->list_linha;
    Coluna * c = m->list_coluna;
    _alterando_list_linha(indice_coluna, l, c, lixo);
    _alterando_list_coluna(indice_linha, l, c, lixo);

}

void _modificar_cadastros_linha(Node * head, int nova_linha){
    Node * n = head;
    while(n != NULL){
        n->linha = nova_linha;
        n = n->next_na_linha;
    }
}

void _modificar_cadastros_coluna(Node * head, int nova_coluna){
    Node * n = head;
    while(n != NULL){
        n->coluna = nova_coluna;
        n = n->next_na_coluna;
    }
}

void matriz_swap_entre_linhas(Matriz *m){
    int a, b;

    printf("Trocar essa = ");
    scanf("%d", &a);
    printf("Por essa = ");
    scanf("%d", &b);

    int total_colunas = m->size_c;
    if(a > m->size_l || b > m->size_l){
        printf("ERROR - Linha informada não existe!");
        exit(0);
    }
    if(a != b){
        Node * iteradorA = m->list_linha[a-1].head;
        Node * iteradorB = m->list_linha[b-1].head;

        if(iteradorA == NULL || iteradorB == NULL){
            m->list_linha[a-1].head = iteradorB;
            m->list_linha[b-1].head = iteradorA;
            _modificar_cadastros_linha(iteradorA, b-1);
            _modificar_cadastros_linha(iteradorB, a-1);
        }
        else{
            for(int i = 0; i < total_colunas; i++){
                if(iteradorA->coluna == iteradorB->coluna){
                    //CASO EM QUE A MATRIZ É COMPLETA E DENSA
                    _trocar_valores(&iteradorA->valor, &iteradorB->valor);
                    iteradorA = iteradorA->next_na_linha;
                    iteradorB = iteradorB->next_na_linha;
                }
            
            }
        }
    }
}

void matriz_swap_entre_colunas(Matriz *m){
    int a, b;

    printf("Trocar essa = ");
    scanf("%d", &a);
    printf("Por essa = ");
    scanf("%d", &b);

    int total_linhas = m->size_l;
    if(a > m->size_c || b > m->size_c){
        printf("ERROR - Linha informada não existe!");
        exit(0);
    }
    if(a != b){
        Node * iteradorA = m->list_coluna[a-1].head;
        Node * iteradorB = m->list_coluna[b-1].head;

        if(iteradorA == NULL || iteradorB == NULL){
            m->list_coluna[a-1].head = iteradorB;
            m->list_coluna[b-1].head = iteradorA;
            _modificar_cadastros_coluna(iteradorA, b-1);
            _modificar_cadastros_coluna(iteradorB, a-1);
        }
        else{
            for(int i = 0; i < total_linhas; i++){
                if(iteradorA->linha == iteradorB->linha){
                    _trocar_valores(&iteradorA->valor, &iteradorB->valor);
                    iteradorA = iteradorA->next_na_coluna;
                    iteradorB = iteradorB->next_na_coluna;
                }
            
            }
        }
    }
}

void matriz_destruir(Matriz *m){
    int total_colunas = m->size_c;
    int total_linhas = m->size_l;
    Linha * l = m->list_linha;
    for(int i = 0; i < total_linhas; i++){
        Node * it = l[i].head;
        for(int j = 0; j < total_colunas; j++){
            
            if(it == NULL){
                break;
            }
            else if(it->coluna == j+1){
                Node * aux = it->next_na_linha; 
                node_destruir(it); //PARA DESALOCAR TODAS OS NODES
                it = aux;
            }
        }
    }
    free(m->list_coluna);
    free(m->list_linha);
    free(m);
}