#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <thread>
#include <mutex>
#include "Client.h" // O Server vai precisar conhecer o Client

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();

private:
    void acceptClients();
    void handleClient(int clientSocket);
    void broadcastMessage(const std::string& message, int senderSocket);

    int m_port;
    int m_serverSocket;
    bool m_isRunning;
    std::vector<Client> m_clients;
    std::mutex m_clientsMutex;
};

#endif // SERVER_H