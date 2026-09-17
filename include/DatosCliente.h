
#ifndef DATOS_CLIENTE_H
#define DATOS_CLIENTE_H

#include <string>

struct datosCliente {
    int socket_cliente;
    std::string username;
    std::string estado;
};

#endif