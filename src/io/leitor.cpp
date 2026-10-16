#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/arquivo.h"
#include "models/dataset.h"
#include "models/arquivoCompra.h"
#include "utils/constantes.h"

ESTRUTURA_DATASET* lerDataset(){
    /* malloc na memoria para os dados do dataset */
    ESTRUTURA_DATASET *buffer = (ESTRUTURA_DATASET*)malloc(sizeof(ESTRUTURA_DATASET) * TAMANHO_BUFFER);
    if(buffer == NULL){
        printf("Erro: Memoria insuficiente para o buffer.\n");
        exit(1);
    }

    /* ponteiro para o arquivo dataset */
    FILE *dataset = abrirArquivo(DATASET, "rb");

    fread(buffer, sizeof(ESTRUTURA_DATASET), TAMANHO_BUFFER, dataset);

    fclose(dataset);

    return buffer;
}