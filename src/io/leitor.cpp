#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/arquivo.h"
#include "models/dataset.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "utils/constantes.h"

RESULTADO lerDataset(ESTRUTURA_DATASET **bufferSaida){
    /* malloc na memoria para os dados do dataset */
    ESTRUTURA_DATASET *buffer = (ESTRUTURA_DATASET*)malloc(sizeof(ESTRUTURA_DATASET) * TAMANHO_BUFFER);
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo dataset */
    FILE *dataset = abrirArquivo(DATASET, "rb");
    if(dataset == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo dataset."};
    }

    if(!fread(buffer, sizeof(ESTRUTURA_DATASET), TAMANHO_BUFFER, dataset)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum arquivo do dataset."};
    };

    fclose(dataset);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar o dadaset na memoria."};
}

RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida){
    /* malloc na memoria para os dados das compras */
    ESTRUTURA_COMPRA *buffer = (ESTRUTURA_COMPRA*)calloc(QUANTIDADE_REGISTRO, sizeof(ESTRUTURA_COMPRA));
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo das compras */
    FILE *arquivo = abrirArquivo(ARQUIVO_COMPRA, "rb");
    if(arquivo == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo de compras."};
    }

    if(!fread(buffer, sizeof(ESTRUTURA_COMPRA), QUANTIDADE_REGISTRO, arquivo)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum registro do arquivo de compras."};
    };

    fclose(arquivo);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar as compras na memoria."};    
}

RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida){
    /* malloc na memoria para os dados dos produtos */
    ESTRUTURA_PRODUTO *buffer = (ESTRUTURA_PRODUTO*)calloc(QUANTIDADE_REGISTRO, sizeof(ESTRUTURA_PRODUTO));
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo dos produtos */
    FILE *arquivo = abrirArquivo(ARQUIVO_PRODUTO, "rb");
    if(arquivo == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo de compras."};
    }

    if(!fread(buffer, sizeof(ESTRUTURA_PRODUTO), QUANTIDADE_REGISTRO, arquivo)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum registro do arquivo de compras."};
    };

    fclose(arquivo);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar os produtos na memoria."};    
}
