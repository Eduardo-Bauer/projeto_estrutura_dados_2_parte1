#include <stdio.h>
#include <stdlib.h>
#include "utils/arquivo.h"

FILE *abrirArquivo(const char *nomeArquivo, const char *modoAbertura){
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);
    if(arquivo == NULL){
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }

    return arquivo;
}
