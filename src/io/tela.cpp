#include <stdio.h>
#include "io/tela.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"

int mostrarMenuArquivo(){
    int opcao = 0;

    printf(AZUL "╔═════════════════════════════════════════════╗\n");
    printf(AZUL "║" PADRAO NEGRITO BRANCO "          arquivos para selecionar           " PADRAO AZUL "║\n");
    printf(AZUL "╚═════════════════════════════════════════════╝\n" PADRAO);

    printf(AZUL "║" NEGRITO " Escolha uma das opções abaixo:              " PADRAO  AZUL "║\n");
    printf(AZUL "║" PADRAO " [1] - Arquivo das compras                    "          AZUL "║\n"
           AZUL "║" PADRAO " [2] - Arquivo dos produtos                   "          AZUL "║\n" PADRAO);

    printf(AZUL "Digite sua escolha: " PADRAO);
    scanf("%d", &opcao);

    return opcao;
}

int mostrarMenuInicial(){
    int opcao = 0;

    printf(AZUL "╔═════════════════════════════════════════════╗\n");
    printf(AZUL "║" PADRAO NEGRITO BRANCO "   arquivos de dados com indices parciais    " PADRAO AZUL "║\n");
    printf(AZUL "╚═════════════════════════════════════════════╝\n" PADRAO);

    printf(AZUL "║" NEGRITO " Escolha uma das opções abaixo:              " PADRAO  AZUL "║\n");
    printf(AZUL "║" PADRAO " [1] - Inserir os dados iniciais             "          AZUL "║\n"
           AZUL "║" PADRAO " [2] - Mostrar os dados parcial              "          AZUL "║\n"
           AZUL "║" PADRAO " [3] - Consultar dado por pesquisa binaria   "          AZUL "║\n"
           AZUL "║" PADRAO " [4] - Consultar dado por sequencia          "          AZUL "║\n"
           AZUL "║" PADRAO " [5] - Consultar dados a partir do indice    "          AZUL "║\n"
           AZUL "║" PADRAO " [6] - Consultar dado por sequencia          "          AZUL "║\n"
           AZUL "║" PADRAO " [7] - Inserir dados                         "          AZUL "║\n"
           AZUL "║" PADRAO " [8] - Remover dados por codigo              "          AZUL "║\n"
           AZUL "║" PADRAO " [9] - Remover dados por sequencia           "          AZUL "║\n"
           AZUL "║" PADRAO " [0] - Fechar programa                       "          AZUL "║\n\n" PADRAO);

    printf(AZUL "Digite sua escolha: " PADRAO);
    scanf("%d", &opcao);

    return opcao;
}

void mostrarMensagem(RESULTADO mensagem) {
    if (mensagem.sucesso == 1) {
        printf(VERDE "%s\n" PADRAO, mensagem.mensagem);

    } else if (mensagem.sucesso == 0) {
        printf(VERMELHO "%s\n" PADRAO, mensagem.mensagem);

    } else {
        printf("%s\n", mensagem.mensagem);
    }
}

void mostrarDadosCompra(ESTRUTURA_COMPRA const *registro, int const quantidade){
    for(int i = 0; i < quantidade; i++){
        if(registro[i].product_id != 0){
            printf("numeracao: %d | "
                   "produto: %d | "
                   "preco: %.2f | "
                   "marca: %s\n", 
                   registro[i].numeracao, 
                   registro[i].product_id, 
                   registro[i].price, 
                   registro[i].brand);
        }else{
            return;
        }
    }
}

void mostrarDadosProduto(ESTRUTURA_PRODUTO const *registro, int const quantidade){
    for(int i = 0; i < quantidade; i++){
        if(registro[i].product_id != 0){
            printf("numeracao: %d | "
                   "produto: %d | "
                   "marca: %s\n", 
                    registro[i].numeracao, 
                    registro[i].product_id,
                    registro[i].brand);
        }else{
            return;
        }
    }
}
