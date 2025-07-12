#include "GeneticAlgorithm.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include <numeric>
#include <omp.h>

GeneticAlgorithm::GeneticAlgorithm(int nGen, float pMut, int tPop, int nInd, const std::string& caminho) 
    : numGeracoes(nGen), probMutacao(pMut), tamPopulacao(tPop), numNovosIndividuos(nInd) {

    matrizDeCaminhos = lerArquivo(caminho);
    tamIndividuo = matrizDeCaminhos.empty() ? 0 : matrizDeCaminhos.size();
    gerador.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

Individuo GeneticAlgorithm::melhorIndividuo(const std::vector<Individuo>& pop) {
    return *std::min_element(pop.begin(), pop.end(), [](const Individuo& a, const Individuo& b) {
        return a.fitness < b.fitness;
    });
}

double GeneticAlgorithm::calcularDistancia(const std::vector<int>& individuo) const {
    double soma = 0;
    for (size_t i = 0; i < individuo.size(); ++i) {
        int atual = individuo[i] - 1;
        int prox = individuo[(i + 1) % individuo.size()] - 1;
        if (atual < 0 || prox < 0 || atual >= matrizDeCaminhos.size()) return 1e9;
        soma += matrizDeCaminhos[atual][prox];
    }
    return soma;
}

void GeneticAlgorithm::gerarPopulacao() {
    std::vector<Individuo> novaPopulacao(tamPopulacao);  // vetor completo

    #pragma omp parallel
    {
        std::mt19937 geradorLocal(std::random_device{}());  // gerador próprio por thread

        #pragma omp for
        for (int i = 0; i < tamPopulacao; ++i) {
            std::vector<int> genes(tamIndividuo);
            std::iota(genes.begin(), genes.end(), 1);
            std::shuffle(genes.begin(), genes.end(), geradorLocal);  // thread-safe
            double fit = calcularDistancia(genes);
            novaPopulacao[i] = Individuo(genes, fit);
        }
    }

    populacao = std::move(novaPopulacao);  // substitui populacao de forma segura
}

std::vector<Individuo> GeneticAlgorithm::selecao() {
    std::uniform_int_distribution<int> dist(0, populacao.size() - 1);
    std::vector<Individuo> candidatos;
    for (int i = 0; i < 10; ++i) candidatos.push_back(populacao[dist(gerador)]);
    std::sort(candidatos.begin(), candidatos.end(), [](auto& a, auto& b) { return a.fitness < b.fitness; });
    return {candidatos[0], candidatos[1]};
}

void GeneticAlgorithm::mutacao(Individuo& ind) {
    std::uniform_int_distribution<int> dist(0, tamIndividuo - 1);
    int a = dist(gerador), b;
    do { b = dist(gerador); } while (a == b);
    std::swap(ind.genes[a], ind.genes[b]);
    ind.fitness = calcularDistancia(ind.genes);
}

Individuo GeneticAlgorithm::cruzamentoOX(const Individuo& pai1, const Individuo& pai2) {
    std::uniform_int_distribution<int> dist(0, tamIndividuo - 1);
    int p1 = dist(gerador), p2 = dist(gerador);
    if (p1 > p2) std::swap(p1, p2);

    Individuo filho;
    filho.genes.resize(tamIndividuo, 0);
    std::unordered_set<int> usados;
    for (int i = p1; i < p2; ++i) {
        filho.genes[i] = pai1.genes[i];
        usados.insert(pai1.genes[i]);
    }

    int pos = 0;
    for (int gene : pai2.genes) {
        if (usados.count(gene)) continue;
        while (pos >= p1 && pos < p2) ++pos;
        filho.genes[pos++] = gene;
    }

    filho.fitness = calcularDistancia(filho.genes);
    return filho;
}

void GeneticAlgorithm::executarAlgoritmo() {
    gerarPopulacao();
    std::uniform_real_distribution<> dis(0.0, 1.0);
    Individuo melhor = melhorIndividuo(populacao);

    for (int g = 0; g < numGeracoes; ++g) {
        std::vector<Individuo> novaPop(numNovosIndividuos);

        #pragma omp parallel
        {
            int idThread = omp_get_thread_num();
            std::mt19937 geradorLocal(std::random_device{}() + idThread);
            std::uniform_real_distribution<> disLocal(0.0, 1.0);

            #pragma omp single
            for (int i = 0; i < numNovosIndividuos; ++i) {
                #pragma omp task firstprivate(i)
                {
                    auto pais = selecao();
                    float testeProbMutLocal = disLocal(geradorLocal);
                    #pragma omp critical
                    {
                        auto numThread = omp_get_thread_num();
                        // std::cout << "Pai1: " << pais[0].cromossomo() << " " << "Pai2: "<< pais[1].cromossomo() << " está com a thread " << numThread << std::endl;
                        std::cout << "Probabilidade de mutação " << testeProbMutLocal << "Na thread " << numThread << std::endl;
                    }
                    Individuo filho = cruzamentoOX(pais[0], pais[1]);
                    if (testeProbMutLocal < probMutacao)
                        mutacao(filho);
                    novaPop[i] = filho;
                }
            }
        }
        Individuo melhorNovo = melhorIndividuo(novaPop);
        if (melhorNovo.fitness < melhor.fitness) melhor = melhorNovo;
        populacao = std::move(novaPop);
    }

    printInd(melhor, "Melhor Individuo Encontrado: ");
    std::cout << "Fitness: " << melhor.fitness << std::endl;
}