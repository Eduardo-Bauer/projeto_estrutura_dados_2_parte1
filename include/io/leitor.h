#ifndef LEITOR_H
#define LEITOR_H

#include "../models/dataset.h"
#include "../models/arquivoCompra.h"

/**
 * @brief Preenche um buffer com os dados do dataset.
 * 
 * Se não puder abrir o arquivo ou criar o buffer, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @return ESTRUTURA_DATASET*   Ponteiro válido para o buffer criado.
 */
ESTRUTURA_DATASET* lerDataset();

#endif