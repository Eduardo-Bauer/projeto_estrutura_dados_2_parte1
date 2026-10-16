#include <stdio.h>
#include <stdlib.h>
#include "utils/arquivo.h"
#include "models/arquivoProduto.h"
#include "utils/constantes.h"

int consultarProdutos(){
    int ultimoId = -1;
    
    FILE *arquivo = abrirArquivo(ARQUIVO_PRODUTO, "rb");
    if(arquivo == NULL){
        return 0;
    }

    ESTRUTURA_PRODUTO *produto = (ESTRUTURA_PRODUTO *)malloc(sizeof(ESTRUTURA_PRODUTO));
    if(produto == NULL){
        fclose(arquivo);
        return 0;
    }

    while(fread(produto, sizeof(ESTRUTURA_PRODUTO), 1, arquivo) != NULL){
        if(produto->product_id == ultimoId){
            free(produto);
            fclose(arquivo);
            return 0;

        }else{
            ultimoId = produto->product_id;
        }
    }

    return 1;

    free(produto);
    fclose(arquivo);
}

int main(){
    printf("teste se tem produto repetido\n");

    if(consultarProdutos()){
        printf("todos os produtos sao diferentes\n");

    }else{
        printf("erro ou produtos iguais\n");
    }
}