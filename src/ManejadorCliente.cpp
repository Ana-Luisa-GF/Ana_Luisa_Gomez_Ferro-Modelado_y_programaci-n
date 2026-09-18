#include "ManejadorCliente.h"
#include "ManejadorEstados.h"
#include <sys/socket.h>
#include <unistd.h> 
#include <cerrno> 
#include <cstring>
#include <cstdio>
#include <vector>

ManejadorCliente::ManejadorCliente(int sck_cliente, ManejadorEstados& estado)
    :contenedor(estado){
    cliente.socket_cliente = sck_cliente;
    cliente.username = "";
    cliente.estado = "";
    ejecutando = true;
}

ManejadorCliente:: ~ManejadorCliente(){
    if (cliente.socket_cliente >= 0) {
        close(cliente.socket_cliente);  
        cliente.socket_cliente = -1; 
    }
    ejecutando = false;
}


void ManejadorCliente::escuchar(){

    printf("Nuevo un cliente, esperando su nombre de usuario...\n");
    std::string mensaje_servidor = "Ingrese su nombre de usuario:\n";
    send(cliente.socket_cliente, mensaje_servidor.c_str(), mensaje_servidor.length(), 0);

    std::string mensaje_cliente= "";
    while(ejecutando){
        mensaje_cliente = recibirMensaje();
        descifrarMensaje(mensaje_cliente);
    }

}


std::string ManejadorCliente::recibirMensaje(){

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
        bytes_leidos = recv(cliente.socket_cliente, buffer_temporal, bytes_a_pedir, 0);

        if (bytes_leidos == 0) {
            desconectarcliente();//El cliente se desconecto
            return ""; 
        }
        
        if (bytes_leidos < 0) {
            fprintf(stderr, "Error al recibir datos del socket %d: %s\n", cliente.socket_cliente, strerror(errno));
            desconectarcliente();
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
    desconectarcliente();
    buffer_acumulador.clear();
    return "";
}

void ManejadorCliente:: limpiarCadena(std::string &cadena){
    size_t pos = cadena.find_last_not_of("\r\n");

    if (pos != std::string::npos) {
        cadena.erase(pos + 1);
    } else {
        cadena.clear(); // Ocurre cuando el mensaje solo contenía "\r\n"
    }

}

void ManejadorCliente::descifrarMensaje(std::string json_recibido){
    MensajeProtocolo msg = ConstructorMensajes::desarmarMensajes(json_recibido);

    if(!msg.valido){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        desconectarcliente();
        return;
    }

    auto iterador =msg.datos.find("type");

    if(iterador == msg.datos.end()){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        desconectarcliente();
        return;
    }

    std::string type = iterador->second;

    if(type  == "IDENTIFY"){
        identificarCliente(msg);
        return;                              
    }
    if(type ==  "DISCONNECTED"){
        desconectarcliente();
        return;
    }
    if(type ==  "STATUS"){
        cambiarEstado(msg);
        return;
    }
    if(type == "USERS"){
        getListaUsuarios();
    }
        
}

void ManejadorCliente::identificarCliente(MensajeProtocolo &msg_cliente){

    auto iterador =msg_cliente.datos.find("username");

    if(iterador == msg_cliente.datos.end()){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        desconectarcliente();
    }
    std::string user = iterador->second;

    if(!contenedor.obtenerCliente(user)){
        cliente.username = iterador->second;
        cliente.estado = "ACTIVE";
        std::vector<int> sockets = contenedor.agregarcliente(cliente);
        
        MensajeProtocolo msg_servidor;
        msg_servidor.datos["type"]= "NEW_USER";
        msg_servidor.datos["username"]= cliente.username;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

        for (int sck : sockets) 
            send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
    }else{
        MensajeProtocolo msg_servidor;                             
        msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "IDENTIFY";
        msg_servidor.datos["result"]= "USER_ALREADY_EXISTS";
        msg_servidor.datos["extra"]= "Kimberly" ;

        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        close(cliente.socket_cliente); //CAMBIAR POR ALGO MAS SEGURO
        return;
    }
}

void ManejadorCliente::cambiarEstado(MensajeProtocolo &msg_cliente){
     auto iterador =msg_cliente.datos.find("status");

    if(iterador == msg_cliente.datos.end()){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        desconectarcliente();
    }
    std::string nuevo_status = iterador->second;

    if(nuevo_status != "AWAY" && nuevo_status != "ACTIVE" && nuevo_status != "BUSY"){
        fprintf(stderr, "Error: El nuevo estado del cliente es invalido.\n");
        desconectarcliente();
    }

    if(nuevo_status == cliente.estado)
        return;

    cliente.estado = nuevo_status;
    std::vector<int> sockets = contenedor.agregarcliente(cliente);
        
    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "NEW_STATUS";
    msg_servidor.datos["username"]= cliente.username;
    msg_servidor.datos["status"]= cliente.estado;
    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

    for (int sck : sockets) 
        send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
};


void ManejadorCliente::desconectarcliente(){
    contenedor.eliminarCliente(cliente.username);
    ejecutando = false;
};


void ManejadorCliente::getListaUsuarios(){
    std::map<std::string,std::string> usuarios = contenedor.getListaClientes();
    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "USER_LIST";
    msg_servidor.users=usuarios;
    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}