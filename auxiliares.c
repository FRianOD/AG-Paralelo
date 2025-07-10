#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "auxiliares.h"


int** arqEuc(const char* nomeDoArquivo, int* N_out) {
    FILE* arquivo = fopen(nomeDoArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int N;
    fscanf(arquivo, "%d", &N);
    *N_out = N;

    Ponto* pontos = (Ponto*)malloc(N * sizeof(Ponto));
    if (!pontos) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        int idx, x, y;
        fscanf(arquivo, "%d %d %d", &idx, &x, &y);
        pontos[i].x = x;
        pontos[i].y = y;
    }

    fclose(arquivo);

    int** matriz = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        matriz[i] = (int*)calloc(N, sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int dx = pontos[j].x - pontos[i].x;
            int dy = pontos[j].y - pontos[i].y;
            int distancia = (int)round(sqrt(dx * dx + dy * dy));
            matriz[i][j] = distancia;
            matriz[j][i] = distancia;
        }
    }

    free(pontos);
    return matriz;
}

Resultado construtivo(int** matriz, int N){
    int inicio = 0;
    int custoTotal = 0;
    int pos = 0;
    int atual = inicio;


    int* visitados = malloc(N * sizeof(int));
    int* caminho = malloc((N + 1) * sizeof(int));

    if (!caminho || !visitados) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }

    visitados[inicio] = 1;
    caminho[pos++] = inicio;

    for (int i = 0; i < N - 1; i++) {
        int menor_peso = INT_MAX;
        int proximo = -1;

        for (int vizinho = 0; vizinho < N; vizinho++) {
            if (vizinho == atual || visitados[vizinho])
                continue;

            int peso = matriz[atual][vizinho];
            if (peso < menor_peso) {
                menor_peso = peso;
                proximo = vizinho;
            }
        }

        if (proximo == -1) {
            fprintf(stderr, "Erro: Grafo desconectado ou ciclo impossível\n");
            exit(EXIT_FAILURE);
        }

        caminho[pos++] = proximo;
        visitados[proximo] = 1;
        custoTotal += menor_peso;
        atual = proximo;
    }

    custoTotal += matriz[atual][inicio];
    caminho[pos++] = inicio;

    Resultado resultado;
    resultado.caminho = caminho;
    resultado.custoTotal = custoTotal;

    free(visitados);

    return resultado;

}
