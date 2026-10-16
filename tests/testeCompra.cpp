#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models/arquivoCompra.h"
#include "utils/constantes.h"
#include "utils/arquivo.h"

int consultarCompras(){
    int i = 0;

    FILE *arquivo = abrirArquivo(ARQUIVO_COMPRA, "rb");
    if(arquivo == NULL){
        return 0;
    }

    ESTRUTURA_COMPRA *compra = (ESTRUTURA_COMPRA *)malloc(sizeof(ESTRUTURA_COMPRA));
    if(compra == NULL){
        fclose(arquivo);
        return 0;
    }

    while(fread(compra, sizeof(ESTRUTURA_COMPRA), 1, arquivo) != NULL){
        if(i < 100){
            printf("%d | %d | %s | %.2f \n", compra->numeracao, compra->product_id, compra->brand, compra->price);
            i++;
            
        }else{
            break;
        }
    }

    free(compra);
    fclose(arquivo);
    return 1;
}

int main(){
    printf("teste se só tem compras\n");

    if(consultarCompras()){
        printf("tudo certo\n");

    } else{
        printf("algo deu errado\n");
    }
}