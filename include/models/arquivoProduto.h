#ifndef ARQUIVO_PRODUTO_H
#define ARQUIVO_PRODUTO_H

/* Estrutura interna do arquivo referente aos produtos do e-commerce. */
typedef struct estrutura_produto{
    int numeracao;
    int product_id;
    int elo;
    int excluido;
    char brand[20];
    char category_code[50];
}ESTRUTURA_PRODUTO;

#endif