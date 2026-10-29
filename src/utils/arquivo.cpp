#include <stdio.h>
#include <stdlib.h>
#include "utils/arquivo.h"
#include "models/status.h"

FILE *abrirArquivo(const char *nomeArquivo, const char *modoAbertura){
    /* Ponteiro para o arquivo que sera aberto */
    FILE *arquivo = fopen(nomeArquivo, modoAbertura);

    return arquivo;
}
