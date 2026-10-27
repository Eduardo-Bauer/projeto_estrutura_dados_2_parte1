#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Nome do arquivo dataset. */
#define DATASET "src/data/dataset.bin"

/* Nome do arquivo com os dados das conpras realizadas. */
#define ARQUIVO_COMPRA "src/data/compras.bin"

/* Nome do arquivo com os dados das conpras realizadas. */
#define ARQUIVO_PRODUTO "src/data/produtos.bin"

/* Nome do arquivo indice dos produtos. */
#define ARQUIVO_INDICE_PRODUTO "src/data/indiceProduto.bin"

/* Nome do arquivo indice das compras. */
#define ARQUIVO_INDICE_COMPRA "src/data/indiceCompra.bin"

/* Nome do arquivo dos produtos recem adicionados. */
#define ARQUIVO_TEMP_PRODUTO "src/data/temp/produtos_temp.bin"

/* Nome do arquivo das compras recem adicionados. */
#define ARQUIVO_TEMP_COMPRA  "src/data/temp/compras_temp.bin"

/* Tamanho para a criacao do buffer. */
#define TAMANHO_BUFFER 65000000UL

/* Quantidade de registros que serao lidos. */
#define QUANTIDADE_REGISTRO 1000

/* Quantidade de registros dentro de um bloco do indice. */
#define QUANTIDADE_REGISTRO_INDICE 10000UL

#endif