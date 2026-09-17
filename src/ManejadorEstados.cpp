#include "ManejadorEstados.h"

ManejadorEstados::ManejadorEstados(){};

ManejadorEstados::~ManejadorEstados(){};


bool ManejadorEstados::obtenerCliente(std::string username){
    return clientes.find(username) != clientes.end();
};

std::vector<int> ManejadorEstados::agregarcliente(datosCliente nuevo_cliente){
    clientes[nuevo_cliente.username]=nuevo_cliente;
    std::vector<int> sockets;
    for (const auto& par : clientes) 
        sockets.push_back(par.second.socket_cliente);
    return sockets;
    
}