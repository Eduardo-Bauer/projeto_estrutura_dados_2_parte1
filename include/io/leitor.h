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

/**
 * @brief               Realiza a pesquisa binaria dentro do arquivo dos produtos para achar o registro solicitado.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ter nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar o registro de produto encontrado.
 * @param valor         Numero do produto que será pesquisado.
 * 
 * @return RESULTADO    Retorna {1, ""} se sucesso, {0, ""} se falhou.
 */
RESULTADO pesquisaBinariaProduto(ESTRUTURA_PRODUTO **saida, const int valor);

/**
 * @brief               Realiza a pesquisa binaria dentro do arquivo das compras para achar o registro solicitado.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ter nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar o registro de compra encontrado.
 * @param valor         Numero do produto que será pesquisado.
 * 
 * @return RESULTADO    Retorna {1, ""} se sucesso, {0, ""} se falhou.
 */
RESULTADO pesquisaBinariaCompra(ESTRUTURA_COMPRA **saida, const int valor);
#endif