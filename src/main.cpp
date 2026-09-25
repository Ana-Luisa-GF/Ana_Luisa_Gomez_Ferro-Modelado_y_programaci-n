
#include "Servidor.h"
#include <iostream>
#include <cstdio>

int main() {
    int puerto; 

    std::cout << "Puerto del servidor:\n";
    std::cin >> puerto;

    Servidor mi_servidor(puerto);

    if (mi_servidor.iniciarServidor()) {
        mi_servidor.escuchar();
        printf("El servidor esta escuchando (para cerrarlo use Ctrl + C)\n");
    } else {
        std::cerr << "Fallo al levantar el servidor." << std::endl;
    }

    return 0;
}
