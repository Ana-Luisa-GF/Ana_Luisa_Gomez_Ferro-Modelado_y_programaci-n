#include "ManejadorEstados.h"


ManejadorEstados::ManejadorEstados(){};

ManejadorEstados::~ManejadorEstados(){};


bool ManejadorEstados::hayCliente(const std::string& username) const{
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    return clientes.find(username) != clientes.end();
}


datosCliente ManejadorEstados::darCliente(const std::string& username)const{
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    auto it = clientes.find(username); 
    if (it != clientes.end()) 
        return it->second;

    return datosCliente{};
}


std::vector<int> ManejadorEstados::agregarcliente(datosCliente nuevo_cliente){
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);
    clientes[nuevo_cliente.username]=nuevo_cliente;
    std::vector<int> sockets;
    for (const auto& par : clientes) 
        sockets.push_back(par.second.socket_cliente);
    return sockets;
    
}

void ManejadorEstados::actualizarCliente(datosCliente cliente){
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);

    clientes[cliente.username] = cliente;

    std::unique_lock<std::shared_mutex> lock_cuartos(mutex_cuartos);
    for (const std::string& sala : cliente.salas) 
        contenedor_cuartos[sala][cliente.username] = cliente;
}


void ManejadorEstados::eliminarCliente(const std::string& username){
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);
        clientes.erase(username);
    //y eliminarlo de los cuartos en que este
}


std::map<std::string, std::string> ManejadorEstados::getListaClientes() const {
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    std::map<std::string, std::string> lista;
    for (const auto& par : clientes) {
        lista[par.first] = par.second.estado;
    }
    return lista;
}


std::vector<int> ManejadorEstados::clientes_mensajePublico()const{
    std::shared_lock<std::shared_mutex> lock(mutex_clientes);
    std::vector<int> sockets;
    for (const auto& par : clientes) 
        sockets.push_back(par.second.socket_cliente);
    return sockets;
}


 bool ManejadorEstados::haySala(const std::string& roomname) const{
    std::shared_lock<std::shared_mutex> lock(mutex_cuartos);
    return contenedor_cuartos.find(roomname) != contenedor_cuartos.end();
 }

bool ManejadorEstados::crearSala(const std::string& roomname, datosCliente cliente){
    std::unique_lock<std::shared_mutex> lock(mutex_cuartos);
   if (contenedor_cuartos.find(roomname) != contenedor_cuartos.end()) 
        return false; 
    contenedor_cuartos[roomname][cliente.username] = cliente;

    return true;
 }

bool ManejadorEstados::agregarInvitacion(const std::string& username, const std::string& sala) {
    std::unique_lock<std::shared_mutex> lock(mutex_clientes);
    auto it = clientes.find(username);
    if (it != clientes.end()) {
        it->second.invitaciones.insert(sala);
        return true;
    }
    return false;
}

void ManejadorEstados::entrarSala(const std::string& username, const std::string& roomname){
    std::unique_lock<std::shared_mutex> lock_clientes(mutex_clientes);
    auto it_cliente = clientes.find(username);

    if (it_cliente == clientes.end()) 
        return;

    datosCliente cliente = it_cliente->second;

    it_cliente->second.invitaciones.erase(roomname); 
    it_cliente->second.salas.insert(roomname);

    lock_clientes.unlock();

    std::unique_lock<std::shared_mutex> lock_cuartos(mutex_cuartos);
    auto it_sala = contenedor_cuartos.find(roomname);
    if (it_sala == contenedor_cuartos.end()){
        it_cliente->second.salas.erase(roomname);
        return;
    } 
 
    it_sala->second[username] =cliente;
}

std::vector<datosCliente> ManejadorEstados::cuartoUsuarios(const std::string& roomname)const{
    std::shared_lock<std::shared_mutex> lock(mutex_cuartos);
    std::vector<datosCliente> integrantes;
    auto it = contenedor_cuartos.find(roomname);
  
    for (const auto& par : it->second) 
        integrantes.push_back(par.second);

    return integrantes;
}