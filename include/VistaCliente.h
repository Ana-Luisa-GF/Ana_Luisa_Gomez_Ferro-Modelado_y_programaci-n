#ifndef VISTA_CLIENTE_H
#define VISTA_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "ControladorCliente.h"

class VistaCliente{

    private:
        ControladorCliente* controlador;

    public:
        VistaCliente(ControladorCliente* controlador);
        void mensjaeInicio();



};
#endif
