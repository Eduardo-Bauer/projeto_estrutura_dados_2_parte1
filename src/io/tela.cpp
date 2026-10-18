#include <stdio.h>
#include "io/tela.h"
#include "models/status.h"

#define PADRAO    "\x1b[0m"
#define NEGRITO   "\x1b[1m"
#define BRANCO    "\x1b[37m"
#define VERDE     "\x1b[32m"
#define VERMELHO  "\x1b[31m"
#define AZUL      "\x1b[34m"

int mostrarMenuInicial(){
    int opcao = 0;

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