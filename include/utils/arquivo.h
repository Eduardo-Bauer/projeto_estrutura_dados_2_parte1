#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

/**
 * @brief               Abre o arquivo solicitado pelo input.
 * 
 * Se não puder abrir o arquivo, imprime uma mensagem de erro 
 * e encerra a execução do programa (exit code 1).
 * 
 * @param nomeArquivo   Caminho para o ficheiro a abrir.
 * @param modoAbertura  Modo de acesso (ex.: "rb", "wb", "r+b").
 * 
 * @return FILE*        Ponteiro válido para o arquivo aberto.
 */
FILE *abrirArquivo(const char *nomeArquivo, const char *modoAbertura);

#endif