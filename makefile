# mingw32-make

# Compilador e flags
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude

# Pastas
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

# No Windows, SEMPRE use a extensão .exe no alvo final
TARGET := $(BIN_DIR)/programa.exe

# Liste explicitamente os seus arquivos .cpp aqui (ou .c se for C puro)
SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/io/leitor.cpp \
		$(SRC_DIR)/io/escritor.cpp \
		$(SRC_DIR)/io/tela.cpp \
        $(SRC_DIR)/utils/arquivo.cpp

# Mapeia .cpp -> .o
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(OBJS) -o $@
	@echo Compilacao concluida com sucesso: $@

# Compila cada .cpp para .o criando as subpastas em build/ automaticamente
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(@D)" mkdir "$(@D)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
	@echo Limpeza concluida.

.PHONY: all clean

# --- CONFIGURAÇÕES DE TESTE ---
TEST_DIR := tests
# Nome do executável de testes
TEST_TARGET := $(BIN_DIR)/testes.exe

# Filtra a lista de objetos (OBJS) para remover o main principal
OBJS_SEM_MAIN := $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

# Alvo para compilar e rodar o teste
test: $(OBJS_SEM_MAIN)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
#	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testeLeitor.cpp $(OBJS_SEM_MAIN) -o $(TEST_TARGET)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testeProduto.cpp $(OBJS_SEM_MAIN) -o $(TEST_TARGET)
#	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testeCompra.cpp $(OBJS_SEM_MAIN) -o $(TEST_TARGET)
	@echo Teste compilado com sucesso! Executando...
	@$(TEST_TARGET)

# Atualize a linha .PHONY no final do arquivo para incluir 'test'
.PHONY: all clean test