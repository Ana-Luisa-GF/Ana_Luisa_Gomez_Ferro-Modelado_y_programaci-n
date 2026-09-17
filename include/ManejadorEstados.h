#ifndef MANEJADOR_ESTADOS_H
#define MANEJADOR_ESTADOS_H
#include "DatosCliente.h"
#include <string>
#include <map>
#include <vector>

class ManejadorEstados{

    private:
        std::map<std::string, datosCliente> clientes;

    public:
        ManejadorEstados();
        ~ManejadorEstados();
         
        bool obtenerCliente(std::string username);
        std::vector<int> agregarcliente(datosCliente nuevo_cliente);
        std::map<std::string, std::string> getListaClientes(); 
        void eliminarCliente(std::string username);

};
#endif