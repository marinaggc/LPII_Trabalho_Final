# Makefile Corrigido

# Define o compilador que vamos usar
CXX = g++

# Define as flags de compilação
CXXFLAGS = -std=c++11 -Wall -Iinclude -pthread

# Define as pastas
SRC_DIR = src
BIN_DIR = bin

# Lista explícita dos nossos arquivos fonte
SOURCES = src/main.cpp src/Logger.cpp

# Converte a lista de fontes (src/main.cpp) para uma lista de objetos (bin/main.o)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SOURCES))

# Define o nome do nosso programa executável final
TARGET = $(BIN_DIR)/teste

# Regra padrão
all: $(TARGET)

# Regra para criar o executável final a partir dos arquivos objeto
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -pthread

# Regra para compilar cada arquivo .cpp de 'src' em um arquivo .o em 'bin'
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -f $(BIN_DIR)/* src/*.o