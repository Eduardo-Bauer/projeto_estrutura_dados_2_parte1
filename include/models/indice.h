#ifndef INDICE_H
#define INDICE_H

#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"

/* Estrutura existente dentro do arquivo do indice. */
typedef struct indice_produto{
    int primeiro_numeracao;
    int primeiro_produto_id;
    int ultimo_numeracao;
    int ultimo_produto_id;
}INDICE_PRODUTO;

/* Estrutura existente dentro do arquivo do indice. */
typedef struct indice_compra{
    int primeiro_numeracao;
    int primeiro_produto_id;
    int ultimo_numeracao;
    int ultimo_produto_id;
}INDICE_COMPRA;

#endif