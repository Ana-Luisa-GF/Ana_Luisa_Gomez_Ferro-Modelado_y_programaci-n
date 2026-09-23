#include "RecibirServ.h"
#include <sys/socket.h>
#include <unistd.h> 

#include <cerrno> 
#include <cstring>

RecibirServ::RecibirServ( int clientSocket, alcanceCliente datos){
    this->clientSocket = clientSocket;
    this->datos = datos;
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

    if(type  == "RESPONSE"){
        std::string operation = encontrarCampo("operation",msg);                  
        if(type == "")
            return;
        if(operation == "IDENTIFY"){
            return;
        }

    }
 
}

void RecibirServ::agregarUsername(MensajeProtocolo msg){
    std::string result = encontrarCampo("result",msg);  
    if(result == "USER_ALREADY_EXISTS")
     return;
    
    std::string extra = encontrarCampo("extra",msg);
    if(!ejecutando)
        return;
    datos.username=extra;  
}