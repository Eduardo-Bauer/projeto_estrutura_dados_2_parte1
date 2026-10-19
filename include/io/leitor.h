#ifndef LEITOR_H
#define LEITOR_H

#include "models/dataset.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/status.h"

/**
 * @brief               Preenche um buffer com os dados do dataset.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ler nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar os registros do dataset.
 * 
 * @return RESULTADO    Retorna {1, ""} se sucesso, {0, ""} se falhou.
 */
RESULTADO lerDataset(ESTRUTURA_DATASET **bufferSaida);


RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida);


RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida);

#endif