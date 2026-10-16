#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/constantes.h"

int testarBuffer(ESTRUTURA_DATASET *buffer){
    for(int i = 0; i < 200; i++){
        if(buffer[i].product_id != NULL){
            printf("codigo produto: %d | tempo: %s | preco: %.2f\n", buffer[i].product_id, buffer[i].event_time, buffer[i].price);

        }else{
            return 0;
        }
    }

    return 1;
}

int main(){
    printf("realizando a implementacao do buffer\n");
    ESTRUTURA_DATASET *buffer = lerDataset();

    printf("testando buffer\n");
    printf("resultado: %d\n", testarBuffer(buffer));

    free(buffer);
}