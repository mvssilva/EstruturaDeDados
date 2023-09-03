#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "matriz.h"
#include "operacoes.h"

int main(){
    
    Matriz * m1 = matriz_construir();

    Node * new;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            new = node_construir(i+1, j+1, j+i);
            matriz_inserir_valores(new, m1);
        }
    }

    imprimir_matriz_denso(m1);
    Matriz * resultado = matriz_construir();
    recortar_matriz(resultado, m1);
    imprimir_matriz_denso(resultado);
    matriz_destruir(m1);
    matriz_destruir(resultado);

    return 0;
}
