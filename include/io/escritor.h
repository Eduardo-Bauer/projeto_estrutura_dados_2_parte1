#ifndef ESCRITOR_H
#define ESCRITOR_H

#include "models/dataset.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/status.h"

/**
 * @brief                  Escreve os produtos no arquivo de produtos.
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
 * @param registro     Ponteiro para o local de um registro do buffer.
 * @param produto      Ponteiro para o local que será convertido o registro do buffer.
 * @param numeracao    Numero sequencial no arquivo de produtos.
 * 
 * @return void        Retorna nada.
 */
void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao);

/**
 * @brief               Cria e grava o arquivo de produtos a partir do buffer em memória.
 * 
 * Se não puder criar o arquivo ou registro do produto, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param buffer        Ponteiro para o array de compras (dados lidos do dataset).
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
RESULTADO preencherArquivoProduto(const ESTRUTURA_DATASET *buffer);

/**
 * @brief                  Escreve as compras realizadas dentro do arquivo de compras.
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
 * @param registro    Ponteiro para o local de um registro do buffer.
 * @param compra      Ponteiro para o local que será convertido o registro do buffer.
 * @param numeracao   Numero sequencial no arquivo de compras.
 * 
 * @return void       Retorna nada.
 */
void converterEstruturaCompra(const ESTRUTURA_DATASET *registro, ESTRUTURA_COMPRA *compra, unsigned long numeracao);

/**
 * @brief               Cria e grava o arquivo de vendas a partir do buffer em memória.
 * 
 * Se não puder criar o arquivo ou registro do produto, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param buffer        Ponteiro para o array de compras (dados lidos do dataset).
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
RESULTADO preencherArquivoCompra(const ESTRUTURA_DATASET *buffer);

/**
 * @brief                   Cria e grava o arquivo de indice parcial para qualquer estrutura de dados.
 * 
 * Se não puder abrir os arquivos ou não tiver nenhum registro no arqyuvi de dados, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param nomeArquivoDados  Ponteiro para arquivo de dados (produtos ou compras).
 * @param nomeArquivoIndice Ponteiro para arquivo de indice que sera gerado.
 * 
 * @return RESULTADO        Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T, typename IndiceT>
RESULTADO escreverIndice(const char *nomeArquivoDados, const char *nomeArquivoIndice);

/**
 * @brief               Cria e grava o arquivo de indice parcial para qualquer estrutura de dados.
 * 
 * Se não puder abrir os arquivos, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param nomeArquivoDados  Ponteiro para arquivo de dados (produtos ou compras).
 * @param nomeArquivoTemp   Ponteiro para arquivo temporario de registros inseridos recentemente (produtos ou compras).
 * @param registro          Ponteiro para o registro que sera excluido.
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T>
RESULTADO excluirRegistro(const char *nomeArquivoDados, const char *nomeArquivoTemp, T *registro);

/**
 * @brief Insere um novo registro no arquivo temporario (data/temp) e atualiza o elo ordenado por product_id.
 *  
 * Se não puder abrir os arquivos, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param nomeArquivoDados  Ponteiro para arquivo de dados (produtos ou compras).
 * @param nomeArquivoTemp   Ponteiro para arquivo temporario de registros inseridos recentemente (produtos ou compras).
 * @param novoRegistro      Registro que sera inserido.
 *  
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T>
RESULTADO inserirRegistro(const char *nomeArquivoDados, const char *nomeArquivoTemp, T novoRegistro);

/**
 * @brief Reorganiza o arquivo principal, removendo fisicamente os registros excluidos, reajustando a numeracao e recriando o arquivo de indice.
 *
 * Se não puder abrir os arquivos, retorna uma mensagem de erro {0, "..."}.
 * 
 * @param nomeArquivoDados      Ponteiro para arquivo de dados (produtos ou compras).
 * @param nomeArquivoTemp       Ponteiro para arquivo temporario de registros inseridos recentemente (produtos ou compras).
 * @param nomeArquivoIndice     Ponteiro para arquivo de indice (produtos ou compras).
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T, typename IndiceT>
RESULTADO reorganizarArquivo(const char *nomeArquivoDados, const char *nomeArquivoTemp, const char *nomeArquivoIndice);

#endif