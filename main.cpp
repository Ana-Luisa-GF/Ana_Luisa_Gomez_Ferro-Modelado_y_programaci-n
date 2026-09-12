#include "servidor.h"
#include <iostream>

int main() {
    int puerto = 1234;
    servidor mi_servidor(puerto);

    if (mi_servidor.iniciarServidor()) {
        std::cout << "Presiona Enter para cerrar el servidor" << std::endl;
        std::cin.get();
    } else {
        std::cerr << "Fallo al levantar el servidor." << std::endl;
    }

    return 0;
}
