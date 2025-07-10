#include <stdio.h>
#include <stdlib.h>
#include "auxiliares.h"

int main() {
    int N;
    int** matriz = arqEuc("copiaEuc.txt", &N);

    FILE* saida = fopen("matriz_saida.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(saida, "%d ", matriz[i][j]);
        }
        fprintf(saida, "\n");
        
    }
    Resultado caminho = construtivo(matriz, N);
    
    fclose(saida);
    for (int i = 0; i <= N; i++) {
        printf("%d ", caminho.caminho[i]);
    }
    printf("\n");
    printf("%d ", caminho.custoTotal);
    printf("\n");

    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(caminho.caminho);

    return 0;
}
