#include "ManejadorEstados.h"


ManejadorEstados::ManejadorEstados(){};

ManejadorEstados::~ManejadorEstados(){};


bool ManejadorEstados::obtenerCliente(const std::string& username) const{
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    return clientes.find(username) != clientes.end();
};

std::vector<int> ManejadorEstados::agregarcliente(datosCliente nuevo_cliente){
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);
    clientes[nuevo_cliente.username]=nuevo_cliente;
    std::vector<int> sockets;
    for (const auto& par : clientes) 
        sockets.push_back(par.second.socket_cliente);
    return sockets;
    
}

void ManejadorEstados::eliminarCliente(const std::string& username){
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);
        clientes.erase(username);
    //y eliminarlo de los cuartos en que este
};

std::map<std::string, std::string> ManejadorEstados::getListaClientes() const {
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    std::map<std::string, std::string> lista;
    for (const auto& par : clientes) {
        lista[par.first] = par.second.estado;
    }
    return lista;
}