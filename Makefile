# Define o compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -Iinclude -pthread

# Pastas
SRC_DIR = src
BIN_DIR = bin

# --- DEFINIÇÕES DO SERVIDOR ---
# Arquivos de objeto (.o) necessários para o servidor
SERVER_OBJS = $(BIN_DIR)/main.o $(BIN_DIR)/Logger.o $(BIN_DIR)/Server.o $(BIN_DIR)/Client.o
# Nome do executável do servidor
SERVER_TARGET = $(BIN_DIR)/server

# --- DEFINIÇÕES DO CLIENTE ---
# Arquivo de objeto (.o) necessário para o cliente
CLIENT_OBJS = $(BIN_DIR)/client_main.o
# Nome do executável do cliente
CLIENT_TARGET = $(BIN_DIR)/client

# Regra padrão: compila tudo
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Regra para compilar o SERVIDOR
server: $(SERVER_TARGET)

# Regra para compilar o CLIENTE
client: $(CLIENT_TARGET)

# Regra de linkagem para o SERVIDOR
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(SERVER_OBJS) -o $(SERVER_TARGET) $(CXXFLAGS)

# Regra de linkagem para o CLIENTE
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CLIENT_OBJS) -o $(CLIENT_TARGET) $(CXXFLAGS)

# Regra genérica para compilar qualquer arquivo .cpp de 'src' para um .o em 'bin'
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra de limpeza
clean:
	rm -f $(BIN_DIR)/*