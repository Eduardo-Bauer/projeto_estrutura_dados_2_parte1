#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/arquivo.h"
#include "models/dataset.h"
#include "models/status.h"
#include "utils/constantes.h"

RESULTADO lerDataset(ESTRUTURA_DATASET **bufferSaida){
    /* malloc na memoria para os dados do dataset */
    ESTRUTURA_DATASET *buffer = (ESTRUTURA_DATASET*)malloc(sizeof(ESTRUTURA_DATASET) * TAMANHO_BUFFER);
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo dataset */
    FILE *dataset = abrirArquivo(DATASET, "rb");
    if(dataset == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo dataset."};
    }

    if(!fread(buffer, sizeof(ESTRUTURA_DATASET), TAMANHO_BUFFER, dataset)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum arquivo do dataset."};
    };

    fclose(dataset);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar o dadaset na memoria."};
}