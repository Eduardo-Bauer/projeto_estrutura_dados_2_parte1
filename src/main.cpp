#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils/constantes.h"
#include "io/leitor.h"
#include "io/escritor.h"

#define PADRAO    "\x1b[0m"
#define NEGRITO   "\x1b[1m"
#define BRANCO    "\x1b[37m"
#define VERDE     "\x1b[32m"
#define VERMELHO  "\x1b[31m"
#define AZUL      "\x1b[34m"

int mostrarDados(){
    return 1;
}

int inserirDadosIniciais(){
    printf("criando buffer dos dados do dataset\n");

    ESTRUTURA_DATASET *buffer = lerDataset();
    if(buffer == NULL){
        printf("Erro: falha ao preencher o buffer.\n");
        return 0;
    }
    
    printf("gerando arquivo das compras realizadas no e-commerce\n");

    if(!preencherArquivoCompra(buffer)){
        return 0;
    }

    printf("gerando arquivo dos produtos do e-commerce\n");

    if(!preencherArquivoProduto(buffer)){
        return 0;
    }

    free(buffer);
    buffer = NULL;

    return 1;
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int opcao = 0;

    while(1){
        printf(AZUL "╔═════════════════════════════════════════════╗\n");
        printf(AZUL "║" PADRAO NEGRITO BRANCO "   arquivos de dados com índices parciais    " PADRAO AZUL "║\n");
        printf(AZUL "╚═════════════════════════════════════════════╝\n" PADRAO);

        printf(AZUL "║" NEGRITO " Escolha uma das opções abaixo:              " PADRAO  AZUL "║\n");
        printf(AZUL "║" PADRAO " [1] - Inserir os dados iniciais             "          AZUL "║\n"
               AZUL "║" PADRAO " [2] - Mostrar os dados (parcial ou total)   "          AZUL "║\n"
               AZUL "║" PADRAO " [3] - Consultar dado por pesquisa binaria   "          AZUL "║\n"
               AZUL "║" PADRAO " [4] - Consultar dados a partir de um índice "          AZUL "║\n"
               AZUL "║" PADRAO " [5] - Inserir dados                         "          AZUL "║\n"
               AZUL "║" PADRAO " [6] - Remover dados                         "          AZUL "║\n"
               AZUL "║" PADRAO " [0] - Fechar programa                       "          AZUL "║\n\n" PADRAO);

        printf(AZUL "Digite sua escolha: " PADRAO);
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                printf("iniciando a inserção dos dados iniciais\n");

                if(inserirDadosIniciais()){
                    printf(VERDE "arquivos iniciais preenchidos com sucesso!\n" PADRAO);

                }else{
                    printf(VERMELHO "falha ao inserir dados iniciais!\n" PADRAO);
                }

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
                printf(AZUL "Programa finalizado\n" PADRAO);
                exit(0);
                break;
        }
    }
}