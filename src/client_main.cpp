#include <iostream>
#include <string>
#include <thread> 
#include <cstring>

// Includes de rede específicos para cada plataforma
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

// Função para receber mensagens do servidor em uma thread separada
void receiveMessages(SOCKET sock) {
    char buffer[4096];
    while (true) {
        memset(buffer, 0, 4096);
        int bytesReceived = recv(sock, buffer, 4096, 0);
        if (bytesReceived <= 0) {
            std::cout << "\nDesconectado do servidor. Pressione Enter para sair." << std::endl;
            break;
        }
        std::cout << "\nServidor: " << std::string(buffer, 0, bytesReceived);
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Falha ao iniciar o Winsock." << std::endl;
        return 1;
    }
#endif

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Falha ao criar o socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Nao foi possivel conectar ao servidor. Verifique se o servidor esta rodando." << std::endl;
#ifdef _WIN32
        closesocket(clientSocket);
        WSACleanup();
#else
        close(clientSocket);
#endif
        return 1;
    }

    std::cout << "Conectado ao servidor de chat. Digite 'sair' para desconectar.\n" << std::endl;

    // Inicia uma thread para ficar escutando por mensagens do servidor
    std::thread receiverThread(receiveMessages, clientSocket);
    receiverThread.detach();

    std::string userInput;
    while (true) {
        std::getline(std::cin, userInput);

        if (userInput == "sair") {
            break;
        }

        if (send(clientSocket, userInput.c_str(), userInput.length(), 0) == SOCKET_ERROR) {
            std::cout << "Falha ao enviar mensagem." << std::endl;
            continue;
        }
    }

#ifdef _WIN32
    closesocket(clientSocket);
    WSACleanup();
#else
    close(clientSocket);
#endif

    return 0;
}