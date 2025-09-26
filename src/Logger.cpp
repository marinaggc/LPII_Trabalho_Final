#include "Logger.h" // Inclui a definição da nossa classe
#include <iostream>

// Função que retorna a instância estática (coração do Singleton)
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Abre o arquivo de log para escrita
void Logger::start(const std::string& filename) {
    // Se um arquivo já estiver aberto, feche-o primeiro
    if (m_logfile.is_open()) {
        m_logfile.close();
    }
    
    m_logfile.open(filename, std::ios::out | std::ios::app);
    if (!m_logfile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de log." << std::endl;
    }
}

// Versão FINAL, COM proteção de thread
void Logger::log(const std::string& message) {
    // Cria um 'lock_guard' que gerencia o nosso mutex.
    // O mutex é trancado aqui.
    std::lock_guard<std::mutex> guard(m_mutex);

    if (m_logfile.is_open()) {
        m_logfile << message << std::endl;
    }

    // Quando a função termina, o 'guard' é destruído e automaticamente
    // destranca o mutex.
}

// Fecha o arquivo de log se estiver aberto
void Logger::stop() {
    if (m_logfile.is_open()) {
        m_logfile.close();
    }
}

// Implementação do destrutor para garantir que o arquivo seja fechado ao final
Logger::~Logger() {
    stop();
}