#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/arquivo.h"
#include "models/dataset.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/indice.h"
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

template <typename T>
RESULTADO pesquisaBinaria(const char *nomeArquivo, int valor, int desInicial, int desFinal, int (*obterChave)(const T*), T **saida){
    /* diferenca gerada entre o produto buscado e o registro analisado */
    int diferenca;
    /* faixa minima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMinimo = desInicial;
    /* faixa maxima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMaximo = desFinal;
    /* valor de deslocamento realizado no arquivo */
    int deslocamento = -1;
    
    T *registro = (T*)malloc(sizeof(T));
    if (registro == NULL) {
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        free(registro);
        return {0, "Erro: Falha ao abrir o arquivo."};
    }

    if(desFinal < 0){
        fseek(arquivo, 0, SEEK_END);
        int totalRegistro = (int)(ftell(arquivo) / sizeof(T));
        if(totalRegistro == 0){
            fclose(arquivo);
            free(registro);
            return {0, "Erro: Arquivo vazio."};
        }

        deslocamentoMaximo = totalRegistro - 1;
    }

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivo, sizeof(T) * deslocamento, SEEK_SET);
        
        if (fread(registro, sizeof(T), 1, arquivo) != 1) {
            break;
        }

        diferenca = valor - obterChave(registro);

        if(diferenca < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        if(diferenca > 0){
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        fclose(arquivo);        

        if(registro->excluido){
            free(registro);
            return {0, "Registro encontrado, mas esta excluido\n"};
        }

        *saida = registro;
        return {1, "Registro encontrado."}; 
    }

    fclose(arquivo);
    free(registro);
    return {0, "Erro: Registro nao encontrado."};
}

template <typename T, typename IndiceT>
RESULTADO pesquisaIndice(
    const char *nomeArquivoDados, 
    const char *nomeArquivoIndice, 
    int valor, 
    int (*obterMinBloco)(const IndiceT*),
    int (*obterMaxBloco)(const IndiceT*),
    int (*obterChaveDados)(const T*),
    T **saida){

    /* diferenca gerada entre o valor buscado e os limites do bloco analisado */
    int diferencaMenor;
    int diferencaMaior;
    /* faixa minima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMinimo = 0;
    /* faixa maxima que e usado para calcular o deslocamento da pesquisa binaria */
    int deslocamentoMaximo = 0;
    /* valor de deslocamento realizado no arquivo */
    int deslocamento = -1;

    IndiceT bloco;

    FILE *arquivoIndice = fopen(nomeArquivoIndice, "rb");
    if(arquivoIndice == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de indice."};
    }

    fseek(arquivoIndice, 0, SEEK_END);
    int totalBloco = (int)(ftell(arquivoIndice) / sizeof(IndiceT));
    if(totalBloco == 0){
        fclose(arquivoIndice);
        return {0, "Erro: Arquivo de indice vazio."};
    }

    deslocamentoMaximo = totalBloco - 1;

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivoIndice, sizeof(IndiceT) * deslocamento, SEEK_SET);
        fread(&bloco, sizeof(IndiceT), 1, arquivoIndice);

        diferencaMenor = valor - obterMinBloco(&bloco);
        diferencaMaior = valor - obterMaxBloco(&bloco);

        if(diferencaMenor < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        if(diferencaMaior > 0){
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        fclose(arquivoIndice);        

        return pesquisaBinaria<T>(
            nomeArquivoDados, 
            valor, 
            bloco.primeiro_numeracao, 
            bloco.ultimo_numeracao, 
            obterChaveDados, 
            saida
        );
    }

    fclose(arquivoIndice);
    return {0, "Registro nao encontrado."};
}

int escolherProduto(){
    int produto = 0;

    scanf("%d", &produto);

    return produto;
}

/* Instanciacao explicita para permitir a chamada direto no main.cpp */
template RESULTADO pesquisaBinaria<ESTRUTURA_PRODUTO>(
    const char*, int, int, int, int (*)(const ESTRUTURA_PRODUTO*), ESTRUTURA_PRODUTO**
);

template RESULTADO pesquisaBinaria<ESTRUTURA_COMPRA>(
    const char*, int, int, int, int (*)(const ESTRUTURA_COMPRA*), ESTRUTURA_COMPRA**
);

template RESULTADO pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
    const char*, const char*, int,
    int (*)(const INDICE_PRODUTO*), int (*)(const INDICE_PRODUTO*), int (*)(const ESTRUTURA_PRODUTO*),
    ESTRUTURA_PRODUTO**
);

template RESULTADO pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
    const char*, const char*, int,
    int (*)(const INDICE_COMPRA*), int (*)(const INDICE_COMPRA*), int (*)(const ESTRUTURA_COMPRA*),
    ESTRUTURA_COMPRA**
);
