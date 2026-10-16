#ifndef ESCRITOR_H
#define ESCRITOR_H

#include "models/dataset.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"

/**
 * @brief                  Escreve os produtos encontrados dentro do arquivo.
 * 
 * @param arquivoProduto   Ponteiro para o arquivo que será salvo os produtos
 * @param produto          Ponteiro para o registro do produto que será escrita no arquivo.
 * 
 * @return void            Retorna nada.
 */
void escreverProduto(FILE *arquivoProduto, const ESTRUTURA_PRODUTO *produto);

/**
 * @brief              Converte o registro do buffer em um registro compativel com a estrutura dos produtos.
 * 
 * Se não puder criar o arquivo ou registro do produto, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @param registro     Ponteiro para o local de um registro do buffer.
 * @param produto      Ponteiro para o local que será convertido o registro do buffer.
 * @param numeracao    Numero sequencial no arquivo de produtos.
 * 
 * @return void        Retorna nada.
 */
void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao);

/**
 * @brief            Cria e grava o arquivo de produtos a partir do buffer em memória.
 * 
 * Se não puder criar o arquivo ou registro do produto, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @param buffer     Ponteiro para o array de estruturas (dados lidos do dataset).
 * 
 * @return int       Retorna 1 se sucesso, 0 se falhou.
 */
int preencherArquivoProduto(const ESTRUTURA_DATASET *buffer);

/**
 * @brief                  Escreve as compras realizadas dentro do arquivo.
 * 
 * @param arquivoCompra    Ponteiro para o arquivo que será salvo as compras
 * @param compra           Ponteiro para o registro da compra que será escrita no arquivo.
 * 
 * @return void            Retorna nada.
 */
void escreverCompra(FILE *arquivoCompra, const ESTRUTURA_COMPRA *compra);

/**
 * @brief             Converte o registro do buffer em um registro compativel com a estrutura das compras.
 * 
 * Se não puder criar o arquivo ou registro do produto, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @param registro    Ponteiro para o local de um registro do buffer.
 * @param compra      Ponteiro para o local que será convertido o registro do buffer.
 * @param numeracao   Numero sequencial no arquivo de produtos.
 * 
 * @return void       Retorna nada.
 */
void converterEstruturaCompra(const ESTRUTURA_DATASET *registro, ESTRUTURA_COMPRA *compra, unsigned long numeracao);

/**
 * @brief            Cria e grava o arquivo de vendas a partir do buffer em memória.
 * 
 * Se não puder criar o arquivo ou registro do produto, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @param buffer     Ponteiro para o array de estruturas (dados lidos do dataset).
 * 
 * @return int       Retorna 1 se sucesso, 0 se falhou.
 */
int preencherArquivoCompra(const ESTRUTURA_DATASET *buffer);

#endif