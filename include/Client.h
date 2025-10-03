#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    Client(int socket, const std::string& ip);
    int getSocket() const;
    std::string getIp() const;

private:
    int m_socket;
    std::string m_ipAddress;
};

#endif // CLIENT_H