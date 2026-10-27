#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/escritor.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/dataset.h"
#include "models/indice.h"
#include "utils/constantes.h"
#include "utils/arquivo.h"

void escreverProduto(FILE *arquivoProduto, const ESTRUTURA_PRODUTO *produto){
    fwrite(produto, sizeof(ESTRUTURA_PRODUTO), 1, arquivoProduto);
}

void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao){
    produto->numeracao = numeracao;
    produto->product_id = registro->product_id;
    produto->elo = -1;
    produto->excluido = 0;
    
    strncpy(produto->brand, registro->brand, sizeof(produto->brand) - 1);
    produto->brand[sizeof(produto->brand) - 1] = '\0';

    strncpy(produto->category_code, registro->category_code, sizeof(produto->category_code) - 1);
    produto->category_code[sizeof(produto->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoProduto(const ESTRUTURA_DATASET *buffer){
    int ultimoProduto = 0;

    /* registo do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_PRODUTO *produto = (ESTRUTURA_PRODUTO*)calloc(1, sizeof(ESTRUTURA_PRODUTO));
    if(produto == NULL){
        return {0, "Erro: Memoria insuficiente para o produto."};
    }
    
    /* ponteiro para o arquivo dos produtos do e-commerce */
    FILE *arquivoProduto = abrirArquivo(ARQUIVO_PRODUTO, "wb");
    if(arquivoProduto == NULL){
        free(produto);
        return {0, "Erro: Falha ao abrir o arquivo dos produtos."};
    }

    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        if(buffer[i].product_id > ultimoProduto){
            converterEstruturaProduto(&buffer[i], produto, numeracao++);
            escreverProduto(arquivoProduto, produto);
            ultimoProduto = buffer[i].product_id;
        }
    }

    fclose(arquivoProduto);
    free(produto);

    return {1, "Sucesso ao preencher o arquivo dos produtos."};
}

void escreverCompra(FILE *arquivoCompra, const ESTRUTURA_COMPRA *compra){
    fwrite(compra, sizeof(ESTRUTURA_COMPRA), 1, arquivoCompra);
}

void converterEstruturaCompra(const ESTRUTURA_DATASET *registro, ESTRUTURA_COMPRA *compra, unsigned long numeracao) {
    compra->numeracao = numeracao;
    compra->price = registro->price;
    compra->product_id = registro->product_id;
    compra->elo = -1;
    compra->excluido = 0;
    
    strncpy(compra->brand, registro->brand, sizeof(compra->brand) - 1);
    compra->brand[sizeof(compra->brand) - 1] = '\0';

    strncpy(compra->category_code, registro->category_code, sizeof(compra->category_code) - 1);
    compra->category_code[sizeof(compra->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoCompra(const ESTRUTURA_DATASET *buffer){
    /* registo do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_COMPRA *compra = (ESTRUTURA_COMPRA*)calloc(1, sizeof(ESTRUTURA_COMPRA));
    if(compra == NULL){
        return {0, "Erro: Memoria insuficiente para a compra."};
    }
    
    /* ponteiro para o arquivo das compras realizadas no e-commerce */
    FILE *arquivoCompra = abrirArquivo(ARQUIVO_COMPRA, "wb");
    if(arquivoCompra == NULL){
        free(compra);
        return {0, "Erro: Falha ao abrir o arquivo das compras."};
    }

    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        if(strcmp(buffer[i].event_type, "purchase") == 0){
            converterEstruturaCompra(&buffer[i], compra, numeracao++);
            escreverCompra(arquivoCompra, compra);
        }
    }

    fclose(arquivoCompra);
    free(compra);

    return {1, "Sucesso ao preencher o arquivo das compras."};
}

template <typename T>
RESULTADO excluirRegistro(const char *nomeArquivo, T *registro){
    if(registro == NULL){
        return {0, "Erro: Registro nulo."};
    }

    FILE *arquivo = abrirArquivo(nomeArquivo, "rb+");
    if(arquivo == NULL){
        return {0, "Erro: Falha ao abrir o arquivo."};
    }

    fseek(arquivo, sizeof(T) * registro->numeracao, SEEK_SET);

    registro->excluido = 1;

    fwrite(registro, sizeof(T), 1, arquivo);

    fclose(arquivo);

    return {1, "Registro excluido com sucesso."};
}

/* Instanciacao explicita para permitir a chamada direto no main.cpp */
template RESULTADO excluirRegistro<ESTRUTURA_PRODUTO>(const char*, ESTRUTURA_PRODUTO*);
template RESULTADO excluirRegistro<ESTRUTURA_COMPRA>(const char*, ESTRUTURA_COMPRA*);

template <typename T, typename IndiceT>
RESULTADO escreverIndice(const char *nomeArquivoDados, const char *nomeArquivoIndice){
    /* ponteiro para o arquivo de dados */
    FILE *arquivo = abrirArquivo(nomeArquivoDados, "rb");
    if(arquivo == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de dados."};
    }

    /* calcula o total de registros do arquivo para nao passar do fim no ultimo bloco */
    fseek(arquivo, 0, SEEK_END);
    int totalRegistros = (int)(ftell(arquivo) / sizeof(T));
    if(totalRegistros == 0){
        fclose(arquivo);
        return {0, "Erro: Arquivo de dados vazio para gerar indice."};
    }
    fseek(arquivo, 0, SEEK_SET);

    /* ponteiro para o arquivo do indice */
    FILE *arquivoIndice = abrirArquivo(nomeArquivoIndice, "wb");
    if(arquivoIndice == NULL){
        fclose(arquivo);
        return {0, "Erro: Falha ao abrir o arquivo de indice."};
    }

    T registro;
    IndiceT indice;
    int posicaoInicioBloco = 0;

    while(posicaoInicioBloco < totalRegistros){
        /* le o primeiro registro do bloco */
        fseek(arquivo, sizeof(T) * posicaoInicioBloco, SEEK_SET);
        if(fread(&registro, sizeof(T), 1, arquivo) != 1){
            break;
        }

        indice.primeiro_numeracao = registro.numeracao;
        indice.primeiro_produto_id = registro.product_id;

        /* calcula o indice do ultimo registro do bloco */
        int posicaoFimBloco = posicaoInicioBloco + QUANTIDADE_REGISTRO_INDICE - 1;
        if(posicaoFimBloco >= totalRegistros){
            posicaoFimBloco = totalRegistros - 1;
        }

        /* le o ultimo registro do bloco */
        fseek(arquivo, sizeof(T) * posicaoFimBloco, SEEK_SET);
        if(fread(&registro, sizeof(T), 1, arquivo) != 1){
            break;
        }

        indice.ultimo_numeracao = registro.numeracao;
        indice.ultimo_produto_id = registro.product_id;

        fwrite(&indice, sizeof(IndiceT), 1, arquivoIndice);

        /* avanca para o inicio do proximo bloco */
        posicaoInicioBloco = posicaoFimBloco + 1;
    }

    fclose(arquivo);
    fclose(arquivoIndice);

    return {1, "Sucesso ao criar arquivo de indice."};
}

/* Instanciacao explicita para permitir a chamada direto no main.cpp */
template RESULTADO escreverIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(const char*, const char*);

template RESULTADO escreverIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(const char*, const char*);
