#include "GeneticAlgorithm.h"
#include <chrono>
#include <iostream>
#include <omp.h>

int main() {
    omp_set_num_threads(4);
    GeneticAlgorithm ga(1, 0.2f, 60, 60, "entradas/ATT48.txt");

    auto inicio = std::chrono::high_resolution_clock::now();
    ga.executarAlgoritmo();
    auto fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracao = fim - inicio;
    std::cout << "\nTempo de execucao: " << duracao.count() << " segundos\n";

    return 0;
}
