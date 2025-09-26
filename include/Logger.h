#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

// Classe Logger implementada como um Singleton
class Logger {
public:
    // Retorna a única instância da classe
    static Logger& getInstance();

    // Abre o arquivo de log para escrita
    void start(const std::string& filename);

    // Escreve uma mensagem no log
    void log(const std::string& message);

    // Fecha o arquivo de log
    void stop();

private:
    // Construtores e operadores de atribuição privados para garantir o padrão Singleton
    Logger() {}
    ~Logger(); // O destrutor precisa fechar o arquivo
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Membros da classe
    std::ofstream m_logfile;
    std::mutex m_mutex;
};

#endif // LOGGER_H