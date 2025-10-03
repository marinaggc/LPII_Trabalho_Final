#include "Client.h"

Client::Client(int socket, const std::string& ip)
    : m_socket(socket), m_ipAddress(ip) {
}

int Client::getSocket() const {
    return m_socket;
}

std::string Client::getIp() const {
    return m_ipAddress;
}