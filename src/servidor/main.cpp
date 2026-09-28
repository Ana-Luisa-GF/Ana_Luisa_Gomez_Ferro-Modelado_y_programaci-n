
#include "servidor/Servidor.h"
#include <iostream>
#include <cstdio>
#include <csignal>

/// @brief Puntero a la instancia de la clase servidor para cerrarlo.
static Servidor* servidorGlobal = nullptr;

/// @brief función que captura el Ctrl + C
/// @param sig 
void manejarSIGINT(int sig) {
    if (servidorGlobal != nullptr) {
        servidorGlobal->detener();
    }
}

int main() {
    int puerto; 

    std::cout << "Puerto del servidor:\n";
    std::cin >> puerto;

    Servidor mi_servidor(puerto);
    servidorGlobal = &mi_servidor;

    std::signal(SIGINT, manejarSIGINT);
    
    if (mi_servidor.iniciarServidor()) {
        mi_servidor.escuchar();
    } else {
        std::cerr << "Fallo al levantar el servidor." << std::endl;
    }

    return 0;
}
