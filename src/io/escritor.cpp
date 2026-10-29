#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/escritor.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/dataset.h"
#include "models/indice.h"
#include "utils/constantes.h"
#include "utils/arquivo.h"

void escreverProduto(FILE *arquivoProduto, const ESTRUTURA_PRODUTO *produto){
    fwrite(produto, sizeof(ESTRUTURA_PRODUTO), 1, arquivoProduto);
}

void converterEstruturaProduto(const ESTRUTURA_DATASET *registro, ESTRUTURA_PRODUTO *produto, unsigned long numeracao){
    produto->numeracao = numeracao;
    produto->product_id = registro->product_id;
    produto->elo = -1;
    produto->excluido = 0;
    
    strncpy(produto->brand, registro->brand, sizeof(produto->brand) - 1);
    produto->brand[sizeof(produto->brand) - 1] = '\0';

    strncpy(produto->category_code, registro->category_code, sizeof(produto->category_code) - 1);
    produto->category_code[sizeof(produto->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoProduto(const ESTRUTURA_DATASET *buffer){
    /* Armazena o ID do ultimo produto inserido para evitar duplicatas */
    int ultimoProduto = 0;

    /* 1. Aloca memoria para o registro do produto que sera preenchido */
    /* Registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_PRODUTO *produto = (ESTRUTURA_PRODUTO*)calloc(1, sizeof(ESTRUTURA_PRODUTO));
    if(produto == NULL){
        return {0, "Erro: Memoria insuficiente para o produto."};
    }
    
    /* 2. Abre o arquivo binario de produtos para escrita */
    /* Ponteiro para o arquivo dos produtos do e-commerce */
    FILE *arquivoProduto = abrirArquivo(ARQUIVO_PRODUTO, "wb");
    if(arquivoProduto == NULL){
        free(produto);
        return {0, "Erro: Falha ao abrir o arquivo dos produtos."};
    }

    /* 3. Percorre o buffer do dataset filtrando produtos unicos e ordenados */
    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        /* 3.1. Se o ID for maior que o ultimo gravado, converte e escreve no arquivo */
        if(buffer[i].product_id > ultimoProduto){
            converterEstruturaProduto(&buffer[i], produto, numeracao++);
            escreverProduto(arquivoProduto, produto);
            ultimoProduto = buffer[i].product_id;
        }
    }

    fclose(arquivoProduto);
    free(produto);

    return {1, "Sucesso ao preencher o arquivo dos produtos."};
}

void escreverCompra(FILE *arquivoCompra, const ESTRUTURA_COMPRA *compra){
    fwrite(compra, sizeof(ESTRUTURA_COMPRA), 1, arquivoCompra);
}

void converterEstruturaCompra(const ESTRUTURA_DATASET *registro, ESTRUTURA_COMPRA *compra, unsigned long numeracao) {
    compra->numeracao = numeracao;
    compra->price = registro->price;
    compra->product_id = registro->product_id;
    compra->elo = -1;
    compra->excluido = 0;
    
    strncpy(compra->brand, registro->brand, sizeof(compra->brand) - 1);
    compra->brand[sizeof(compra->brand) - 1] = '\0';

    strncpy(compra->category_code, registro->category_code, sizeof(compra->category_code) - 1);
    compra->category_code[sizeof(compra->category_code) - 1] = '\0';
}

RESULTADO preencherArquivoCompra(const ESTRUTURA_DATASET *buffer){
    /* 1. Aloca memoria para o registro da compra que sera preenchido */
    /* Registro do dataset que sera convertido para uma estrutura menor */
    ESTRUTURA_COMPRA *compra = (ESTRUTURA_COMPRA*)calloc(1, sizeof(ESTRUTURA_COMPRA));
    if(compra == NULL){
        return {0, "Erro: Memoria insuficiente para a compra."};
    }
    
    /* 2. Abre o arquivo binario de compras para escrita */
    /* Ponteiro para o arquivo das compras realizadas no e-commerce */
    FILE *arquivoCompra = abrirArquivo(ARQUIVO_COMPRA, "wb");
    if(arquivoCompra == NULL){
        free(compra);
        return {0, "Erro: Falha ao abrir o arquivo das compras."};
    }

    /* 3. Percorre o buffer do dataset filtrando apenas eventos de compra efetivada */
    for(unsigned long i = 0, numeracao = 0; i < TAMANHO_BUFFER; i++){
        /* 3.1. Se o tipo do evento for "purchase", converte e escreve no arquivo */
        if(strcmp(buffer[i].event_type, "purchase") == 0){
            converterEstruturaCompra(&buffer[i], compra, numeracao++);
            escreverCompra(arquivoCompra, compra);
        }
    }

    fclose(arquivoCompra);
    free(compra);

    return {1, "Sucesso ao preencher o arquivo das compras."};
}

template <typename T, typename IndiceT>
RESULTADO escreverIndice(const char *nomeArquivoDados, const char *nomeArquivoIndice){
    /* Posicao do primeiro registro do bloco atual */
    int posicaoInicioBloco = 0;
    /* Posicao do ultimo registro do bloco atual */
    int posicaoFimBloco = 0;
    /* Quantidade total de registros presentes no arquivo de dados */
    int totalRegistros = 0;
    /* Registro do arquivo de dados (compra ou produto) */
    T registro;
    /* Estrutura do bloco de indice parcial */
    IndiceT indice;
   
    /* 1. Abre o arquivo de dados para leitura */
    /* Ponteiro para o arquivo de dados */
    FILE *arquivo = abrirArquivo(nomeArquivoDados, "rb");
    if(arquivo == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de dados."};
    }

    /* 2. Calcula o total de registros do arquivo para nao ultrapassar o fim no ultimo bloco */
    fseek(arquivo, 0, SEEK_END);
    totalRegistros = (int)(ftell(arquivo) / sizeof(T));
    if(totalRegistros == 0){
        fclose(arquivo);
        return {0, "Erro: Arquivo de dados vazio para gerar indice."};
    }
    fseek(arquivo, 0, SEEK_SET);

    /* 3. Abre o arquivo de indice para escrita binaria */
    /* Ponteiro para o arquivo do indice */
    FILE *arquivoIndice = abrirArquivo(nomeArquivoIndice, "wb");
    if(arquivoIndice == NULL){
        fclose(arquivo);
        return {0, "Erro: Falha ao abrir o arquivo de indice."};
    }

    /* 4. Percorre o arquivo de dados saltando de bloco em bloco para montar o indice parcial */
    while(posicaoInicioBloco < totalRegistros){
        /* 4.1. Le o primeiro registro do bloco e armazena suas chaves no indice */
        fseek(arquivo, sizeof(T) * posicaoInicioBloco, SEEK_SET);
        if(fread(&registro, sizeof(T), 1, arquivo) != 1){
            break;
        }

        indice.primeiro_numeracao = registro.numeracao;
        indice.primeiro_produto_id = registro.product_id;

        /* 4.2. Calcula a posicao do ultimo registro do bloco (ajustando caso seja o fim do arquivo) */
        posicaoFimBloco = posicaoInicioBloco + QUANTIDADE_REGISTRO_INDICE - 1;
        if(posicaoFimBloco >= totalRegistros){
            posicaoFimBloco = totalRegistros - 1;
        }

        /* 4.3. Le o ultimo registro do bloco e armazena suas chaves no indice */
        fseek(arquivo, sizeof(T) * posicaoFimBloco, SEEK_SET);
        if(fread(&registro, sizeof(T), 1, arquivo) != 1){
            break;
        }

        indice.ultimo_numeracao = registro.numeracao;
        indice.ultimo_produto_id = registro.product_id;

        /* 4.4. Grava o bloco preenchido no arquivo de indice e avanca para o proximo bloco */
        fwrite(&indice, sizeof(IndiceT), 1, arquivoIndice);

        /* avanca para o inicio do proximo bloco */
        posicaoInicioBloco = posicaoFimBloco + 1;
    }

    fclose(arquivo);
    fclose(arquivoIndice);

    return {1, "Sucesso ao criar arquivo de indice."};
}

/* Instanciacao explicita de escrever indice para permitir a chamada direto no main.cpp */
template RESULTADO escreverIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(const char*, const char*);
template RESULTADO escreverIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(const char*, const char*);

template <typename T>
RESULTADO excluirRegistro(const char *nomeArquivoDados, const char *nomeArquivoTemp, T *registro){
    /* Quantidade total de registros no arquivo de dados principal */
    int totalDados = 0;
    /* Deslocamento relativo do registro dentro do arquivo temporario */
    int posTemp = 0;        
    
    /* 1. Abre o arquivo principal de dados para leitura e escrita */
    /* Ponteiro para o arquivo de dados principal */
    FILE *arqDados = abrirArquivo(nomeArquivoDados, "rb+");
    if(arqDados == NULL){
        return {0, "Erro: Falha ao abrir o arquivo principal."};
    }

    /* 2. Calcula a quantidade total de registros no arquivo principal */
    fseek(arqDados, 0, SEEK_END);
    totalDados = (int)(ftell(arqDados) / sizeof(T));

    /* 3. Verifica se o registro pertence ao arquivo principal */
    if(registro->numeracao < totalDados){
        fseek(arqDados, sizeof(T) * registro->numeracao, SEEK_SET);
        registro->excluido = 1;
        fwrite(registro, sizeof(T), 1, arqDados);
        fclose(arqDados);
        return {1, "Registro excluido do arquivo principal com sucesso."};
    }

    fclose(arqDados);

    /* 4. Caso o registro tenha sido recem-adicionado, abre o arquivo temporario */
    /* Ponteiro para o arquivo temporario */
    FILE *arqTemp = abrirArquivo(nomeArquivoTemp, "rb+");
    if(arqTemp == NULL){
        return {0, "Erro: Falha ao abrir o arquivo temporario."};
    }

    /* 5. Calcula a posicao no arquivo temporario, marca como excluido e sobrescreve */
    posTemp = registro->numeracao - totalDados;

    fseek(arqTemp, sizeof(T) * posTemp, SEEK_SET);

    registro->excluido = 1;

    fwrite(registro, sizeof(T), 1, arqTemp);
    fclose(arqTemp);

    return {1, "Registro excluido do arquivo temporario com sucesso."};
}

/* Instanciacao explicita da exclusao de registros */
template RESULTADO excluirRegistro<ESTRUTURA_PRODUTO>(const char*, const char*, ESTRUTURA_PRODUTO*);
template RESULTADO excluirRegistro<ESTRUTURA_COMPRA>(const char*, const char*, ESTRUTURA_COMPRA*);

template <typename T>
RESULTADO inserirRegistro(const char *nomeArquivoDados, const char *nomeArquivoTemp, T novoRegistro){
    /* Primeiro registro do arquivo principal (posicao 0) */
    T primeiroItem;
    /* Registro antecessor localizado no arquivo principal */
    T itemPrincipal;
    /* Registro auxiliar para navegacao na pesquisa binaria */
    T itemAux;
    /* Registro auxiliar para percorrer a cadeia de elos no arquivo temporario */
    T itemTempAtual;
    /* Quantidade total de registros no arquivo principal */
    int totalDados = 0;
    /* Posicao onde o novo registro sera gravado no fim do arquivo temporario */
    int novaPosicaoTemp = 0;
    /* Limite inferior da busca binaria */
    int min = 0;
    /* Limite superior da busca binaria */
    int max = 0;
    /* Posicao do registro antecessor encontrada na busca binaria */
    int posAntecessor = 0;
    /* Indice do elo anterior na lista encadeada do arquivo temporario */
    int eloAnterior = -1;
    /* Indice do elo atual na lista encadeada do arquivo temporario */
    int eloAtual = -1;

    /* 1. Abre o arquivo principal de dados para leitura e escrita */
    /* Ponteiro para o arquivo de dados principal */
    FILE *arqDados = abrirArquivo(nomeArquivoDados, "rb+");
    if(arqDados == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de dados principal."};
    }

    /* 2. Abre o arquivo temporario para leitura e escrita */
    /* Ponteiro para o arquivo temporario */
    FILE *arqTemp = abrirArquivo(nomeArquivoTemp, "rb+");
    if(arqTemp == NULL){
        fclose(arqDados);
        return {0, "Erro: Falha ao abrir o arquivo temporario."};
    }

    /* 3. Calcula o total de registros do arquivo principal e verifica se nao esta vazio */
    fseek(arqDados, 0, SEEK_END);
    totalDados = (int)(ftell(arqDados) / sizeof(T));
    if(totalDados == 0){
        fclose(arqDados);
        fclose(arqTemp);
        return {0, "Erro: Arquivo principal vazio."};
    }

    /* 4. Calcula a nova posicao disponivel no final do arquivo temporario e inicializa flags */
    fseek(arqTemp, 0, SEEK_END);
    novaPosicaoTemp = (int)(ftell(arqTemp) / sizeof(T));

    novoRegistro.excluido = 0;
    novoRegistro.elo = -1;

    /* 5. Le o primeiro registro do arquivo principal (posicao 0) para verificar caso especial */
    fseek(arqDados, 0, SEEK_SET);
    fread(&primeiroItem, sizeof(T), 1, arqDados);

    /* 6. Caso especial: se o novo produto for menor que o primeiro registro (posicao 0) */
    if(novoRegistro.product_id < primeiroItem.product_id){
        /* 6.1. Move o antigo primeiro registro para o arquivo temporario mantendo seu elo */
        primeiroItem.numeracao = totalDados + novaPosicaoTemp;
        fseek(arqTemp, sizeof(T) * novaPosicaoTemp, SEEK_SET);
        fwrite(&primeiroItem, sizeof(T), 1, arqTemp);

        /* 6.2. Grava o novo registro na posicao 0 do arquivo principal apontando para o antigo no temporario */
        novoRegistro.numeracao = 0;
        novoRegistro.elo = novaPosicaoTemp;
        fseek(arqDados, 0, SEEK_SET);
        fwrite(&novoRegistro, sizeof(T), 1, arqDados);

        fclose(arqDados);
        fclose(arqTemp);
        return {1, "Registro inserido na primeira posicao (0) e vinculado por elo com sucesso."};
    }

    /* 7. Caso padrao: o novo produto e maior ou igual ao primeiro registro */
    novoRegistro.numeracao = totalDados + novaPosicaoTemp;
    min = 0;
    max = totalDados - 1;

    /* 7.1. Realiza busca binaria no arquivo principal para encontrar o registro antecessor */
    while(min <= max){
        int meio = min + (max - min) / 2;
        fseek(arqDados, sizeof(T) * meio, SEEK_SET);
        fread(&itemAux, sizeof(T), 1, arqDados);

        if(itemAux.product_id <= novoRegistro.product_id){
            posAntecessor = meio;
            min = meio + 1;
        } else {
            max = meio - 1;
        }
    }

    /* 7.2. Le o registro antecessor no arquivo principal */
    fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
    fread(&itemPrincipal, sizeof(T), 1, arqDados);

    /* 8. Atualiza a cadeia de elos para inserir o novo registro de forma ordenada */
    if(itemPrincipal.elo == -1){
        /* 8.1. Se o antecessor nao tinha elo, passa a apontar direto para o novo registro no temporario */
        itemPrincipal.elo = novaPosicaoTemp;

        fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
        fwrite(&itemPrincipal, sizeof(T), 1, arqDados);
    } else {
        /* 8.2. Se ja possuia elos, percorre a lista encadeada no arquivo temporario ate achar a posicao ordenada */
        eloAtual = itemPrincipal.elo;

        while(eloAtual != -1){
            fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
            if(fread(&itemTempAtual, sizeof(T), 1, arqTemp) != 1){
                break;
            }

            if(novoRegistro.product_id < itemTempAtual.product_id){
                break;
            }

            eloAnterior = eloAtual;
            eloAtual = itemTempAtual.elo;
        }

        /* 8.3. O novo registro aponta para o proximo elemento da cadeia */
        novoRegistro.elo = eloAtual;

        /* 8.4. Ajusta o ponteiro de quem vem antes (no arquivo principal ou no temporario) para apontar para o novo registro */
        if(eloAnterior == -1){
            itemPrincipal.elo = novaPosicaoTemp;

            fseek(arqDados, sizeof(T) * posAntecessor, SEEK_SET);
            fwrite(&itemPrincipal, sizeof(T), 1, arqDados);
        } else {
            fseek(arqTemp, sizeof(T) * eloAnterior, SEEK_SET);
            fread(&itemTempAtual, sizeof(T), 1, arqTemp);

            itemTempAtual.elo = novaPosicaoTemp;

            fseek(arqTemp, sizeof(T) * eloAnterior, SEEK_SET);
            fwrite(&itemTempAtual, sizeof(T), 1, arqTemp);
        }
    }

    /* 9. Escreve o novo registro no final do arquivo temporario e fecha os arquivos */
    fseek(arqTemp, sizeof(T) * novaPosicaoTemp, SEEK_SET);
    fwrite(&novoRegistro, sizeof(T), 1, arqTemp);

    fclose(arqDados);
    fclose(arqTemp);

    return {1, "Registro inserido no arquivo temporario e vinculado por elo com sucesso."};
}

/* Instanciacao explicita da funcao de inserir registros */
template RESULTADO inserirRegistro<ESTRUTURA_PRODUTO>(const char*, const char*, ESTRUTURA_PRODUTO);
template RESULTADO inserirRegistro<ESTRUTURA_COMPRA>(const char*, const char*, ESTRUTURA_COMPRA);

template <typename T, typename IndiceT>
RESULTADO reorganizarArquivo(const char *nomeArquivoDados, const char *nomeArquivoTemp, const char *nomeArquivoIndice){
    /* Registro do arquivo de dados principal (compra ou produto) */
    T itemPrincipal;
    /* Registro do arquivo temporario (compra ou produto) */
    T itemTemp;
    /* Contador para renumerar sequencialmente os registros ativos */
    int novaNumeracao = 0;
    /* Indice para percorrer a cadeia de elos no arquivo temporario */
    int eloAtual = -1;
    /* Armazena o proximo elo antes de resetar o campo do registro temporario */
    int proximoElo = -1;

    /* 1. Abre o arquivo principal de dados para leitura */
    /* Ponteiro para o arquivo de dados principal */
    FILE *arqDados = abrirArquivo(nomeArquivoDados, "rb");
    if(arqDados == NULL){
        return {0, "Erro: Nao foi possivel abrir o arquivo principal para reorganizacao."};
    }

    /* 2. Abre o arquivo temporario de extensao para seguir os elos */
    /* Ponteiro para o arquivo temporario */
    FILE *arqTemp = abrirArquivo(nomeArquivoTemp, "rb");
    if(arqTemp == NULL){
        fclose(arqDados);
        return {0, "Erro: Falha ao abrir o arquivo temporario."};
    }

    /* 3. Cria o arquivo auxiliar onde sera montada a nova base limpa e reordenada */
    /* Ponteiro para o arquivo auxiliar de reorganizacao */
    FILE *arqNovo = abrirArquivo(ARQUIVO_REORGANIZACAO, "wb");
    if(arqNovo == NULL){
        fclose(arqTemp);
        fclose(arqDados);
        return {0, "Erro: Falha ao criar arquivo auxiliar de reorganizacao."};
    }

    /* 4. Percorre todo o arquivo principal intercalando com os elos do arquivo temporario */
    while(fread(&itemPrincipal, sizeof(T), 1, arqDados) == 1){
        eloAtual = itemPrincipal.elo;

        /* 4.1. Se o registro do arquivo principal nao estiver excluido, grava com numeracao atualizada e elo resetado */
        if(itemPrincipal.excluido == 0){
            itemPrincipal.numeracao = novaNumeracao++;
            itemPrincipal.elo = -1;
            fwrite(&itemPrincipal, sizeof(T), 1, arqNovo);
        }

        /* 4.2. Percorre a cadeia de elos deste registro no arquivo temporario e grava os registros ativos */
        while(eloAtual != -1){
            fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
            if(fread(&itemTemp, sizeof(T), 1, arqTemp) != 1){
                break;
            }

            proximoElo = itemTemp.elo;

            if(itemTemp.excluido == 0){
                itemTemp.numeracao = novaNumeracao++;
                itemTemp.elo = -1;
                fwrite(&itemTemp, sizeof(T), 1, arqNovo);
            }

            eloAtual = proximoElo;
        }
    }

    /* 5. Fecha os arquivos abertos durante a copia */
    fclose(arqNovo);
    fclose(arqTemp);
    fclose(arqDados);

    /* 6. Substitui o arquivo principal antigo pelo novo arquivo reorganizado */
    remove(nomeArquivoDados);
    rename(ARQUIVO_REORGANIZACAO, nomeArquivoDados);

    /* 7. Limpa (zera) o arquivo temporario de elos, pois todos ja foram incorporados */
    /* Ponteiro para zerar o arquivo temporario */
    FILE *limparTemp = abrirArquivo(nomeArquivoTemp, "wb");
    if(limparTemp != NULL){
        fclose(limparTemp);
    }

    /* 8. Recria o arquivo de indice parcial com as novas numeracoes e IDs atualizados */
    if(novaNumeracao > 0){
        escreverIndice<T, IndiceT>(nomeArquivoDados, nomeArquivoIndice);
    } else {
        /* 8.1. Se todos os registros foram excluidos, zera tambem o arquivo de indice */
        FILE *limparIndice = abrirArquivo(nomeArquivoIndice, "wb");
        if(limparIndice != NULL){
            fclose(limparIndice);
        }
    }

    return {1, "Arquivo reorganizado, excluidos removidos e numeracao corrigida com sucesso."};
}

/* Instanciacao explicita da funcao de reorganizar arquivo */
template RESULTADO reorganizarArquivo<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(const char*, const char*, const char*);
template RESULTADO reorganizarArquivo<ESTRUTURA_COMPRA, INDICE_COMPRA>(const char*, const char*, const char*);
