#ifndef CONTROLADOR_CLIENTE_H
#define CONTROLADOR_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"


class ControladorCliente{

private:
    bool ejecutando;
    VistaCliente* vista;
    alcanceCliente datos;

public:
    ControladorCliente(alcanceCliente datos);
    void setVista(VistaCliente* vista);
    void escuhar_cliente();

    void capturarUsername(std::string user);
};
#endif
