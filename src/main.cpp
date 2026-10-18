#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils/constantes.h"
#include "models/status.h"
#include "io/leitor.h"
#include "io/escritor.h"
#include "io/tela.h"

int mostrarDados(){
    return 1;
}

int inserirDadosIniciais(){
    mostrarMensagem({2, "Criando buffer dos dados do dataset."});

    ESTRUTURA_DATASET *buffer = NULL;

    mostrarMensagem(lerDataset(&buffer));

    mostrarMensagem({2, "Gerando arquivo das compras realizadas no e-commerce."});

    mostrarMensagem(preencherArquivoCompra(buffer));

    mostrarMensagem({2, "Gerando arquivo dos produtos do e-commerce."});

    mostrarMensagem(preencherArquivoCompra(buffer));

    free(buffer);

    return 1;
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    while(1){
        int opcao = mostrarMenuInicial();

        switch (opcao){
            case 1:
                mostrarMensagem({2, "Iniciando a insercao dos dados iniciais."});
                
                inserirDadosIniciais();

                break;

            case 2:
                mostrarDados();
                break;

            case 3:

                break;

            case 4:

                break;

            case 5:

                break;

            case 6:

                break;

            case 0:
                exit(0);
                break;
        }
    }
}