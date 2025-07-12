#pragma once
#include <vector>
#include <string>
#include "Individuo.h"

std::vector<std::vector<double>> lerArquivo(const std::string& caminhoArquivo);
void printInd(const Individuo& individuo, const std::string& texto = "");
void imprimirElementosComDistancias(const std::vector<Individuo>& elementos);