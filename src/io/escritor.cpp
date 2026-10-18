#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/escritor.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/dataset.h"
#include "utils/constantes.h"
#include "utils/arquivo.h"

void escreverProduto(FILE *arquivoProduto, const ESTRUTURA_PRODUTO *produto){
    fwrite(produto, sizeof(ESTRUTURA_PRODUTO), 1, arquivoProduto);
}

void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao){
    produto->numeracao = numeracao;
    produto->product_id = registro->product_id;
    
    strncpy(produto->brand, registro->brand, sizeof(produto->brand) - 1);
    produto->brand[sizeof(produto->brand) - 1] = '\0';

    strncpy(produto->category_code, registro->category_code, sizeof(produto->category_code) - 1);
    produto->category_code[sizeof(produto->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoProduto(const ESTRUTURA_DATASET *buffer){
    int ultimoProduto = 0;

    /* registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_PRODUTO *produto = (ESTRUTURA_PRODUTO*)malloc(sizeof(ESTRUTURA_PRODUTO));
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
    
    strncpy(compra->brand, registro->brand, sizeof(compra->brand) - 1);
    compra->brand[sizeof(compra->brand) - 1] = '\0';

    strncpy(compra->category_code, registro->category_code, sizeof(compra->category_code) - 1);
    compra->category_code[sizeof(compra->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoCompra(const ESTRUTURA_DATASET *buffer){
    /* registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_COMPRA *compra = (ESTRUTURA_COMPRA*)malloc(sizeof(ESTRUTURA_COMPRA));
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