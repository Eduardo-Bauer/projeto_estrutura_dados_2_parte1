# 📦 Organização de Arquivos Sequencial-Indexado com Índices Parciais

Trabalho prático desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados II**. O objetivo deste projeto é implementar uma organização de arquivos **sequencial-indexada** em memória secundária (disco), utilizando arquivos binários de tamanho fixo, índices parciais baseados em blocos, pesquisa binária direta com `fseek` e manipulação de inserções/exclusões por meio de área de extensão (overflow) encadeada por elos e exclusão lógica.

---

## 🎯 1. Contexto e Base de Dados

O conjunto de dados explorado neste projeto refere-se ao comportamento de usuários em uma loja virtual de múltiplos departamentos (**"eCommerce behavior data from multi category store"**), disponibilizado publicamente no [Kaggle](https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store).

O dataset original contém registros de acessos, visualizações e compras de produtos ao longo de meses. A partir da base bruta pré-processada, o sistema extrai e constrói **dois arquivos de dados binários independentes e ordenados**:
1. **Arquivo de Produtos (`ARQUIVO_PRODUTO`):** Catálogo contendo os produtos únicos comercializados no e-commerce, ordenados por `product_id` e numerados sequencialmente (`numeracao`).
2. **Arquivo de Compras / Acesso (`ARQUIVO_COMPRA`):** Histórico contendo especificamente os eventos de conversão em vendas (`event_type == "purchase"`), armazenando o produto adquirido, preço praticado, marca e categoria.

---

## ❓ 2. Perguntas de Negócio (Consultas) e Hipóteses

Para validar a organização sequencial-indexada sem carregar a base de dados para a memória RAM, foram definidas três questões principais que são respondidas diretamente pelas operações de busca do sistema:

### Pergunta 1: Conversão de Catálogo em Venda (Busca por `product_id`)
> *"Um determinado produto cadastrado no catálogo da loja (identificado pelo seu `product_id`) chegou a ser efetivamente comprado pelos usuários, e por qual valor e marca ele foi comercializado?"*

* **Hipótese A:** Nem todos os produtos presentes no catálogo geral (`ARQUIVO_PRODUTO`) possuem registros de venda concretizada (`event_type == "purchase"`) no arquivo de compras (`ARQUIVO_COMPRA`).
* **Hipótese B:** Produtos de marcas consolidadas possuem registros ativos tanto no arquivo de catálogo quanto no arquivo de compras.
* **Como responder no sistema:** Utilizar as opções **[3] Consultar dado por pesquisa binária** ou **[5] Consultar dados a partir do índice** informando o `product_id` primeiro no **Arquivo de Produtos** (para atestar sua existência e marca no catálogo) e, em seguida, no **Arquivo de Compras** (para verificar a ocorrência da venda e o `price` praticado).

### Pergunta 2: Rastreamento Sequencial de Transações (Busca por `numeracao`)
> *"Qual foi o $N$-ésimo registro de compra efetivada no e-commerce (por exemplo, a 1ª, a 100ª ou a 500ª compra registrada na base) e a qual marca e faixa de preço esse produto pertence?"*

* **Hipótese A:** É possível recuperar instantaneamente qualquer transação específica pela sua ordem sequencial de ocorrência (`numeracao`) em complexidade logarítmica $\mathcal{O}(\log N)$, sem percorrer o arquivo sequencialmente desde o início.
* **Como responder no sistema:** Utilizar as opções **[4] Consultar dado por sequência** (pesquisa binária direta) ou **[6] Consultar dado por sequência (índice)** no **Arquivo de Compras**, informando o número sequencial desejado.

### Pergunta 3: Desempenho de Consulta e Integridade após Atualizações Dinâmicas
> *"Como manter a eficiência de consulta por um produto ou compra específica após a chegada de novos registros fora de ordem ou do cancelamento (exclusão) de registros, sem reconstruir o arquivo inteiro a cada operação?"*

* **Hipótese A:** O uso de um **Índice Parcial** (Opções 5 e 6) reduz o espaço de busca no arquivo de dados a um único bloco delimitado por `primeiro_numeracao` e `ultimo_numeracao`, exigindo menos leituras em disco no arquivo principal do que a pesquisa binária global (Opções 3 e 4).
* **Hipótese B:** A combinação de uma área de extensão (`arqTemp`) encadeada pelo campo `elo` com a marcação de exclusão lógica (`excluido = 1`) permite inserir e remover registros mantendo a consistência das consultas antes da reorganização física ao encerrar o programa (**Opção 0**).

---

## 🗂️ 3. Descrição dos Arquivos de Dados e de Índices

Todos os arquivos são gravados em **modo binário** (`rb`, `wb`, `rb+`), garantindo registros de **tamanho fixo** pelo alinhamento nativo das estruturas (`sizeof(struct)`) e preenchimento com bytes nulos (`\0`) nos campos textuais, permitindo o acesso direto via deslocamento de bytes (`fseek`).

### 3.1. Arquivo de Dados de Produtos (`ESTRUTURA_PRODUTO`)
Armazena o catálogo único de produtos ordenado por `product_id`.

| Campo | Tipo | Descrição | Papel na Organização |
| :--- | :--- | :--- | :--- |
| `numeracao` | `int` / `unsigned long` | Número sequencial único gerado na criação | Chave primária sequencial (não repetida) |
| `product_id` | `int` | Identificador do produto no e-commerce | Chave de ordenação e busca (não repetida em Produtos) |
| `brand` | `char[]` | Marca do fabricante do produto | Campo textual de tamanho fixo (informação repetida) |
| `category_code` | `char[]` | Categoria/departamento do produto | Campo textual de tamanho fixo (informação repetida) |
| `elo` | `int` | Índice do próximo registro na área de extensão (`-1` se vazio) | Controle de encadeamento de overflow |
| `excluido` | `int` | Flag de status do registro (`0` = ativo, `1` = excluído) | Controle de remoção lógica |

### 3.2. Arquivo de Dados de Compras (`ESTRUTURA_COMPRA`)
Armazena os eventos de compras realizadas (`event_type == "purchase"`), ordenado por `product_id`.

| Campo | Tipo | Descrição | Papel na Organização |
| :--- | :--- | :--- | :--- |
| `numeracao` | `int` / `unsigned long` | Número sequencial único da transação de compra | Chave sequencial única (não repetida) |
| `product_id` | `int` | Identificador do produto comprado | Chave de ordenação e busca por código |
| `price` | `float` | Preço pago pelo produto na transação | Atributo numérico da compra |
| `brand` | `char[]` | Marca do produto adquirido | Campo textual de tamanho fixo (informação repetida) |
| `category_code` | `char[]` | Categoria do produto adquirido | Campo textual de tamanho fixo (informação repetida) |
| `elo` | `int` | Índice do próximo registro na área de extensão (`-1` se vazio) | Controle de encadeamento de overflow |
| `excluido` | `int` | Flag de status do registro (`0` = ativo, `1` = excluído) | Controle de remoção lógica |

### 3.3. Arquivos de Índices Parciais (`INDICE_PRODUTO` e `INDICE_COMPRA`)
Para cada arquivo de dados, é construído um arquivo de **índice parcial denso por blocos** (de nível 1). O arquivo de dados é dividido em blocos de tamanho fixo definidos pela constante `QUANTIDADE_REGISTRO_INDICE`. Cada entrada no arquivo de índice armazena as chaves do **primeiro** e do **último** registro daquele bloco:

| Campo | Tipo | Descrição |
| :--- | :--- | :--- |
| `primeiro_numeracao` | `int` | Posição/numeração inicial do bloco no arquivo de dados |
| `primeiro_produto_id` | `int` | Menor `product_id` contido no início do bloco |
| `ultimo_numeracao` | `int` | Posição/numeração final do bloco no arquivo de dados |
| `ultimo_produto_id` | `int` | Maior `product_id` contido no final do bloco |

---

## ⚙️ 4. Estratégias de Pesquisa, Inserção, Remoção e Reconstrução

Conforme os critérios da disciplina, **nenhuma operação de busca, inserção ou exclusão carrega o arquivo de dados inteiro na memória RAM**. Todas as operações manipulam apenas um registro/bloco por vez diretamente em memória secundária usando `fseek` e `fread`/`fwrite`.

### 🔍 4.1. Pesquisa Binária Direta vs. Pesquisa por Índice Parcial
* **Pesquisa Binária Direta (`pesquisaBinaria`):** Calcula o total de registros do arquivo via `ftell(arquivo) / sizeof(T)` e executa a busca binária posicionando o cabeçote de leitura com `fseek(arquivo, sizeof(T) * deslocamento, SEEK_SET)`. Caso não encontre no arquivo principal (ou o registro esteja marcado como excluído), verifica automaticamente a cadeia de `elo` do registro antecessor no arquivo temporário.
* **Pesquisa pelo Índice Parcial (`pesquisaIndice`):** 
  1. Realiza uma **pesquisa binária no arquivo de índice** para descobrir em qual bloco o valor procurado (`product_id` ou `numeracao`) se encontra.
  2. Uma vez identificado o bloco, invoca a `pesquisaBinaria` no arquivo de dados **restrita apenas ao intervalo daquele bloco** (`bloco.primeiro_numeracao` até `bloco.ultimo_numeracao`), reduzindo drasticamente os acessos ao arquivo de dados.

### ➕ 4.2. Estratégia de Inserção (Área de Extensão + Encadeamento por Elos)
Para evitar o deslocamento físico custoso de milhares de registros a cada nova inserção, foi implementada a estratégia de **Área de Extensão (Overflow) com Lista Encadeada em Disco**:
1. **Caso Padrão (Chave $\ge$ 1º registro):** O programa localiza via pesquisa binária no arquivo principal o registro imediatamente **antecessor** ao novo item. O novo registro é gravado no final do arquivo temporário (`ARQUIVO_TEMP_PRODUTO` ou `ARQUIVO_TEMP_COMPRA`) e o campo `elo` do antecessor é atualizado para apontar para a posição física desse novo registro no arquivo temporário. Caso já existam elos pendurados naquele antecessor, a cadeia no arquivo temporário é percorrida para inserir o novo elo mantendo a ordem crescente de `product_id`.
2. **Caso Especial (Chave $<$ 1º registro da posição 0):** Se o novo registro tiver um `product_id` menor que o primeiro item do arquivo principal (posição `0`), o antigo item da posição `0` é movido para o arquivo temporário (preservando seus elos) e o novo registro assume a posição `0` do arquivo principal apontando para o antigo. Isso garante que nenhum registro fique órfão antes do início do arquivo.

### ➖ 4.3. Estratégia de Remoção (Exclusão Lógica)
A remoção (`excluirRegistro`) utiliza **exclusão lógica**:
1. O registro é localizado via pesquisa no índice parcial (por código `product_id` ou por sequência `numeracao`).
2. O sistema verifica se a numeração pertence ao arquivo principal ou ao arquivo temporário de extensão.
3. Posiciona o ponteiro exatamente sobre o registro com `fseek`, altera a flag `excluido = 1` e sobrescreve apenas aquele registro com `fwrite`.
4. Registros com `excluido == 1` passam a ser ignorados nas listagens e buscas subsequentes, mas seus ponteiros de `elo` continuam preservados para não quebrar a cadeia de registros ativos na área de extensão.

### 🔄 4.4. Reorganização dos Arquivos e Reconstrução dos Índices
A reconstrução física dos arquivos e dos índices ocorre **ao encerrar o programa (Opção 0 - `encerrarSistema`)**:
1. A função `reorganizarArquivo` percorre sequencialmente o arquivo principal.
2. Copia para um novo arquivo auxiliar apenas os registros ativos (`excluido == 0`), intercalando imediatamente após cada registro principal toda a sua cadeia de elos ativos presentes no arquivo temporário.
3. Atualiza o campo `numeracao` de forma contínua (`0, 1, 2, ...`) e reseta `elo = -1`.
4. Substitui o arquivo principal antigo pelo novo arquivo consolidado, esvazia o arquivo temporário de extensão e **reconstrói integralmente os arquivos de índice parcial** (`escreverIndice`).

---

## 📂 5. Estrutura de Diretórios do Projeto

```text
├── data/                       # Diretório destinado aos arquivos binários (dados, índices e temp)
├── include/                    # Cabeçalhos (.h)
│   ├── io/
│   │   ├── escritor.h          # Assinaturas de criação, inserção, exclusão e reorganização
│   │   ├── leitor.h            # Assinaturas de leitura, busca binária e busca indexada
│   │   └── tela.h              # Assinaturas de interface de console e menus
│   ├── models/
│   │   ├── arquivoCompra.h     # Definição da struct ESTRUTURA_COMPRA
│   │   ├── arquivoProduto.h    # Definição da struct ESTRUTURA_PRODUTO
│   │   ├── dataset.h           # Definição da struct ESTRUTURA_DATASET
│   │   ├── indice.h            # Definição das structs INDICE_PRODUTO e INDICE_COMPRA
│   │   └── status.h            # Estrutura RESULTADO para retorno padronizado de operações
│   └── utils/
│       ├── arquivo.h           # Utilitário de abertura de arquivos
│       └── constantes.h        # Caminhos de arquivos e tamanhos de blocos/buffers
└── src/                        # Implementação do código-fonte (.cpp)
    ├── arquivo.cpp             # Encapsulamento de abertura de arquivos
    ├── escritor.cpp            # Escrita de dados, índices, inserção com elos, exclusão e reorganização
    ├── leitor.cpp              # Leitura com elos, pesquisa binária e pesquisa por índice parcial
    ├── tela.cpp                # Menus interativos e exibição formatada dos registros
    └── main.cpp                # Ponto de entrada e orquestração das funcionalidades
```

---

## 🚀 6. Como Compilar e Executar

### Pré-requisitos
* Compilador C++ compatível com C++11 ou superior (`g++` / MinGW no Windows).
* Estrutura de pastas para os arquivos binários criada conforme configurado em `constantes.h`.

### Compilação
Na raiz do projeto, execute o comando abaixo no terminal:

```bash
g++ src/main.cpp src/leitor.cpp src/escritor.cpp src/tela.cpp src/arquivo.cpp -Iinclude -o ecommerce_indexado.exe
```

### Execução e Fluxo Recomendado de Uso
```bash
./ecommerce_indexado.exe
```

1. **Opção `[1]` - Inserir os dados iniciais:** Execute esta opção na primeira vez para ler o dataset base e gerar os arquivos binários de **Produtos**, **Compras** e seus respectivos **Índices Parciais**.
2. **Opção `[2]` - Mostrar os dados parcial:** Exibe os registros ativos dos arquivos, já intercalando na ordem correta eventuais itens inseridos na área de extensão.
3. **Opções `[3]` e `[4]` - Consultas por Pesquisa Binária Direta:** Buscam um registro por `product_id` (`[3]`) ou `numeracao` (`[4]`) diretamente no arquivo de dados.
4. **Opções `[5]` e `[6]` - Consultas a partir do Índice Parcial:** Pesquisam primeiro no arquivo de índice para localizar o bloco e depois fazem `fseek` restrito ao bloco no arquivo de dados.
5. **Opção `[7]` - Inserir dados:** Insere um novo produto ou compra na área de extensão e vincula o elo mantendo a ordenação lógica.
6. **Opções `[8]` e `[9]` - Remover dados:** Realizam a exclusão lógica de um registro por código (`[8]`) ou por sequência (`[9]`).
7. **Opção `[0]` - Fechar programa:** Reorganiza fisicamente os arquivos (incorporando os elos e removendo definitivamente os excluídos) e reconstrói os arquivos de índice parcial em disco.