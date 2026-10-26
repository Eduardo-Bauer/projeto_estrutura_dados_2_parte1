#include <stdio.h>
#include <stdlib.h>
#include "models/arquivoProduto.h"
#include "models/indice.h"
#include "utils/constantes.h"

void testarIndiceProduto(){
    FILE *arquivo = fopen(ARQUIVO_INDICE_PRODUTO, "rb");
    if(arquivo == NULL){
        printf("nao foi possivel acessar o arquivo\n");
        return;
    }

    INDICE_COMPRA *indice = (INDICE_COMPRA*)malloc(sizeof(INDICE_COMPRA));
    if(indice == NULL){
        fclose(arquivo);
        printf("nao foi possivel criar o indice\n");
        return;
    }

    if(!fread(indice, sizeof(INDICE_COMPRA), 1, arquivo)){
        printf("nao leu nada\n");
        return;
    }

    printf("numeracao: %d | "
            "produto: %d |\n", 
            indice->primeiro_numeracao, 
            indice->primeiro_produto_id);

    printf("numeracao: %d | "
            "produto: %d |\n", 
            indice->ultimo_numeracao, 
            indice->ultimo_produto_id);

    free(indice);

    fclose(arquivo);
}

void testarIndiceCompra(){
    FILE *arquivo = fopen(ARQUIVO_INDICE_COMPRA, "rb");
    if(arquivo == NULL){
        printf("nao foi possivel acessar o arquivo\n");
        return;
    }

    INDICE_COMPRA *indice = (INDICE_COMPRA*)malloc(sizeof(INDICE_COMPRA));
    if(indice == NULL){
        fclose(arquivo);
        printf("nao foi possivel criar o indice\n");
        return;
    }

    if(!fread(indice, sizeof(INDICE_COMPRA), 1, arquivo)){
        printf("nao leu nada\n");
        return;
    }

    printf("numeracao: %d | "
            "produto: %d |\n", 
            indice->primeiro_numeracao, 
            indice->primeiro_produto_id);

    printf("numeracao: %d | "
            "produto: %d |\n", 
            indice->ultimo_numeracao, 
            indice->ultimo_produto_id);

    free(indice);

    fclose(arquivo);
}

int main(){
    testarIndiceProduto();

    testarIndiceCompra();
}