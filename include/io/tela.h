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

/**
 * @brief       Mostra na tela o menu inicial.
 * 
 * @return int  Retorna a opção que o usuário escolheu no menu.
 */
int mostrarMenuInicial();

/**
 * @brief           Mostra a mensagem retornada pelas funções na tela.
 * 
 * @param mensagem  Mensagem recebida pelas funções {numero, "texto"}.
 * 
 * @return void     Retorna nada.
 */
void mostrarMensagem(RESULTADO mensagem);

#endif