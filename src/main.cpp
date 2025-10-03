#include "Logger.h"
#include "Server.h"
#include <iostream>

int main() {
    // Inicializa o logger para registrar os eventos do servidor
    Logger::getInstance().start("server_log.txt");

    // Define a porta em que o servidor vai escutar
    const int PORT = 8080;

    std::cout << "Iniciando o servidor na porta " << PORT << "..." << std::endl;
    Logger::getInstance().log("Servidor iniciando...");

    try {
        // Cria e inicia o servidor
        Server chatServer(PORT);
        chatServer.start();
    } catch (const std::exception& e) {
        std::cerr << "Erro fatal: " << e.what() << std::endl;
        Logger::getInstance().log("Erro fatal: " + std::string(e.what()));
        return 1;
    }

    Logger::getInstance().log("Servidor finalizado.");
    std::cout << "Servidor finalizado." << std::endl;

    return 0;
}