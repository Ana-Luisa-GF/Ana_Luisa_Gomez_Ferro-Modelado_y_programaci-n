#include "AlcanceCliente.h"


void AlcanceCliente::setUsername(const std::string& username){
    this->username = username;
}


void AlcanceCliente::setStatus(const std::string& status){
    this->status = status;
}

const std::string AlcanceCliente::getUsername(){
    return username;
}


const std::string AlcanceCliente::getStatus(){
    return status;
}

void AlcanceCliente::agregarUsuario(const std::string& username, const std::string& status) {
    auto it = listaUsuarios.find(username);
    
    if (it != listaUsuarios.end()) {
        it->second->status = status;
    } else {
        auto nuevo_cliente = std::make_shared<datosCliente>();
        nuevo_cliente->username = username;
        nuevo_cliente->status = status;
        
        listaUsuarios[username] = nuevo_cliente;
    }
}


void AlcanceCliente::agregarUsuarioASala(const std::string& nombre_sala, const std::string& username){
     auto it_sala = salas.find(nombre_sala);
     if(it_sala==salas.end())
        return;

    auto it_usuario = listaUsuarios.find(username);
    if(it_usuario == listaUsuarios.end())
        return;

    it_sala->second[username]=it_usuario->second;
}


void AlcanceCliente::actualizarListaUsuarios(const std::map<std::string,std::string>& nuevaLista){
    for(const auto& par: nuevaLista){
        agregarUsuario(par.first,par.second);
    }
}


void AlcanceCliente::actualizarListaSala(const std::string& sala, const std::map<std::string,std::string>& nuevaLista){
    for(const auto& par: nuevaLista){
        agregarUsuario(par.first,par.second);
        agregarUsuarioASala(sala,par.first);
    }
}


void AlcanceCliente::quitarUsuarioSala(const std::string& nombre_sala, const std::string& username){
    auto it = salas.find(nombre_sala);
    if(it == salas.end())
        return;
    it->second.erase(username);
}


void AlcanceCliente::quitarUsuario(const std::string& username){
    listaUsuarios.erase(username);
    for(const auto&par : salas)
        quitarUsuarioSala(par.first,username);
}

void AlcanceCliente::eliminarSala(const std::string& nombre_sala){
    salas.erase(nombre_sala);
}

void AlcanceCliente::agregarSala(const std::string& nombre_sala){
    if (salas.find(nombre_sala) == salas.end()) 
        salas[nombre_sala] = std::unordered_map<std::string, std::shared_ptr<datosCliente>>();

    quitarInvitacion(nombre_sala);
}

void AlcanceCliente::agregarInvitacion(const std::string& nombre_sala){
    invitaciones.insert(nombre_sala);
}

void AlcanceCliente::quitarInvitacion(const std::string& nombre_sala){
    invitaciones.erase(nombre_sala);
}

const std::unordered_set<std::string>& AlcanceCliente::getInvitaciones() const{
    return invitaciones;
}