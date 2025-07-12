#pragma once
#include <vector>
#include <random>
#include "Individuo.h"

class GeneticAlgorithm {
private:
    int numGeracoes, tamPopulacao, numNovosIndividuos, tamIndividuo;
    float probMutacao;
    std::vector<std::vector<double>> matrizDeCaminhos;
    std::vector<Individuo> populacao;
    std::mt19937 gerador;

    Individuo melhorIndividuo(const std::vector<Individuo>& populacao);
    double calcularDistancia(const std::vector<int>& individuo) const;
    void gerarPopulacao();
    std::vector<Individuo> selecao();
    void mutacao(Individuo& individuo);
    Individuo cruzamentoOX(const Individuo& pai1, const Individuo& pai2);

public:
    GeneticAlgorithm(int numGeracoes, float probMutacao, int tamPopulacao, 
                     int numNovosIndividuos, const std::string& caminhoArquivo);
    void executarAlgoritmo();
};