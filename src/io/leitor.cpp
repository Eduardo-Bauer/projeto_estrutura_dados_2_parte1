#include <stdio.h>
#include <stdlib.h>
#include "io/leitor.h"
#include "utils/arquivo.h"
#include "models/dataset.h"
#include "models/status.h"
#include "models/arquivoCompra.h"
#include "models/arquivoProduto.h"
#include "models/indice.h"
#include "utils/constantes.h"

RESULTADO lerDataset(ESTRUTURA_DATASET **bufferSaida){
    /* malloc na memoria para os dados do dataset */
    ESTRUTURA_DATASET *buffer = (ESTRUTURA_DATASET*)malloc(sizeof(ESTRUTURA_DATASET) * TAMANHO_BUFFER);
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo dataset */
    FILE *dataset = abrirArquivo(DATASET, "rb");
    if(dataset == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo dataset."};
    }

    if(!fread(buffer, sizeof(ESTRUTURA_DATASET), TAMANHO_BUFFER, dataset)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum arquivo do dataset."};
    };

    fclose(dataset);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar o dadaset na memoria."};
}

RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida){
    int i = 0;
    
    /* malloc na memoria para os dados das compras */
    ESTRUTURA_COMPRA *buffer = (ESTRUTURA_COMPRA*)calloc(QUANTIDADE_REGISTRO, sizeof(ESTRUTURA_COMPRA));
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo das compras */
    FILE *arquivo = abrirArquivo(ARQUIVO_COMPRA, "rb");
    if(arquivo == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo de compras."};
    }

    while(i < QUANTIDADE_REGISTRO){
        if(fread(&buffer[i], sizeof(ESTRUTURA_COMPRA), 1, arquivo) == 0){
            break;
        }

        if(buffer[i].excluido == 1){
            i--;
        }
        
        i++;
    }

    fclose(arquivo);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar as compras na memoria."};    
}

RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida){
    int i = 0;
    
    /* malloc na memoria para os dados dos produtos */
    ESTRUTURA_PRODUTO *buffer = (ESTRUTURA_PRODUTO*)calloc(QUANTIDADE_REGISTRO, sizeof(ESTRUTURA_PRODUTO));
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* ponteiro para o arquivo dos produtos */
    FILE *arquivo = abrirArquivo(ARQUIVO_PRODUTO, "rb");
    if(arquivo == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo de produtos."};
    }

    while(i < QUANTIDADE_REGISTRO){
        if(fread(&buffer[i], sizeof(ESTRUTURA_PRODUTO), 1, arquivo) == 0){
            break;
        }

        if(buffer[i].excluido == 1){
            i--;
        }
        
        i++;
    }

    fclose(arquivo);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar os produtos na memoria."};    
}

template <typename T>
RESULTADO pesquisaBinaria(
    const char *nomeArquivo,
    const char *nomeArquivoTemp,
    int valor,
    int desInicial,
    int desFinal,
    int (*obterChave)(const T*),
    T **saida
){
    *saida = NULL;
    int diferenca;
    int deslocamentoMinimo = desInicial;
    int deslocamentoMaximo = desFinal;
    int deslocamento = -1;
    
    /* CORRECAO 1: Inicializa com desInicial (0) para achar elos de IDs menores que o primeiro registro */
    int posAntecessor = desInicial;
    
    T *registro = (T*)malloc(sizeof(T));
    if (registro == NULL) {
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        free(registro);
        return {0, "Erro: Falha ao abrir o arquivo."};
    }

    fseek(arquivo, 0, SEEK_END);
    int totalRegistro = (int)(ftell(arquivo) / sizeof(T));
    if(totalRegistro == 0){
        fclose(arquivo);
        free(registro);
        return {0, "Erro: Arquivo vazio."};
    }

    if(desFinal < 0 || desFinal >= totalRegistro){
        deslocamentoMaximo = totalRegistro - 1;
    }

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivo, sizeof(T) * deslocamento, SEEK_SET);
        if (fread(registro, sizeof(T), 1, arquivo) != 1) {
            break;
        }

        diferenca = valor - obterChave(registro);

        if(diferenca < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        if(diferenca > 0){
            posAntecessor = deslocamento;
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        /* Achou o valor no arquivo principal: se nao estiver excluido, retorna imediatamente */
        if(!registro->excluido){
            fclose(arquivo);
            *saida = registro;
            return {1, "Registro encontrado no arquivo principal."};
        }

        /* CORRECAO 2: Se estava excluido ou repetido, avanca para achar a ultima ocorrencia onde fica o elo */
        posAntecessor = deslocamento;
        deslocamentoMinimo = deslocamento + 1;
    }

    /* Verifica a cadeia de elos no arquivo temporario */
    if(nomeArquivoTemp != NULL && posAntecessor >= 0){
        fseek(arquivo, sizeof(T) * posAntecessor, SEEK_SET);
        if(fread(registro, sizeof(T), 1, arquivo) == 1 && registro->elo != -1){
            FILE *arqTemp = fopen(nomeArquivoTemp, "rb");
            if(arqTemp != NULL){
                int eloAtual = registro->elo;

                while(eloAtual != -1){
                    fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
                    if(fread(registro, sizeof(T), 1, arqTemp) != 1){
                        break;
                    }

                    int chaveTemp = obterChave(registro);

                    if(chaveTemp == valor && !registro->excluido){
                        fclose(arqTemp);
                        fclose(arquivo);
                        *saida = registro;
                        return {1, "Registro encontrado na area de extensao (elo)."};
                    }

                    eloAtual = registro->elo;
                }
                fclose(arqTemp);
            }
        }

        /* Caso seja uma busca por numeracao sequencial de um item que esta no arquivo temporario */
        if(valor >= totalRegistro){
            FILE *arqTemp = fopen(nomeArquivoTemp, "rb");
            if(arqTemp != NULL){
                int posTemp = valor - totalRegistro;
                fseek(arqTemp, 0, SEEK_END);
                int totalTemp = (int)(ftell(arqTemp) / sizeof(T));

                if(posTemp >= 0 && posTemp < totalTemp){
                    fseek(arqTemp, sizeof(T) * posTemp, SEEK_SET);
                    if(fread(registro, sizeof(T), 1, arqTemp) == 1 && obterChave(registro) == valor){
                        fclose(arqTemp);
                        fclose(arquivo);
                        if(registro->excluido){
                            free(registro);
                            return {0, "Registro encontrado no arquivo temporario, mas esta excluido."};
                        }
                        *saida = registro;
                        return {1, "Registro encontrado na area de extensao."};
                    }
                }
                fclose(arqTemp);
            }
        }
    }

    fclose(arquivo);
    free(registro);
    return {0, "Erro: Registro nao encontrado."};
}

template <typename T, typename IndiceT>
RESULTADO pesquisaIndice(
    const char *nomeArquivoDados, 
    const char *nomeArquivoIndice,
    const char *nomeArquivoTemp,
    int valor, 
    int (*obterMinBloco)(const IndiceT*),
    int (*obterMaxBloco)(const IndiceT*),
    int (*obterChaveDados)(const T*),
    T **saida
){
    *saida = NULL;
    int diferencaMenor;
    int diferencaMaior;
    int deslocamentoMinimo = 0;
    int deslocamentoMaximo = 0;
    int deslocamento = -1;
    
    /* CORRECAO 3: Inicializa com 0 para que IDs menores que o 1o bloco verifiquem o bloco 0 */
    int blocoCandidato = 0;

    IndiceT bloco;

    FILE *arquivoIndice = fopen(nomeArquivoIndice, "rb");
    if(arquivoIndice == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de indice."};
    }

    fseek(arquivoIndice, 0, SEEK_END);
    int totalBloco = (int)(ftell(arquivoIndice) / sizeof(IndiceT));
    if(totalBloco == 0){
        fclose(arquivoIndice);
        return {0, "Erro: Arquivo de indice vazio."};
    }

    deslocamentoMaximo = totalBloco - 1;

    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        fseek(arquivoIndice, sizeof(IndiceT) * deslocamento, SEEK_SET);
        fread(&bloco, sizeof(IndiceT), 1, arquivoIndice);

        diferencaMenor = valor - obterMinBloco(&bloco);
        diferencaMaior = valor - obterMaxBloco(&bloco);

        if(diferencaMenor < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        if(diferencaMaior > 0){
            blocoCandidato = deslocamento;
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        fclose(arquivoIndice);        

        return pesquisaBinaria<T>(
            nomeArquivoDados,
            nomeArquivoTemp,
            valor, 
            bloco.primeiro_numeracao, 
            bloco.ultimo_numeracao, 
            obterChaveDados, 
            saida
        );
    }

    /* Se o valor caiu fora dos blocos (menor que o 1o, entre dois blocos ou maior que o ultimo),
       busca no blocoCandidato onde o elo foi pendurado */
    if(nomeArquivoTemp != NULL && blocoCandidato >= 0){
        fseek(arquivoIndice, sizeof(IndiceT) * blocoCandidato, SEEK_SET);
        fread(&bloco, sizeof(IndiceT), 1, arquivoIndice);
        fclose(arquivoIndice);

        return pesquisaBinaria<T>(
            nomeArquivoDados,
            nomeArquivoTemp,
            valor,
            bloco.primeiro_numeracao,
            bloco.ultimo_numeracao,
            obterChaveDados,
            saida
        );
    }

    fclose(arquivoIndice);
    return {0, "Registro nao encontrado."};
}

/* Instanciacao explicita atualizada no final do leitor_3.cpp */
template RESULTADO pesquisaBinaria<ESTRUTURA_PRODUTO>(
    const char*, const char*, int, int, int, int (*)(const ESTRUTURA_PRODUTO*), ESTRUTURA_PRODUTO**
);

template RESULTADO pesquisaBinaria<ESTRUTURA_COMPRA>(
    const char*, const char*, int, int, int, int (*)(const ESTRUTURA_COMPRA*), ESTRUTURA_COMPRA**
);

template RESULTADO pesquisaIndice<ESTRUTURA_PRODUTO, INDICE_PRODUTO>(
    const char*, const char*, const char*, int,
    int (*)(const INDICE_PRODUTO*), int (*)(const INDICE_PRODUTO*), int (*)(const ESTRUTURA_PRODUTO*),
    ESTRUTURA_PRODUTO**
);

template RESULTADO pesquisaIndice<ESTRUTURA_COMPRA, INDICE_COMPRA>(
    const char*, const char*, const char*, int,
    int (*)(const INDICE_COMPRA*), int (*)(const INDICE_COMPRA*), int (*)(const ESTRUTURA_COMPRA*),
    ESTRUTURA_COMPRA**
);

int escolherProduto(){
    int produto = 0;

    scanf("%d", &produto);

    return produto;
}
