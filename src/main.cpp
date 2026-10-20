#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils/constantes.h"
#include "utils/arquivo.h"
#include "models/status.h"
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

    mostrarMensagem(preencherArquivoCompra(buffer));

    mostrarMensagem({2, "Gerando arquivo dos indices."});

    mostrarMensagem(escreverIndiceProduto());

    mostrarMensagem(escreverIndiceCompra());

    free(buffer);
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

        default:
            mostrarMensagem({0, "Opcao nao encontrada."});
            break;
    }
}

void consultarDados(){
    int opcao = mostrarMenuArquivo();

    mostrarMensagem({2, "Digite o codigo do produto que sera procurado: "});
    int produto = escolherProduto();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            mostrarMensagem(pesquisaBinariaCompra(&bufferCompra, produto));

            mostrarDadosCompra(bufferCompra, 1);

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            mostrarMensagem(pesquisaBinariaProduto(&bufferProduto, produto));

            if(bufferProduto != NULL){
                mostrarDadosProduto(bufferProduto, 1);
            }

            free(bufferProduto);
            break;
        }

        default:
            mostrarMensagem({0, "Opcao nao encontrada."});
            break;
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
                consultarDados();
                break;
            }

            case 4:{
                // ver se vai dar tempo para emplementar
                break;
            }

            case 5:{

                break;
            }

            case 6:{

                break;
            }

            case 7:{

                break;
            }

            case 0:
                exit(0);
        }
    }
}
