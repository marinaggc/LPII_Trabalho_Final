#include "Server.h"
#include "Logger.h"
#include <iostream>
#include <stdexcept>
#include <string.h> // Para strerror
#include <vector>
#include <thread>

// Includes de rede específicos para cada plataforma
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Link com a biblioteca de sockets do Windows
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> // Para close()
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

// Construtor do Servidor: prepara o socket para escutar por conexões
Server::Server(int port) : m_port(port), m_serverSocket(INVALID_SOCKET), m_isRunning(false) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Falha ao iniciar o Winsock.");
    }
#endif
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Falha ao criar o socket do servidor.");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        stop();
        throw std::runtime_error("Falha ao fazer o bind do socket.");
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        stop();
        throw std::runtime_error("Falha ao colocar o socket em modo de escuta.");
    }

    m_isRunning = true;
    Logger::getInstance().log("Servidor configurado com sucesso na porta " + std::to_string(m_port));
}

// Destrutor: garante que tudo seja limpo ao final
Server::~Server() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::start() {
    Logger::getInstance().log("Servidor aguardando por conexoes...");
    acceptClients();
}

void Server::stop() {
    m_isRunning = false;
    if (m_serverSocket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(m_serverSocket);
#else
        close(m_serverSocket);
#endif
        m_serverSocket = INVALID_SOCKET;
    }
}

// Loop principal que aceita novas conexões de clientes
void Server::acceptClients() {
    while (m_isRunning) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        SOCKET clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            if (m_isRunning) {
                Logger::getInstance().log("Erro ao aceitar conexao de cliente.");
            }
            continue;
        }

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
        Logger::getInstance().log("Novo cliente conectado: IP " + std::string(clientIp));

        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach();
    }
}

// Função executada por cada thread para cuidar de um cliente específico
void Server::handleClient(int clientSocket) {
    char buffer[4096];
    std::string welcomeMsg = "Bem-vindo ao servidor de Chat!\n";
    send(clientSocket, welcomeMsg.c_str(), welcomeMsg.length(), 0);

    while (m_isRunning) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);

        if (bytesReceived <= 0) {
            Logger::getInstance().log("Cliente desconectado.");
            break;
        }

        std::string message(buffer, 0, bytesReceived);
        Logger::getInstance().log("Mensagem recebida: " + message);

        std::string confirmation = "Servidor recebeu sua mensagem.\n";
        send(clientSocket, confirmation.c_str(), confirmation.length(), 0);
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

void Server::broadcastMessage(const std::string& message, int senderSocket) {
    // A ser implementado na Etapa 3
}