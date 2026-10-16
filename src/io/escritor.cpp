#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/escritor.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/dataset.h"
#include "utils/constantes.h"
#include "utils/arquivo.h"

void escreverProduto(FILE *arquivoProduto, const ESTRUTURA_PRODUTO *produto){
    fwrite(produto, sizeof(ESTRUTURA_PRODUTO), 1, arquivoProduto);
}

void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao){
    if(produto == NULL || registro == NULL){
        printf("Erro: registro para conversao vazio.\n");
        exit(1);
    }

    produto->numeracao = numeracao;
    produto->product_id = registro->product_id;
    
    strncpy(produto->brand, registro->brand, sizeof(produto->brand) - 1);
    produto->brand[sizeof(produto->brand) - 1] = '\0';

    strncpy(produto->category_code, registro->category_code, sizeof(produto->category_code) - 1);
    produto->category_code[sizeof(produto->category_code) - 1] = '\0';
}

int preencherArquivoProduto(const ESTRUTURA_DATASET *buffer){
    int ultimoProduto = 0;

    /* registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_PRODUTO *produto = (ESTRUTURA_PRODUTO*)malloc(sizeof(ESTRUTURA_PRODUTO));
    if(produto == NULL){
        printf("Erro: Memoria insuficiente para o produto.\n");
        return 0;
    }
    
    /* ponteiro para o arquivo dos produtos do e-commerce */
    FILE *arquivoProduto = abrirArquivo(ARQUIVO_PRODUTO, "wb");

    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        if(buffer[i].product_id > ultimoProduto){
            converterEstruturaProduto(&buffer[i], produto, numeracao++);
            escreverProduto(arquivoProduto, produto);
            ultimoProduto = buffer[i].product_id;
        }
    }

    fclose(arquivoProduto);
    free(produto);
    produto = NULL;

    return 1;
}


void escreverCompra(FILE *arquivoCompra, const ESTRUTURA_COMPRA *compra){
    fwrite(compra, sizeof(ESTRUTURA_COMPRA), 1, arquivoCompra);
}

void converterEstruturaCompra(const ESTRUTURA_DATASET *registro, ESTRUTURA_COMPRA *compra, unsigned long numeracao) {
    if (compra == NULL || registro == NULL) {
        printf("Erro: registro para conversao vazio.\n");
        exit(1);
    }

    compra->numeracao = numeracao;
    compra->price = registro->price;
    compra->product_id = registro->product_id;
    
    strncpy(compra->brand, registro->brand, sizeof(compra->brand) - 1);
    compra->brand[sizeof(compra->brand) - 1] = '\0';

    strncpy(compra->category_code, registro->category_code, sizeof(compra->category_code) - 1);
    compra->category_code[sizeof(compra->category_code) - 1] = '\0';
}

int preencherArquivoCompra(const ESTRUTURA_DATASET *buffer){
    /* registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_COMPRA *compra = (ESTRUTURA_COMPRA*)malloc(sizeof(ESTRUTURA_COMPRA));
    if(compra == NULL){
        printf("Erro: Memoria insuficiente para o produto.\n");
        return 0;
    }
    
    /* ponteiro para o arquivo das compras realizadas no e-commerce */
    FILE *arquivoCompra = abrirArquivo(ARQUIVO_COMPRA, "wb");

    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        if(strcmp(buffer[i].event_type, "purchase") == 0){
            converterEstruturaCompra(&buffer[i], compra, numeracao++);
            escreverCompra(arquivoCompra, compra);
        }
    }

    fclose(arquivoCompra);
    free(compra);
    compra = NULL;

    return 1;
}