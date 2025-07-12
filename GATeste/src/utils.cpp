#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::vector<double>> lerArquivo(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    std::vector<std::vector<double>> matriz;
    std::string linha;
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << std::endl;
        return {};
    }
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::vector<double> linhaNumeros;
        double num;
        while (iss >> num) linhaNumeros.push_back(num);
        if (!linhaNumeros.empty()) matriz.push_back(std::move(linhaNumeros));
    }
    return matriz;
}

void printInd(const Individuo& ind, const std::string& texto) {
    std::cout << texto;
    for (int gene : ind.genes) std::cout << gene << " ";
    std::cout << std::endl;
}

void imprimirElementosComDistancias(const std::vector<Individuo>& elementos) {
    std::cout << "Printando Elementos selecionados\n";
    for (int i = 0; i < elementos.size(); ++i) {
        std::cout << i+1 << "º-Individuo ";
        for (int g : elementos[i].genes) std::cout << g << " ";
        std::cout << " - Fitness: " << elementos[i].fitness << std::endl;
    }
}