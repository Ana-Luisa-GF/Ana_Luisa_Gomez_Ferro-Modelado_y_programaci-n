#include "ControladorCliente.h"
#include "VistaCliente.h"
#include "ConstructorMensajes.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <vector>


ControladorCliente::ControladorCliente(AlcanceCliente* datos, PedirServ* pedirServ){
    this->vista = nullptr;
    this->datos = datos;
    this->pedirServ = pedirServ;
    ejecutando = true;
}


void ControladorCliente::setVista(VistaCliente* vista){
    this->vista = vista;
}


void ControladorCliente::escuhar_cliente(){
    std::string linea_entrada;
    vista->pantallaInicio(); //----------------------------------------------------------------------
    std::getline(std::cin, linea_entrada);
    capturarUsername(linea_entrada);

    while (ejecutando) {        
        std::getline(std::cin, linea_entrada);

        if (linea_entrada.empty())
            continue;

        capturarInstruccion(linea_entrada);

        ejecutando=datos->getSesionActiva();
    }

}

void ControladorCliente::capturarUsername(std::string username){  
    pedirServ->identificarse(username);
} 

void ControladorCliente::capturarInstruccion(std::string mensaje){
    std::stringstream ss(mensaje);
    std::string primer_comando="";
    std::string resto_linea = "";
    ss >> primer_comando;
 
        std::getline(ss >> std::ws, resto_linea);

    if(primer_comando == "/status"){
        std::transform(resto_linea.begin(), resto_linea.end(), resto_linea.begin(), ::toupper);
        pedirServ->cambiarStatus(resto_linea);
        return;
    }
    if(primer_comando == "/usuarios"){
        pedirServ->getListaUsuarios();
        return;   
    }
    if(primer_comando =="/sala_usuarios"){
        pedirServ->getUsuariosSala(resto_linea);
        return;
    }
    if(primer_comando == "/msg"){
        std::string usuario="";
        std::string msg="";
        std::stringstream ss2(resto_linea);

        ss2 >> usuario;
        std::getline(ss2 >> std::ws, msg);

        pedirServ->mensajePrivado(usuario,msg);
        return;
    }
    if(primer_comando == "/crear_sala"){
        pedirServ->crearSala(resto_linea);
        return;
    }
    if(primer_comando == "/unirse"){
        pedirServ->unirseSala(resto_linea);  
        return;
    }
    if(primer_comando == "/miembros"){
        pedirServ->getUsuariosSala(resto_linea);
        return;
    }
    if(primer_comando == "/msg_sala"){
        std::string sala="";
        std::string msg="";
        std::stringstream ss2(resto_linea);  

        ss2 >> sala;
        std::getline(ss2 >> std::ws, msg);

        pedirServ->mensajeSala(sala,msg);
        return;
    }
    if(primer_comando == "/salir_sala"){
        pedirServ->abandonarSala(resto_linea);
        return;
    }
    if(primer_comando == "/salir"){
        pedirServ->desconectarse();
        return;
    }
    if(primer_comando == "/invitar"){
        std::string sala="";
        std::string invitados="";
        std::stringstream ss2(resto_linea); 

        ss2 >> sala;

        std::vector<std::string> lista;
        std::string invitado="";

        while (ss2 >> invitado) 
            lista.push_back(invitado);
        
        pedirServ->invitarSala(sala,lista);
        return;
    }
    pedirServ->mensajePublico(mensaje);
}
 

void ControladorCliente::usernameExistente(const std::string& username) {
    vista->mostrarUsernameExistente(username);
}

void ControladorCliente::identificacionExitosa() {
    vista->mostrarIdentificacionExitosa(datos->getUsername());
}

void ControladorCliente::nuevoUsusario(const std::string& username) {
    vista->mostrarNuevoUsuario(username);
}

void ControladorCliente::usuarioCambioStatus(const std::string& username, const std::string& status) {
    vista->mostrarUsuarioCambioStatus(username, status);
}

void ControladorCliente::listaUsuarios(const std::map<std::string, std::string>& lista) {
    vista->mostrarListaUsuarios(lista);
}

void ControladorCliente::mensajePrivado(const std::string& usuario, const std::string& mensaje) {
    vista->mostrarMensajePrivado(usuario, mensaje);
}

void ControladorCliente::falloMensajePrivado(const std::string& user) {
    vista->mostrarFalloMensajePrivado(user);
}

void ControladorCliente::recibirMensajePublico(const std::string& usuario, const std::string& mensaje) {
    vista->mostrarMensajePublico(usuario, mensaje);
}

void ControladorCliente::salaCreada(const std::string& sala) {
    vista->mostrarSalaCreada(sala);
}

void ControladorCliente::falloSalaExistente(const std::string& sala) {
    vista->mostrarFalloSalaExistente(sala);
}

void ControladorCliente::nuevaInvitación(const std::string& invitacion, const std::string& username) {
    vista->mostrarNuevaInvitacion(invitacion, username);
}

void ControladorCliente::salaNoExiste(const std::string& sala) {
    vista->mostrarSalaNoExiste(sala);
}

void ControladorCliente::usuarioNoExiste(const std::string& usuario) {
    vista->mostrarUsuarioNoExiste(usuario);
}

void ControladorCliente::entroSala(const std::string& sala) {
    vista->mostrarEntroSala(sala);
}

void ControladorCliente::usuarioNoInvitado(const std::string& sala) {
    vista->mostrarUsuarioNoInvitado(sala);
}

void ControladorCliente::listaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista) {
    vista->mostrarListaUsuariosSala(sala, lista);
}

void ControladorCliente::usuarioFueraDeSala(const std::string& sala) {
    vista->mostrarUsuarioFueraDeSala(sala);
}

void ControladorCliente::nuevoUsusarioSala(const std::string& username, const std::string& sala) {
    vista->mostrarNuevoUsuarioSala(username, sala);
}

void ControladorCliente::recibirMensajeSala(const std::string& sala, const std::string& user, const std::string& texto) {
    vista->mostrarMensajeSala(sala, user, texto);
}

void ControladorCliente::usuarioAbandonoSala(const std::string& sala, const std::string& user) {
    vista->mostrarUsuarioAbandonoSala(sala, user);
}

void ControladorCliente::usuarioDesconectado(const std::string& user) {
    vista->mostrarUsuarioDesconectado(user);
}