#ifndef ARQUIVO_COMPRA_H
#define ARQUIVO_COMPRA_H

/* Estrutura interna do arquivo referente as compras realizadas no e-commerce. */
typedef struct estrutura_compra{
    int numeracao;
    int product_id;
    int elo;
    int excluido;
    float price;
    char brand[20];
    char category_code[50];
}ESTRUTURA_COMPRA;

#endif