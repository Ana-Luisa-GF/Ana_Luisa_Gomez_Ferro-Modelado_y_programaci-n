
#include "servidor.h"
#include <iostream>
#include <cstdio>

int main() {
    int puerto = 1234;
    servidor mi_servidor(puerto);

    if (mi_servidor.iniciarServidor()) {
        mi_servidor.escuchar();
        printf("El servidor esta escuchando (para cerrarlo use Ctrl + C)\n");
    } else {
        std::cerr << "Fallo al levantar el servidor." << std::endl;
    }

    return 0;
}
