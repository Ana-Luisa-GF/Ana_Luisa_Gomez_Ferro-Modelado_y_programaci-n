#include "RecibirServ.h"
#include "ControladorCliente.h"
#include "AlcanceCliente.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno> 
#include <cstring>

RecibirServ::RecibirServ( int clientSocket, AlcanceCliente* datos,ControladorCliente* controlador){
    this->clientSocket = clientSocket;
    this->datos = datos;
    this->controlador = controlador;
     ejecutando = true;
}


void RecibirServ::escucha(){

    std::string mensaje_servidor= "";

     while(ejecutando){
        mensaje_servidor = recibirMensaje();

        if (mensaje_servidor.empty()) {
            printf("Conexión cerrada por el servidor.\n");
            ejecutando = false; 
            break;
        }

        descifrarMensaje(mensaje_servidor);

    }

    if (clientSocket >= 0) {
        shutdown(clientSocket, SHUT_RDWR); 
        close(clientSocket);               
        clientSocket = -1;
    }
}

std::string RecibirServ::recibirMensaje(){

    const size_t MEGABYTE = 1024 * 1024; 
    char buffer_temporal[1024];
    ssize_t bytes_leidos = 0;
    size_t bytes_a_pedir = 0;

    while(buffer_acumulador.size() < MEGABYTE){

        if(buffer_acumulador.find('\n') != std::string::npos){
            std::string mensaje_aislado = buffer_acumulador.substr(0,buffer_acumulador.find('\n'));
            buffer_acumulador.erase(0, buffer_acumulador.find('\n') + 1);

            limpiarCadena(mensaje_aislado);
            if(!mensaje_aislado.empty())
            return mensaje_aislado;

            continue;
        }

        bytes_a_pedir = std::min(sizeof(buffer_temporal) - 1, MEGABYTE - buffer_acumulador.size());
        bytes_leidos = recv(clientSocket, buffer_temporal, bytes_a_pedir, 0);

        if (bytes_leidos == 0) {
            return ""; 
        }
        
        if (bytes_leidos < 0) {
            fprintf(stderr, "Error al recibir datos del socket %d: %s\n", clientSocket, strerror(errno));
            return "";
        }

        buffer_acumulador.append(buffer_temporal, bytes_leidos); 
    }

    if(buffer_acumulador.find('\n') != std::string::npos){
        std::string mensaje = buffer_acumulador.substr(0,buffer_acumulador.find('\n'));
        buffer_acumulador.erase(0, buffer_acumulador.find('\n') + 1);
        limpiarCadena(mensaje);
        return mensaje;
    }

    fprintf(stderr, "Error: El mensaje superó el límite de 1MB sin salto de línea.\n");
    buffer_acumulador.clear();
    return "";
}

void RecibirServ:: limpiarCadena(std::string &cadena){
    size_t pos = cadena.find_last_not_of("\r\n");

    if (pos != std::string::npos) {
        cadena.erase(pos + 1);
    } else {
        cadena.clear(); 
    }
}


std::string RecibirServ::encontrarCampo(std::string campo, MensajeProtocolo msg){
    if(!msg.valido){
    fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
    return "";
    }

    auto iterador =msg.datos.find(campo);

    if(iterador == msg.datos.end()){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        return "";
    }

    return iterador->second;
}


void RecibirServ::descifrarMensaje(std::string json_recibido){
    MensajeProtocolo msg = ConstructorMensajes::desarmarMensajes(json_recibido);

    std::string type = encontrarCampo("type",msg);

    if(type == "")
        return;

    if (type == "RESPONSE") {
        std::string operation = encontrarCampo("operation", msg);                  
        if (operation == "") 
        return;

        if (operation == "IDENTIFY"){
            identificarse(msg);
            return;
        }   
        if (operation == "TEXT"){
            falloMensajePrivado(msg);
            return;
        }  
        if (operation == "NEW_ROOM"){
            crearSala(msg);
            return;
        } 
        if (operation == "INVITE"){
            falloInvitarSala(msg);
            return;
        }
        if (operation == "JOIN_ROOM"){
            unirseSala(msg);
            return;
        } 
        if (operation == "ROOM_USERS"){
            falloListaUsuariosSala(msg);
            return;
        } 
        if (operation == "ROOM_TEXT"){
            falloMensajeSala(msg);
            return;
        } 
        if (operation == "LEAVE_ROOM"){
            falloAbandonarSala(msg);
            return;
        } 
        return;
    }

    if (type == "NEW_USER"){
        nuevoUsuario(msg);
        return;
    }            
    if (type == "NEW_STATUS"){
        usuarioCambioStatus(msg);
        return;
    }     
    if (type == "USER_LIST"){
        listaUsuarios(msg);
    }     
    if (type == "TEXT_FROM"){
        recibirMensajePrivado(msg);
        return;
    }      
    if (type == "PUBLIC_TEXT_FROM"){
        recibirMensajePublico(msg);
        return;
    } 
    if (type == "INVITATION"){
        recibirInvitacionSala(msg);
        return;
    }     
    if (type == "JOINED_ROOM"){
        nuevoUsuarioSala(msg);
        return;
    }   
    if (type == "ROOM_USER_LIST"){
        listaUsuariosSala(msg);
        return;
    } 
    if (type == "ROOM_TEXT_FROM"){
        mensajeSala(msg);
        return;
    } 
    if (type == "LEFT_ROOM"){
        usuarioAbandonoSala(msg);
        return;
    }      
    if (type == "DISCONNECTED"){
        usuarioDesconectado(msg);
        return;
    }   
 
}


void RecibirServ::identificarse(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);
    if(!ejecutando)
        return;
    
    std::string user;
    if(result == "USER_ALREADY_EXISTS"){
        user = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
        controlador->usernameExistente(user);
        return;
    }

    user = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;

    datos->setUsername(user);
    datos->setStatus("ACTIVE");
    controlador->identificacionExitosa();
}


void RecibirServ::nuevoUsuario(MensajeProtocolo msg){
    std::string user = encontrarCampo("username",msg);
        if(!ejecutando)
            return;

    datos->agregarUsuario(user, "ACTIVE");
    controlador->nuevoUsusario(user);
}

void RecibirServ::usuarioCambioStatus(MensajeProtocolo msg){
    std::string user = encontrarCampo("username",msg);
        if(!ejecutando)
            return;
    std::string status = encontrarCampo("status",msg);
        if(!ejecutando)
            return;
    datos->agregarUsuario(user, status);
    controlador->usuarioCambioStatus(user,status);
}


void RecibirServ::listaUsuarios(MensajeProtocolo msg){
    std::map<std::string,std::string> lista = msg.users;
    datos->actualizarListaUsuarios(lista);
    controlador->listaUsuarios(lista);
}


 void RecibirServ::recibirMensajePrivado(MensajeProtocolo msg){
    std::string user = encontrarCampo("username",msg);
        if(!ejecutando)
            return;

    std::string mensaje = encontrarCampo("text",msg);
        if(!ejecutando)
            return;
    controlador->mensajePrivado(user,mensaje);
 }


void RecibirServ::falloMensajePrivado(MensajeProtocolo msg){
    std::string user = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
    controlador->falloMensajePrivado(user);
}

void RecibirServ::recibirMensajePublico(MensajeProtocolo msg){
    std::string user = encontrarCampo("username",msg);
        if(!ejecutando)
            return;
    std::string mensaje = encontrarCampo("text",msg);
        if(!ejecutando)
            return;
    controlador->recibirMensajePublico(user,mensaje);
}

void RecibirServ::crearSala(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);
        if(!ejecutando)
            return;
    std::string sala = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;

        if(result=="SUCCESS"){
            datos->agregarSala(sala);
            datos->agregarUsuarioASala(sala, datos->getUsername());
            controlador->salaCreada(sala);
            return;
        }
        if(result == "ROOM_ALREADY_EXISTS"){
            controlador->falloSalaExistente(sala);
            return;
        }
}

void RecibirServ::recibirInvitacionSala(MensajeProtocolo msg){
    std::string username = encontrarCampo("username",msg);
    if(!ejecutando)
        return;
    std::string sala = encontrarCampo("roomname",msg);
        if(!ejecutando)
            return;
    datos->agregarInvitacion(sala);
    controlador->nuevaInvitación(sala,username);
}

void RecibirServ::falloInvitarSala(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);
        if(!ejecutando)
            return;
    std::string sala = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
    if(result == "NO_SUCH_ROOM"){
        controlador->salaNoExiste(sala);
        return;
    }
    if(result == "NO_SUCH_USER"){
        std::string user = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
        controlador->usuarioNoExiste(user);
        return;
    }
}

void RecibirServ::unirseSala(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);
    if(!ejecutando)
        return;
    std::string sala = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
    if(result == "SUCCESS"){
        datos->agregarSala(sala);
        controlador->entroSala(sala);
    }
    if(result == "NO_SUCH_ROOM"){
        controlador->salaNoExiste(sala);
        return;
    }
    if(result == "NOT_INVITED"){
        controlador->usuarioNoInvitado(sala);
        return;
    }
}


void RecibirServ::listaUsuariosSala(MensajeProtocolo msg){
    std::string sala = encontrarCampo("roomname",msg);
    if(!ejecutando)
        return;
    datos->actualizarListaSala(sala,msg.users);
    controlador->listaUsuariosSala(sala,msg.users);
}

void RecibirServ::falloListaUsuariosSala(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);
    if(!ejecutando)
        return;
    std::string sala = encontrarCampo("extra",msg);
        if(!ejecutando)
            return;
    if(result == "NO_SUCH_ROOM"){
        controlador->salaNoExiste(sala);
        return;
    }
    if(result == "NOT_JOINED"){
        controlador->usuarioFueraDeSala(sala);
        return;
    }
}

void RecibirServ::nuevoUsuarioSala(MensajeProtocolo msg){
    std::string username = encontrarCampo("username",msg);
    if(!ejecutando)
        return;
    std::string sala = encontrarCampo("roomname",msg);
        if(!ejecutando)
            return;
    datos->agregarUsuarioASala(sala,username);
    controlador->nuevoUsusarioSala(username,sala);
}







void RecibirServ::mensajeSala(MensajeProtocolo msg) {
    std::string sala = encontrarCampo("roomname", msg);
    if (!ejecutando) 
        return;
    std::string user = encontrarCampo("username", msg);
    if (!ejecutando) 
        return;
    std::string texto = encontrarCampo("text", msg);
    if (!ejecutando) 
        return;

    controlador->recibirMensajeSala(sala, user, texto);
}

void RecibirServ::falloMensajeSala(MensajeProtocolo msg) {
    std::string result = encontrarCampo("result", msg);
    if (!ejecutando) 
        return;

    std::string sala = encontrarCampo("extra", msg);
    if (!ejecutando) 
        return;

    if (result == "NO_SUCH_ROOM") {
        controlador->salaNoExiste(sala);
        return;
    }
    if (result == "NOT_JOINED") {
        controlador->usuarioFueraDeSala(sala);
        return;
    }
}

void RecibirServ::usuarioAbandonoSala(MensajeProtocolo msg) {
    std::string sala = encontrarCampo("roomname", msg);
    if (!ejecutando) 
        return;

    std::string user = encontrarCampo("username", msg);
    if (!ejecutando) 
        return;

    datos->quitarUsuarioSala(sala, user);
    controlador->usuarioAbandonoSala(sala, user);
}

void RecibirServ::falloAbandonarSala(MensajeProtocolo msg) {
    std::string result = encontrarCampo("result", msg);
    if (!ejecutando) return;

    std::string sala = encontrarCampo("extra", msg);
    if (!ejecutando) return;

    if (result == "NO_SUCH_ROOM") {
        controlador->salaNoExiste(sala);
        return;
    }
    if (result == "NOT_JOINED") {
        controlador->usuarioFueraDeSala(sala);
        return;
    }
}

void RecibirServ::usuarioDesconectado(MensajeProtocolo msg) {
    std::string user = encontrarCampo("username", msg);
    if (!ejecutando) 
        return;
    datos->quitarUsuario(user);
    controlador->usuarioDesconectado(user);
}