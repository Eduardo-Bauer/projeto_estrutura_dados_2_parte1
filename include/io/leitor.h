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

/**
 * @brief               Preenche um buffer com a quantidade definida de registros das compras.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ler nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar os registros de compras.
 * 
 * @return RESULTADO    Retorna {1, ""} se sucesso, {0, ""} se falhou.
 */
RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida);

/**
 * @brief               Preenche um buffer com a quantidade definida de registros daos produtos.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ler nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar os registros de produtos.
 * 
 * @return RESULTADO    Retorna {1, ""} se sucesso, {0, ""} se falhou.
 */
RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida);

#endif