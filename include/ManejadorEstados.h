#ifndef MANEJADOR_ESTADOS_H
#define MANEJADOR_ESTADOS_H
#include "DatosCliente.h"
#include <string>
#include <map>
#include <vector>
#include <shared_mutex>
#include <mutex>


class ManejadorEstados{

    private:
        std::map<std::string, datosCliente> clientes;
        mutable std::shared_mutex mutex_clientes;

    public:
        ManejadorEstados();
        ~ManejadorEstados();
         
        bool obtenerCliente(const std::string& username) const;

        std::vector<int> agregarcliente(datosCliente nuevo_cliente);
        std::map<std::string, std::string> getListaClientes()const; 
        void eliminarCliente(const std::string& username);

};
#endif
