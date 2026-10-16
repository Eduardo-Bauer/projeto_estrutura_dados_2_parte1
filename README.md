meu_projeto/
├── bin/
├── build/
├── include/
│   ├── models/            <- Apenas as definições de dados (structs/classes)
│   │   ├── produto.h
│   │   └── indice.h
│   ├── io/                <- Operações de leitura, escrita e serialização
│   │   ├── leitor.h
│   │   └── escritor.h
│   └── utils/
│       └── constantes.h   <- Suas macros/defines de caminhos e tamanhos
├── src/
│   ├── io/
│   │   ├── leitor.cpp
│   │   └── escritor.cpp
│   ├── utils/
│   └── main.cpp           <- Geralmente mantido na raiz de src/
├── makefile
└── README.md