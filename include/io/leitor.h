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
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
RESULTADO lerDataset(ESTRUTURA_DATASET **bufferSaida);

/**
 * @brief               Preenche um buffer com a quantidade definida de registros das compras.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ler nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar os registros de compras.
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida);

/**
 * @brief               Preenche um buffer com a quantidade definida de registros dos produtos.
 * 
 * Se não puder criar o buffer, abrir o arquivo e não ler nenhum registro, retorna uma mensagem de erro {0, ""}.
 * 
 * @param bufferSaida   Ponteiro para um ponteiro para armazenar os registros de produtos.
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida);

/**
 * @brief               Realiza a pesquisa binaria generica dentro de um arquivo de dados.
 * 
 * @param nomeArquivo   Caminho do arquivo binario.
 * @param valor         Valor (ID ou sequencial) que sera pesquisado.
 * @param desInicial    Indice inicial da busca no arquivo.
 * @param desFinal      Indice final da busca no arquivo (-1 para calcular o fim automaticamente).
 * @param obterChave    Lambda ou funcao que extrai o campo a ser comparado do registro.
 * @param saida         Ponteiro para ponteiro onde sera armazenado o registro encontrado.
 * 
 * @return RESULTADO    Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T>
RESULTADO pesquisaBinaria(
    const char *nomeArquivo,
    const char *nomeArquivoTemp,
    int valor,
    int desInicial,
    int desFinal,
    int (*obterChave)(const T*),
    T **saida
);

/**
 * @brief                   Realiza a pesquisa pelo arquivo de indice parcial (por ID ou sequencial).
 * 
 * @param nomeArquivoDados  Caminho do arquivo de dados (produtos ou compras).
 * @param nomeArquivoIndice Caminho do arquivo de indice.
 * @param valor             Valor (ID ou sequencial) que sera pesquisado.
 * @param obterMinBloco     Lambda que retorna o limite inicial do bloco de indice.
 * @param obterMaxBloco     Lambda que retorna o limite final do bloco de indice.
 * @param obterChaveDados   Lambda que retorna o campo do registro no arquivo de dados.
 * @param saida             Ponteiro para ponteiro onde sera armazenado o registro encontrado.
 * 
 * @return RESULTADO        Retorna {1, "..."} se sucesso, {0, "..."} se falhou.
 */
template <typename T, typename IndiceT>
RESULTADO pesquisaIndice(
    const char *nomeArquivoDados,
    const char *nomeArquivoIndice,
    const char *nomeArquivoTemp,
    int valor,
    int (*obterMinBloco)(const IndiceT*),
    int (*obterMaxBloco)(const IndiceT*),
    int (*obterChaveDados)(const T*),
    T **saida
);

/**
 * @brief       Pergunta o numero do produto que você quer usar.
 * 
 * @return int  Retorna o numero do produto.
 */
int escolherProduto();
#endif