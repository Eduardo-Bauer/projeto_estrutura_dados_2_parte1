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

RESULTADO pesquisaBinariaProduto(ESTRUTURA_PRODUTO **saida, const int valor){
    /* diferenca gerada entre o produto buscado e o registro analisado */
    int diferenca;
    /* faixa minima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMinimo = 0;
    /* faixa maxima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMaximo = 0;
    /* valor de deslocamento realizado no arquivo */
    int deslocamento = -1;

    ESTRUTURA_PRODUTO *registro = (ESTRUTURA_PRODUTO*)malloc(sizeof(ESTRUTURA_PRODUTO));
    if(registro == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo de produtos */
    FILE *arquivo = fopen(ARQUIVO_PRODUTO, "rb");
    if(arquivo == NULL){
        free(registro);
        return {0, "Erro: Falha ao abrir o arquivo de compras."};
    }

    fseek(arquivo, 0, SEEK_END);

    /* tamanho do arquivo de produtos */
    long tamanho_arquivo = ftell(arquivo);

    /* quantidade de registro dentro do arquivo de produtos */
    int total_registros = tamanho_arquivo / sizeof(ESTRUTURA_PRODUTO);
    
    if (total_registros == 0) {
        fclose(arquivo);
        free(registro);
        return {0, "Erro: Arquivo vazio."};
    }

    deslocamentoMaximo = total_registros - 1;

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivo, sizeof(ESTRUTURA_PRODUTO) * deslocamento, SEEK_SET);
        fread(registro, sizeof(ESTRUTURA_PRODUTO), 1, arquivo);

        diferenca = valor - registro->product_id;

        if(diferenca < 0){
            deslocamentoMaximo = deslocamento - 1;
        }
        else if(diferenca > 0){
            deslocamentoMinimo = deslocamento + 1;
        }
        else {
            *saida = registro;
            fclose(arquivo);
            return {1, "Arquivo encontrado."}; 
        }
    }

    fclose(arquivo);
    free(registro);
    return {0, "Arquivo nao encontrado."};
}

RESULTADO pesquisaBinariaCompra(ESTRUTURA_COMPRA **saida, const int valor){
    /* diferenca gerada entre o produto buscado e o registro analisado */
    int diferenca;
    /* faixa minima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMinimo = 0;
    /* faixa maxima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMaximo = 0;
    /* valor de deslocamento realizado no arquivo */
    int deslocamento = -1;

    ESTRUTURA_COMPRA *registro = (ESTRUTURA_COMPRA*)malloc(sizeof(ESTRUTURA_COMPRA));
    if(registro == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo das compras */
    FILE *arquivo = fopen(ARQUIVO_COMPRA, "rb");
    if(arquivo == NULL){
        free(registro);
        return {0, "Erro: Falha ao abrir o arquivo de compras."};
    }

    fseek(arquivo, 0, SEEK_END);

    /* tamanho do arquivo de compras */
    long tamanho_arquivo = ftell(arquivo);

    /* quantidade de registros dentro do arquivo de compras */
    int total_registros = tamanho_arquivo / sizeof(ESTRUTURA_COMPRA);
    
    if (total_registros == 0) {
        fclose(arquivo);
        free(registro);
        return {0, "Erro: Arquivo vazio."};
    }

    deslocamentoMaximo = total_registros - 1;

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivo, sizeof(ESTRUTURA_COMPRA) * deslocamento, SEEK_SET);
        fread(registro, sizeof(ESTRUTURA_COMPRA), 1, arquivo);

        diferenca = valor - registro->product_id;

        if(diferenca < 0){
            deslocamentoMaximo = deslocamento - 1;
        }
        else if(diferenca > 0){
            deslocamentoMinimo = deslocamento + 1;
        }
        else {
            *saida = registro;
            fclose(arquivo);
            return {1, "Arquivo encontrado."}; 
        }
    }

    fclose(arquivo);
    free(registro);
    return {0, "Arquivo nao encontrado."};
}
