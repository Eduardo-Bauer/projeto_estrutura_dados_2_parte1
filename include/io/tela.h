#ifndef TELA_H
#define TELA_H

/* Definicoes das corres para as saidas. */
#define PADRAO    "\x1b[0m"
#define NEGRITO   "\x1b[1m"
#define BRANCO    "\x1b[37m"
#define VERDE     "\x1b[32m"
#define VERMELHO  "\x1b[31m"
#define AZUL      "\x1b[34m"

#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"

/**
 * @brief       Mostra na tela o menu inicial.
 * 
 * @return int  Retorna a opção que o usuário escolheu no menu.
 */
int mostrarMenuInicial();

/**
 * @brief       Mostra na tela o menu de escolha de arquivo.
 * 
 * @return int  Retorna a opção que o usuário escolheu no menu.
 */
int mostrarMenuArquivo();

/**
 * @brief           Mostra a mensagem retornada pelas funcoes na tela.
 * 
 * @param mensagem  Mensagem recebida pelas funcoes {numero, "texto"}.
 * 
 * @return void     Retorna nada.
 */
void mostrarMensagem(RESULTADO mensagem);

/**
 * @brief               Mostra os dados das compras registradas.
 * 
 * @param registro      Ponteiro para o buffer das compras.
 * @param quantidade    quantidade de registros para mostrar.
 * 
 * @return void         Retorna nada.
 */
void mostrarDadosCompra(ESTRUTURA_COMPRA const *registro, int const quantidade);

/**
 * @brief               Mostra os dados dos produtos registradas.
 * 
 * @param arquivo       Ponteiro para o buffer dos produtos.
 * @param quantidade    quantidade de registros para mostrar.
 * 
 * @return void         Retorna nada.
 */
void mostrarDadosProduto(ESTRUTURA_PRODUTO const *registro, int const quantidade);

#endif