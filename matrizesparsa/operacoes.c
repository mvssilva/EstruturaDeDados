#include <stdio.h>
#include <stdlib.h>
#include "operacoes.h"

void multiplicacao_por_escalar(Matriz* m){
    int total_colunas = m->size_c;
    int total_linhas = m->size_l;
    Linha * l = m->list_linha;
    int k;
    printf("- MULTIPLICAR A MATRIZ POR = ");
    scanf("%d", &k);
    for(int i = 0; i < total_linhas; i++){
        Node * it = l[i].head;

        for(int j = 0; j < total_colunas; j++){
            if(it == NULL){
                break;
            }
            else{
                it->valor *= k; 
                it = it->next_na_linha;           
            }
        }

    }
}

void soma_matrizes(Matriz * m1, Matriz *m2, Matriz * resultado){
    if(m1->size_c != m2->size_c || m1->size_l != m2->size_l){
        printf("ERROR - Soma de matrizes com dimensões diferentes não pode ser realizada!");
        exit(0);
    }
    
    Linha * l1 = m1->list_linha;
    Linha * l2 = m2->list_linha;
    int i = 0;
    int j = 0;

    
    while(i < m1->size_l){   
        Node * it1 = l1[i].head;
        Node * it2 = l2[j].head;

        while(it1 != NULL || it2 != NULL){
            float soma;
            int linha;
            int coluna;

            if(it1 == NULL){
                soma = it2->valor;
                linha = it2->linha;
                coluna = it2->coluna;
            }
            else if(it2 == NULL){
                soma = it1->valor;
                linha = it1->linha;
                coluna = it1->coluna;
            }else{
                soma = it1->valor + it2->valor;
                linha = it1->linha;
                coluna = it1->coluna;
            }
            Node * new = node_construir(linha, coluna, soma);
            matriz_inserir_valores(new, resultado);

            it2 = it2->next_na_linha;
            it1 = it1->next_na_linha;
        }
        i++;
        j++;

    }
}

float _multiplicando_linha_x_coluna(Node * linha, Node * coluna, int limite){
    if(linha == NULL || coluna == NULL){
        return 0;
    }

    float resultado = 0;
    float valorL, valorC;
    int indiceC, indiceL;

    for(int i = 0; i < limite; i++){
        
        //SEGUINDO A LINHA E SALVANDO O PRIMEIRO INDICE
        if(linha == NULL){
            indiceL = i;
            valorL = 0;
        }else{
            indiceL = linha->coluna - 1;
            valorL = linha->valor;
        }
        
        //SEGUINDO A COLUNA E SALVANDO O PRIMEIRO INDICE
        if(coluna == NULL){
            indiceC = i;
            valorC = 0;
        }else{
            indiceC = coluna->linha - 1;
            valorC = coluna->valor;
        }

        //COMPARANDO COM AS DIMENSÕES DAS MATRIZES (QUE NOMIEI DE LIMITE)
        //ESTÁ FAZENDO A OPERACAO QUANDO OS VALORES ESTÃO NAS POSICOES CORRETAS 
        if(indiceL == i && indiceC == i){
            resultado += valorC * valorL;
            linha = linha->next_na_linha;
            coluna = coluna->next_na_coluna;
        }
        else if (indiceC != i && indiceC != indiceL){
            //entra nesse quando o valor na coluna não é cadastrado == 0
            resultado += 0 * valorL;//só para visualizacao
            linha = linha->next_na_linha;
        }
        else if(indiceL != i && indiceL != indiceC){
            //entra nesse quando o valor na linha não é cadastrado == 0
            resultado += valorC * 0;//só para visualizacao
            coluna = coluna->next_na_coluna;
        }

        valorC = 0;
        valorL = 0;

    }

    return resultado;
}

void multiplicacao_matriz_x_matriz(Matriz * a, Matriz * b, Matriz * resultado){
    if(a->size_c != b->size_l){
        printf("ERROR - Multiplicacao - Matriz X Matriz - não pode ser realizada com essas dimensões das matrizes de entrada!");
        exit(0);
    }

    int total_linhas = a->size_l;
    int total_colunas = b->size_c;
    
    float new_valor = 0;

    for(int i = 0; i < total_linhas; i++){
        for(int j = 0; j < total_colunas; j++){
            //if(a->list_linha[i].head == NULL ||  b->list_coluna[j].head == NULL){
            new_valor = _multiplicando_linha_x_coluna(a->list_linha[i].head, b->list_coluna[j].head, a->size_c);
            if(new_valor == 0)
                continue;
            Node * new = node_construir(i+1, j+1, new_valor);
            matriz_inserir_valores(new, resultado);
        }
    }

}

float _multiplicando_ponto_ponto(float a, float b){
    return a * b;
}

void multiplicacao_matriz_ponto_a_ponto(Matriz * a, Matriz * b, Matriz * resultado){
    if(a->size_c != b->size_c || a->size_l != b->size_l){
        printf("ERROR - Multiplicacao -Matriz Ponto a Ponto- não pode ser realizada com matrizes de dimensoes diferentes!");
        exit(0);
    }

    int total_linhas = a->size_l;
    int total_colunas = b->size_c;
    
    float new_valor = 0;

    for(int i = 0; i < total_linhas; i++){
        Node * linhaA = a->list_linha[i].head;
        Node * linhaB = b->list_linha[i].head;

            if(linhaA == NULL || linhaB == NULL)
                continue;

        for(int j = 0; j < total_colunas; j++){

                if(linhaA == NULL || linhaB == NULL)
                    break;

            float valorA = linhaA->valor;
            int colunaA = linhaA->coluna - 1;
            float valorB = linhaB->valor;
            int colunaB = linhaB->coluna - 1;  

            if(colunaA == colunaB){
                new_valor = _multiplicando_ponto_ponto(valorA, valorB);
                Node * new = node_construir(i+1, j+1, new_valor);
                matriz_inserir_valores(new, resultado);
                linhaA = linhaA->next_na_linha;
                linhaB = linhaB->next_na_linha;
            }
            if(colunaA == j && colunaB != j){
                linhaA = linhaA->next_na_linha;
            }
            else if(colunaB == j && colunaA != j){
                linhaB = linhaB->next_na_linha;
            }
            
        }
    
    }
}

void recortar_matriz(Matriz* slice, Matriz* m){
    int linha1, coluna1, linha2, coluna2;
        printf("--- Recortar Matriz ---\nInforme a posicao inicial (x,y) = ");
        scanf("%d %d", &linha1, &coluna1);
        if(linha1 > m->size_c || coluna1 > m->size_l){
            printf("Ponto invalido!\n");
            return recortar_matriz(slice, m);
        }

        printf("Informe a posicao final (x,y) = ");
        scanf("%d %d", &linha2, &coluna2);
        if(linha2-1 > m->size_c || coluna2-1 > m->size_l){
            printf("Ponto invalido!\n");
            return recortar_matriz(slice, m);
        }

    //int tam_colunas = (linha2 - linha1) -1;
    int tam_linhas = (linha2 - linha1) - 1;

    Linha * l = m->list_linha;
    int i = 0, j = 0;
    Node * iterador = l[linha1].head;
    while(j < tam_linhas){
        if(iterador == NULL){
            linha1++;
            iterador = l[linha1].head;
            i = 0;
            j++;
        }
        else if(iterador->coluna >= coluna1+1 && iterador->coluna < coluna2){
            i = iterador->coluna - (coluna1+1);
            Node * new = node_construir(j+1, i+1, iterador->valor);
            matriz_inserir_valores(new, slice);
            iterador= iterador->next_na_linha;

        }else if(iterador->coluna <= coluna1){
            iterador = iterador->next_na_linha;
        }else{
            linha1++;
            iterador = l[linha1].head;
            i = 0;
            j++;
        }
        
    }

}