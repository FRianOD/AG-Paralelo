#ifndef AUXILIARES
#define AUXILIARES

typedef struct {
    int x;
    int y;
} Ponto;

typedef struct {
    int* caminho;
    int custoTotal;
} Resultado;

int** arqEuc(const char* nomeDoArquivo, int* N_out);

Resultado construtivo(int** matriz, int N);

#endif
