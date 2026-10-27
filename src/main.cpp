#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils/constantes.h"
#include "utils/arquivo.h"
#include "models/status.h"
#include "models/indice.h"
#include "io/leitor.h"
#include "io/escritor.h"
#include "io/tela.h"

void inserirDadosIniciais(){
    mostrarMensagem({2, "Iniciando a insercao dos dados iniciais."});

    mostrarMensagem({2, "Criando buffer dos dados do dataset."});

    ESTRUTURA_DATASET *buffer = NULL;

    mostrarMensagem(lerDataset(&buffer));

    mostrarMensagem({2, "Gerando arquivo das compras realizadas no e-commerce."});

    mostrarMensagem(preencherArquivoCompra(buffer));

    mostrarMensagem({2, "Gerando arquivo dos produtos do e-commerce."});

    mostrarMensagem(preencherArquivoProduto(buffer));

    mostrarMensagem({2, "Gerando arquivo dos indices."});

    mostrarMensagem(escreverIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(ARQUIVO_PRODUTO, ARQUIVO_INDICE_PRODUTO));

    mostrarMensagem(escreverIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(ARQUIVO_COMPRA, ARQUIVO_INDICE_COMPRA));

    free(buffer);

    /* Limpa os arquivos temporarios ao recriar a base inicial */
    FILE *tempProd = abrirArquivo(ARQUIVO_TEMP_PRODUTO, "wb");
    if(tempProd != NULL) fclose(tempProd);

    FILE *tempComp = abrirArquivo(ARQUIVO_TEMP_COMPRA, "wb");
    if(tempComp != NULL) fclose(tempComp);
}

void mostrarDados(){
    int opcao = mostrarMenuArquivo();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            mostrarMensagem(lerCompra(&bufferCompra));

            mostrarDadosCompra(bufferCompra, QUANTIDADE_REGISTRO);

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            mostrarMensagem(lerProduto(&bufferProduto));

            mostrarDadosProduto(bufferProduto, QUANTIDADE_REGISTRO);

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }
}

void consultarDadosId(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo do produto que sera procurado: "});
    int produto = escolherProduto();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambda que extrai o codigo do produto da compra*/
            auto extrairId = [](const ESTRUTURA_COMPRA *r) {return r->product_id;};

            mostrarMensagem(pesquisaBinaria<ESTRUTURA_COMPRA>(ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, produto, 0, -1, extrairId, &bufferCompra));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambda que extrai o codigo do produto */
            auto extrairId = [](const ESTRUTURA_PRODUTO *r) {return r->product_id;};

            mostrarMensagem(pesquisaBinaria<ESTRUTURA_PRODUTO>(ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, produto, 0, -1, extrairId, &bufferProduto));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }
}

void consultarDadosSequencial(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo sequencial que sera procurado: "});
    int numero = escolherProduto();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambda que extrai o numero sequencial da compra */
            auto extrairSeqCompra = [](const ESTRUTURA_COMPRA *r) -> int { 
                return r->numeracao; 
            };

            mostrarMensagem(pesquisaBinaria<ESTRUTURA_COMPRA>(ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, numero, 0, -1, extrairSeqCompra, &bufferCompra));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambda que extrai o numero sequencial do produto */
            auto extrairSeqProduto = [](const ESTRUTURA_PRODUTO *r) -> int { 
                return r->numeracao; 
            };

            mostrarMensagem(pesquisaBinaria<ESTRUTURA_PRODUTO>(ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, numero, 0, -1, extrairSeqProduto, &bufferProduto));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }    
}

void consultarDadosIndiceId(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo do produto que sera procurado: "});
    int produto = escolherProduto();   

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambdas para extrair os limites de ID do bloco de indice e o ID da compra */
            auto minBlocoId = [](const INDICE_COMPRA *b) -> int { return b->primeiro_produto_id; };
            auto maxBlocoId = [](const INDICE_COMPRA *b) -> int { return b->ultimo_produto_id; };
            auto extrairId  = [](const ESTRUTURA_COMPRA *r) -> int { return r->product_id; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
                ARQUIVO_COMPRA,
                ARQUIVO_INDICE_COMPRA,
                ARQUIVO_TEMP_COMPRA,
                produto,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferCompra
            ));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambdas para extrair os limites de ID do bloco de indice e o ID do produto */
            auto minBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->primeiro_produto_id; };
            auto maxBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->ultimo_produto_id; };
            auto extrairId  = [](const ESTRUTURA_PRODUTO *r) -> int { return r->product_id; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
                ARQUIVO_PRODUTO,
                ARQUIVO_INDICE_PRODUTO,
                ARQUIVO_TEMP_PRODUTO,
                produto,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferProduto
            ));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }
}

void consultarDadosIndiceSequencial(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o numero do produto que sera procurado: "});
    int numero = escolherProduto();   

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao da compra */
            auto minBlocoId = [](const INDICE_COMPRA *b) -> int { return b->primeiro_numeracao; };
            auto maxBlocoId = [](const INDICE_COMPRA *b) -> int { return b->ultimo_numeracao; };
            auto extrairId  = [](const ESTRUTURA_COMPRA *r) -> int { return r->numeracao; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
                ARQUIVO_COMPRA,
                ARQUIVO_INDICE_COMPRA,
                ARQUIVO_TEMP_COMPRA,
                numero,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferCompra
            ));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao do produto */
            auto minBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->primeiro_numeracao; };
            auto maxBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->ultimo_numeracao; };
            auto extrairId  = [](const ESTRUTURA_PRODUTO *r) -> int { return r->numeracao; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
                ARQUIVO_PRODUTO,
                ARQUIVO_INDICE_PRODUTO,
                ARQUIVO_TEMP_PRODUTO,
                numero,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferProduto
            ));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }    
}

void inserirRegistro(){
int opcao = mostrarMenuArquivo();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA novaCompra = {};

            printf("Digite o ID do produto (product_id): ");
            scanf("%d", &novaCompra.product_id);

            printf("Digite o preco: ");
            scanf("%f", &novaCompra.price);

            printf("Digite a marca (brand): ");
            scanf("%19s", novaCompra.brand);

            printf("Digite a categoria (category_code): ");
            scanf("%49s", novaCompra.category_code);

            mostrarMensagem(inserirRegistro<ESTRUTURA_COMPRA>(
                ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, novaCompra
            ));
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO novoProduto = {};

            printf("Digite o ID do produto (product_id): ");
            scanf("%d", &novoProduto.product_id);

            printf("Digite a marca (brand): ");
            scanf("%19s", novoProduto.brand);

            printf("Digite a categoria (category_code): ");
            scanf("%49s", novoProduto.category_code);

            mostrarMensagem(inserirRegistro<ESTRUTURA_PRODUTO>(
                ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, novoProduto
            ));
            break;
        }

        default:
            mostrarMensagem({0, "Opcao nao encontrada."});
            break;
    }
}

void excluirRegistroId(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo do produto que sera excluido: "});
    int produto = escolherProduto();
    
    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao da compra */
            auto minBlocoId = [](const INDICE_COMPRA *b) -> int { return b->primeiro_produto_id; };
            auto maxBlocoId = [](const INDICE_COMPRA *b) -> int { return b->ultimo_produto_id; };
            auto extrairId  = [](const ESTRUTURA_COMPRA *r) -> int { return r->product_id; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
                ARQUIVO_COMPRA,
                ARQUIVO_INDICE_COMPRA,
                ARQUIVO_TEMP_COMPRA,
                produto,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferCompra
            ));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
                mostrarMensagem(excluirRegistro(ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, bufferCompra));
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao do produto */
            auto minBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->primeiro_produto_id; };
            auto maxBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->ultimo_produto_id; };
            auto extrairId  = [](const ESTRUTURA_PRODUTO *r) -> int { return r->product_id; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
                ARQUIVO_PRODUTO,
                ARQUIVO_INDICE_PRODUTO,
                ARQUIVO_TEMP_PRODUTO,
                produto,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferProduto
            ));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
                mostrarMensagem(excluirRegistro(ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, bufferProduto));
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }    
}

void excluirRegistroSequencial(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo do produto que sera excluido: "});
    int numero = escolherProduto();
    
    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao da compra */
            auto minBlocoId = [](const INDICE_COMPRA *b) -> int { return b->primeiro_numeracao; };
            auto maxBlocoId = [](const INDICE_COMPRA *b) -> int { return b->ultimo_numeracao; };
            auto extrairId  = [](const ESTRUTURA_COMPRA *r) -> int { return r->numeracao; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
                ARQUIVO_COMPRA,
                ARQUIVO_INDICE_COMPRA,
                ARQUIVO_TEMP_COMPRA,
                numero,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferCompra
            ));

            if(bufferCompra != NULL){
                mostrarDadosCompra(bufferCompra, 1);
                mostrarMensagem(excluirRegistro(ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, bufferCompra));
            }

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            /* Lambdas para extrair os limites de numeracao do bloco de indice e a numeracao do produto */
            auto minBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->primeiro_numeracao; };
            auto maxBlocoId = [](const INDICE_PRODUTO *b) -> int { return b->ultimo_numeracao; };
            auto extrairId  = [](const ESTRUTURA_PRODUTO *r) -> int { return r->numeracao; };

            mostrarMensagem(pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
                ARQUIVO_PRODUTO,
                ARQUIVO_INDICE_PRODUTO,
                ARQUIVO_TEMP_PRODUTO,
                numero,
                minBlocoId,
                maxBlocoId,
                extrairId,
                &bufferProduto
            ));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
                mostrarMensagem(excluirRegistro(ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, bufferProduto));
            }

            free(bufferProduto);
            break;
        }

        default:{
            mostrarMensagem({0, "Opcao nao encontrada."});
        }
    }    
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    while(1){
        int opcao = mostrarMenuInicial();

        switch (opcao){
            case 1:{
                inserirDadosIniciais();
                break;
            }

            case 2:{
                mostrarDados();
                break;
            }

            case 3:{
                consultarDadosId();
                break;
            }

            case 4:{
                consultarDadosSequencial();
                break;
            }

            case 5:{
                consultarDadosIndiceId();
                break;
            }

            case 6:{
                consultarDadosIndiceSequencial();
                break;
            }

            case 7:{
                inserirRegistro();
                break;
            }

            case 8:{
                excluirRegistroId();
                break;
            }

            case 9:{
                excluirRegistroSequencial();
                break;
            }

            case 0:{
                exit(0);
            }

            default:{
                mostrarMensagem({0, "Opcao nao encontrada."});
            }
        }
    }
}
