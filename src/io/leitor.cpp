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
    /* 1. Cria o buffer dos registros que serao lidos do dataset */
    /* Malloc na memoria para os dados do dataset */
    ESTRUTURA_DATASET *buffer = (ESTRUTURA_DATASET*)malloc(sizeof(ESTRUTURA_DATASET) * TAMANHO_BUFFER);
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* 2. Abre o dataset como reitura */
    /* Ponteiro para o arquivo dataset */
    FILE *dataset = abrirArquivo(DATASET, "rb");
    if(dataset == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo dataset."};
    }

    /* 3. Se nao tiver nenhum registro para ser lido */
    if(!fread(buffer, sizeof(ESTRUTURA_DATASET), TAMANHO_BUFFER, dataset)){
        free(buffer);
        return {0, "Erro: Nao foi lido nenhum arquivo do dataset."};
    };

    fclose(dataset);

    *bufferSaida = buffer;

    return {1, "Sucesso ao carregar o dadaset na memoria."};
}

template <typename T>
RESULTADO lerRegistrosComElos(const char *nomeArquivoDados, const char *nomeArquivoTemp, T **bufferSaida){
    /* Contador sequencial */
    int i = 0;
    /* Registro do arquivo de dados (compra ou produto) */
    T itemPrincipal;
    /* Registro do arquivo temporario (compra ou produto) */
    T itemTemp;

    /* 1. Cria buffer dos registros que serao lidos */
    /* Buffer para armazenar os arquivos lidos */
    T *buffer = (T*)calloc(QUANTIDADE_REGISTRO, sizeof(T));
    if(buffer == NULL){
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* 2. Abre o arquivo de dados */
    /* Ponteiro para o arquivo de dados (compra ou produto) */
    FILE *arqDados = abrirArquivo(nomeArquivoDados, "rb");
    if(arqDados == NULL){
        free(buffer);
        return {0, "Erro: Falha ao abrir o arquivo de dados."};
    }

    /* 3. Abre o arquivo temporario para seguir os elos */
    /* Ponteiro para o arquivo temporario */
    FILE *arqTemp = abrirArquivo(nomeArquivoTemp, "rb");
    if(arqTemp == NULL){
        free(buffer);
        fclose(arqDados);
        return {0, "Erro: Falha ao abrir o arquivo de temporario."};    
    }

    /* 4. Percorre o arquivo de dados até terminar a quantidade de registros para ler ou nao achar mais nenhum arquivo */
    while(i < QUANTIDADE_REGISTRO && fread(&itemPrincipal, sizeof(T), 1, arqDados) == 1){
        /* 4.1. Se o registro do arquivo principal nao estiver excluido, adiciona ao buffer */
        if(itemPrincipal.excluido == 0){
            buffer[i++] = itemPrincipal;
        }

        /* 4.2. Se este registro possuir elos no arquivo temporario, percorre a cadeia em ordem */
        int eloAtual = itemPrincipal.elo;
        while(eloAtual != -1 && i < QUANTIDADE_REGISTRO){
            fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
            if(fread(&itemTemp, sizeof(T), 1, arqTemp) != 1){
                break;
            }

            if(itemTemp.excluido == 0){
                buffer[i++] = itemTemp;
            }

            eloAtual = itemTemp.elo;
        }
    }

    fclose(arqTemp);
    fclose(arqDados);

    *bufferSaida = buffer;
    return {1, "Sucesso ao carregar os registros na memoria."};
}

RESULTADO lerCompra(ESTRUTURA_COMPRA **bufferSaida){
    return lerRegistrosComElos<ESTRUTURA_COMPRA>(ARQUIVO_COMPRA, ARQUIVO_TEMP_COMPRA, bufferSaida);
}

RESULTADO lerProduto(ESTRUTURA_PRODUTO **bufferSaida){
    return lerRegistrosComElos<ESTRUTURA_PRODUTO>(ARQUIVO_PRODUTO, ARQUIVO_TEMP_PRODUTO, bufferSaida);
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

    /* Diferenca entre o valor procurado e o valor comparado */
    int diferenca;
    /* Limite inferior do deslocamento para a busca binaria */
    int deslocamentoMinimo = desInicial;
    /* Limite superior do deslocamento para a busca binaria */
    int deslocamentoMaximo = desFinal;
    /* Posicao central calculada a cada iteracao da busca binaria */
    int deslocamento = -1;
    /* Posissao do registro analizado antes do elo */
    int posAntecessor = desInicial;
    
    /* 1. Cria buffer do registro que sera lido */
    /* Buffer para registro do arquivo de dados que sera comparado */
    T *registro = (T*)malloc(sizeof(T));
    if (registro == NULL) {
        return {0, "Erro: Memoria insuficiente para o buffer."};
    }

    /* 2. Abre o arquivo de dados (compra ou produto) */
    /* Ponteiro para o arquivo de dados */
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        free(registro);
        return {0, "Erro: Falha ao abrir o arquivo."};
    }

    fseek(arquivo, 0, SEEK_END);

    /* 3. Calcula a quantidade de registros que tem dentro do arquivo de dados */
    /* Quantidade de registros dentro do arquivo */
    int totalRegistro = (int)(ftell(arquivo) / sizeof(T));
    if(totalRegistro == 0){
        fclose(arquivo);
        free(registro);
        return {0, "Erro: Arquivo vazio."};
    }

    /* Se for uma pesquisa sem ter pesquisado antes nos indices */
    if(desFinal < 0 || desFinal >= totalRegistro){
        deslocamentoMaximo = totalRegistro - 1;
    }

    /* 4. Faz a busca dividindo os registros e procurando um com o mesmo valor */
    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        /* 4.1. Posiciona o ponteiro no registro central e realiza a leitura */
        fseek(arquivo, sizeof(T) * deslocamento, SEEK_SET);
        if (fread(registro, sizeof(T), 1, arquivo) != 1) {
            break;
        }

        diferenca = valor - obterChave(registro);

        /* 4.2. Se o valor buscado for menor, restringe a busca a metade inferior */
        if(diferenca < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        /* 4.3. Se o valor buscado for maior, atualiza o antecessor e restringe a metade superior */
        if(diferenca > 0){
            posAntecessor = deslocamento;
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        /* 4.4. Achou o valor no arquivo principal: se nao estiver excluido, retorna imediatamente */
        if(!registro->excluido){
            fclose(arquivo);
            *saida = registro;
            return {1, "Registro encontrado no arquivo principal."};
        }

        /* 4.5. Se estava excluido ou repetido, avanca para achar a ultima ocorrencia onde fica o elo */
        posAntecessor = deslocamento;
        deslocamentoMinimo = deslocamento + 1;
    }

    /* 5. Caso nao encontre ativo no principal, verifica a cadeia de elos no arquivo temporario */
    if(nomeArquivoTemp != NULL){
        /* 5.1. Le o registro antecessor no arquivo principal para obter o inicio do elo */
        fseek(arquivo, sizeof(T) * posAntecessor, SEEK_SET);

        if(fread(registro, sizeof(T), 1, arquivo) == 1 && registro->elo != -1){
            /* Ponteiro para o arquivo temporario */
            FILE *arqTemp = fopen(nomeArquivoTemp, "rb");
            if(arqTemp != NULL){
                int eloAtual = registro->elo;

                /* 5.2. Percorre a lista de elos no arquivo temporario */
                while(eloAtual != -1){
                    fseek(arqTemp, sizeof(T) * eloAtual, SEEK_SET);
                    if(fread(registro, sizeof(T), 1, arqTemp) != 1){
                        break;
                    }

                    /* Valor do registro do arquivo temporario */
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

        /* 5.3. Caso seja uma busca por numeracao sequencial de um item inserido no temporario */
        if(valor >= totalRegistro){
            /* Ponteiro para o arquivo temporario (compra, produto) */
            FILE *arqTemp = fopen(nomeArquivoTemp, "rb");
            if(arqTemp != NULL){
                /* Pega a posicao do registro no arquivo temporario */
                int posTemp = valor - totalRegistro;
                fseek(arqTemp, 0, SEEK_END);
                /* Quantidade de registros dentro do arquivo temporario */
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
    /* Diferenca entre o valor buscado e a chave minima do bloco de indice */
    int diferencaMenor;
    /* Diferenca entre o valor buscado e a chave maxima do bloco de indice */
    int diferencaMaior;
    /* Limite inferior de blocos na pesquisa binaria do indice */
    int deslocamentoMinimo = 0;
    /* Limite superior de blocos na pesquisa binaria do indice */
    int deslocamentoMaximo = 0;
    /* Bloco central analisado na iteracao atual */
    int deslocamento = -1;
    /* Guarda o bloco candidato para buscar elos caso o valor caia entre dois blocos */
    int blocoCandidato = 0;
    /* Estrutura para armazenar o bloco de indice lido */
    IndiceT bloco;

    /* 1. Abre o arquivo de indice parcial para leitura binaria */
    /* Ponteiro para o arquivo de indice */
    FILE *arquivoIndice = fopen(nomeArquivoIndice, "rb");
    if(arquivoIndice == NULL){
        return {0, "Erro: Falha ao abrir o arquivo de indice."};
    }

    /* 2. Calcula a quantidade total de blocos presentes no arquivo de indice */
    fseek(arquivoIndice, 0, SEEK_END);
    int totalBloco = (int)(ftell(arquivoIndice) / sizeof(IndiceT));
    if(totalBloco == 0){
        fclose(arquivoIndice);
        return {0, "Erro: Arquivo de indice vazio."};
    }

    deslocamentoMaximo = totalBloco - 1;

    /* 3. Realiza pesquisa binaria no arquivo de indice para localizar o bloco correspondente */
    while(deslocamentoMinimo <= deslocamentoMaximo){
        deslocamento = deslocamentoMinimo + (deslocamentoMaximo - deslocamentoMinimo) / 2;

        /* 3.1. Le o bloco de indice na posicao central */
        fseek(arquivoIndice, sizeof(IndiceT) * deslocamento, SEEK_SET);
        fread(&bloco, sizeof(IndiceT), 1, arquivoIndice);

        diferencaMenor = valor - obterMinBloco(&bloco);
        diferencaMaior = valor - obterMaxBloco(&bloco);

        /* 3.2. Se o valor for menor que o inicio do bloco, busca nos blocos anteriores */
        if(diferencaMenor < 0){
            deslocamentoMaximo = deslocamento - 1;
            continue;
        }
        
        /* 3.3. Se o valor for maior que o fim do bloco, salva como candidato e avanca */
        if(diferencaMaior > 0){
            blocoCandidato = deslocamento;
            deslocamentoMinimo = deslocamento + 1;
            continue;
        }

        /* 3.4. O valor esta dentro dos limites do bloco: fecha o indice e faz busca binaria restrita ao bloco */
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

    /* 4. Se o valor caiu fora dos limites dos blocos principais, verifica no bloco candidato por causa dos elos */
    if(nomeArquivoTemp != NULL){
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

/* Instanciacao explicita das funcoes de pesquisa */
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
