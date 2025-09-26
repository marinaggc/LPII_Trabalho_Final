#include "Logger.h"
#include <iostream>
#include <thread> // Biblioteca para usar threads
#include <vector> // Para guardar nossas threads

// Esta é a função que cada thread irá executar
void task(int thread_id) {
    for (int i = 0; i < 100; ++i) {
        // Constrói uma mensagem única para cada log
        std::string message = "Mensagem " + std::to_string(i) + " da Thread " + std::to_string(thread_id);
        
        // Escreve no log
        Logger::getInstance().log(message);
    }
}

int main() {
    std::cout << "Iniciando o teste de log concorrente." << std::endl;

    // Inicia o logger
    Logger::getInstance().start("log_concorrente.txt");

    // Cria um vetor para armazenar as threads
    std::vector<std::thread> threads;
    const int NUM_THREADS = 10;

    // Inicia 10 threads, cada uma executando a função 'task'
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(task, i + 1);
    }

    // Espera todas as threads terminarem seu trabalho
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i].join();
    }

    std::cout << "Teste finalizado. Verifique o arquivo log_concorrente.txt" << std::endl;

    return 0;
}