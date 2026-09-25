#include "ControladorCliente.h"
#include "VistaCliente.h"
#include "ConstructorMensajes.h"
#include <iostream>
#include <string>
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

    while (ejecutando) {
        std::getline(std::cin, linea_entrada);

        if (linea_entrada.empty())
            continue;
        

        if (linea_entrada == "/salir") {
            ejecutando = false;
            break;
        }

        distribuidor_provicional(linea_entrada);
    }

}


void ControladorCliente::distribuidor_provicional(std::string mensaje){
    char primero =mensaje.front();
    mensaje.erase(mensaje.begin());

    if(primero == '1'){
        pedirServ->identificarse(mensaje);
        return;
    }
    if(primero == '2'){
        pedirServ->cambiarStatus(mensaje);
        return;
    }
    if(primero == '3'){
        pedirServ->getListaUsuarios();
        return;
    }
    if(primero == '4'){
        pedirServ->mensajePrivado(mensaje, "ESTE ES UN MENSAJE PRIVADOOO");
        return;
    }
    if(primero == '5'){
        pedirServ->mensajePublico(mensaje);
        return;
    }
    if(primero == '6'){
        pedirServ->crearSala(mensaje);
        return;
    }
    if(primero == '7'){
        std::vector<std::string> invitados = {"Ana","Luisa"};
        pedirServ->invitarSala(mensaje,invitados);
        return;
    }
    if(primero == '8'){
        pedirServ->unirseSala(mensaje);
        return;
    }
    if(primero == '9'){
        pedirServ->getUsuariosSala(mensaje);
        return;
    }
    if(primero == '0'){
        pedirServ->mensajeSala(mensaje, "Hola sala!!! :D");
        return;
    }
    if(primero == 'a'){
        pedirServ->abandonarSala(mensaje);
        return;
    }
    if(primero == 'z'){
        pedirServ->desconectarse();
        return;
    }
    
    
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