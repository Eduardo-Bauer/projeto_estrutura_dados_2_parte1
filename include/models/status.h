#ifndef STATUS_H
#define STATUS_H

/* Estrutura que padroniza os retornos de erro e sucesso no sistema. */
typedef struct resultado{
    int sucesso;          // 1 para sucesso, 0 para erro
    const char *mensagem; // Ponteiro para o texto
} RESULTADO;

#endif