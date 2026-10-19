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

void mostrarDados(){
    int opcao = mostrarMenuArquivo();

    switch (opcao){
        case 1: {
            ESTRUTURA_COMPRA *bufferCompra = NULL;

            mostrarMensagem(lerCompra(&bufferCompra));

            mostrarDadosCompra(bufferCompra);

            free(bufferCompra);
            break;
        }

        case 2: {
            ESTRUTURA_PRODUTO *bufferProduto = NULL;

            mostrarMensagem(lerProduto(&bufferProduto));

            mostrarDadosProduto(bufferProduto);

            free(bufferProduto);
            break;
        }

        default:
            mostrarMensagem({0, "Opcao nao encontrada."});
            break;
    }
}

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

                break;
            }

            case 4:{

                break;
            }

            case 5:{

                break;
            }

            case 6:{

                break;
            }

            case 0:
                exit(0);
        }
    }
}
