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

template <typename T>
RESULTADO inserirRegistro(const char *nomeArquivoDados, const char *nomeArquivoTemp, T novoRegistro){
    FILE *arqDados = abrirArquivo(nomeArquivoDados, "rb+");
    if(arqDados == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de dados principal."};
    }

    /* Tenta abrir com rb+ para manter os registros existentes; se nao existir, cria com wb+ */
    FILE *arqTemp = abrirArquivo(nomeArquivoTemp, "rb+");
    if(arqTemp == NULL){
        arqTemp = abrirArquivo(nomeArquivoTemp, "wb+");
        if(arqTemp == NULL){
            fclose(arqDados);
            return {0, "Erro: Falha ao criar o arquivo temporario (verifique se a pasta data/temp existe)."};
        }
    }

    fseek(arqDados, 0, SEEK_END);
    int totalDados = (int)(ftell(arqDados) / sizeof(T));

    if(totalDados == 0){
        fclose(arqDados);
        fclose(arqTemp);
        return {0, "Erro: Arquivo principal vazio."};
    }

    /* 1. Descobre a posicao onde o novo registro sera gravado no arquivo temporario */
    fseek(arqTemp, 0, SEEK_END);
    int novaPosicaoTemp = (int)(ftell(arqTemp) / sizeof(T));

    /* A numeracao continua a sequencia do arquivo principal para funcionar na busca sequencial */
    novoRegistro.numeracao = totalDados + novaPosicaoTemp;
    novoRegistro.excluido = 0;
    novoRegistro.elo = -1;

    /* 2. Busca binaria no arquivo principal para achar o registro antecessor (maior product_id <= novo) */
    int min = 0;
    int max = totalDados - 1;
    int posAntecessor = 0; /* Se o novo ID for menor que todos, vincula na posicao 0 */
    T itemAux;

    while(min <= max){
        int meio = min + (max - min) / 2;
        fseek(arqDados, sizeof(T) * meio, SEEK_SET);
        fread(&itemAux, sizeof(T), 1, arqDados);

        if(itemAux.product_id <= novoRegistro.product_id){
            posAntecessor = meio;
            min = meio + 1;
        } else {
            max = meio - 1;
        }
    }

    /* Carrega o registro antecessor do arquivo principal */
    T itemPrincipal;
    fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
    fread(&itemPrincipal, sizeof(T), 1, arqDados);

    /* 3. Encadeia mantendo a ordem crescente de product_id */
    if(itemPrincipal.elo == -1){
        itemPrincipal.elo = novaPosicaoTemp;
        fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
        fwrite(&itemPrincipal, sizeof(T), 1, arqDados);
    } else {
        int eloAtual = itemPrincipal.elo;
        int eloAnterior = -1;
        T itemTempAtual;

        while(eloAtual != -1){
            fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
            if(fread(&itemTempAtual, sizeof(T), 1, arqTemp) != 1){
                break;
            }

            if(novoRegistro.product_id < itemTempAtual.product_id){
                break;
            }

            eloAnterior = eloAtual;
            eloAtual = itemTempAtual.elo;
        }

        novoRegistro.elo = eloAtual;

        if(eloAnterior == -1){
            itemPrincipal.elo = novaPosicaoTemp;
            fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
            fwrite(&itemPrincipal, sizeof(T), 1, arqDados);
        } else {
            fseek(arqTemp, sizeof(T) * eloAnterior, SEEK_SET);
            fread(&itemTempAtual, sizeof(T), 1, arqTemp);
            itemTempAtual.elo = novaPosicaoTemp;
            fseek(arqTemp, sizeof(T) * eloAnterior, SEEK_SET);
            fwrite(&itemTempAtual, sizeof(T), 1, arqTemp);
        }
    }

    /* 4. Grava o novo registro no final do arquivo temporario */
    fseek(arqTemp, sizeof(T) * novaPosicaoTemp, SEEK_SET);
    fwrite(&novoRegistro, sizeof(T), 1, arqTemp);

    fclose(arqDados);
    fclose(arqTemp);

    return {1, "Registro inserido no arquivo temporario e vinculado por elo com sucesso."};
}

/* Instanciacao explicita no final do escritor.cpp */
template RESULTADO inserirRegistro<ESTRUTURA_PRODUTO>(const char*, const char*, ESTRUTURA_PRODUTO);
template RESULTADO inserirRegistro<ESTRUTURA_COMPRA>(const char*, const char*, ESTRUTURA_COMPRA);
