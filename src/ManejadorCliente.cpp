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
    identificado=false;
}


ManejadorCliente:: ~ManejadorCliente(){
    if (cliente.socket_cliente >= 0) {
        close(cliente.socket_cliente);  
        cliente.socket_cliente = -1; 
    }
    ejecutando = false;
}


void ManejadorCliente::escuchar(){

    std::string mensaje_cliente= "";

    mensaje_cliente = recibirMensaje();
    identificarCliente(mensaje_cliente);

    while(ejecutando){
        mensaje_cliente = recibirMensaje();

        if (mensaje_cliente.empty()) {
            desconectarcliente(); 
            return;
        }

        descifrarMensaje(mensaje_cliente);
    }

    if (cliente.socket_cliente >= 0) {
        shutdown(cliente.socket_cliente, SHUT_RDWR); 
        close(cliente.socket_cliente);               
        cliente.socket_cliente = -1;
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
            return ""; 
        }
        
        if (bytes_leidos < 0) {
            fprintf(stderr, "Error al recibir datos del socket %d: %s\n", cliente.socket_cliente, strerror(errno));
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
        cadena.clear(); 
    }
}

std::string ManejadorCliente::encontrarCampo(std::string campo, MensajeProtocolo msg){
    if(!msg.valido){
    fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
    desconectarcliente();
    return "";
    }

    auto iterador =msg.datos.find(campo);

    if(iterador == msg.datos.end()){
        fprintf(stderr, "Error: La estructura de un mensaje json es incorrecta.\n");
        desconectarcliente();
        return "";
    }

    return iterador->second;
}


void ManejadorCliente::descifrarMensaje(std::string json_recibido){
    MensajeProtocolo msg = ConstructorMensajes::desarmarMensajes(json_recibido);

    std::string type = encontrarCampo("type",msg);

    if(type == "")
        return;

    if(type  == "IDENTIFY"){
        fprintf(stderr, "Error: El cliente se intento identificar 2 veces.\n");
        desconectarcliente();
        return;                            
    }
    if(type ==  "DISCONNECT"){
        desconectarcliente();
        return;
    }
    if(type ==  "STATUS"){
        cambiarEstado(msg);
        return;
    }
    if(type == "USERS"){
        getListaUsuarios();
        return;
    }
    if(type == "TEXT"){
        mensajePrivado(msg);
        return;
    }
    if(type ==  "PUBLIC_TEXT"){
        mensajePublico(msg);
        return;
    }
    if(type == "NEW_ROOM"){
        crearSala(msg);
        return;
    }
    if(type == "INVITE"){
        invitarSala(msg);
        return;
    }
    if(type == "JOIN_ROOM"){
        aceptarInvitacion(msg);
        return;
    }
    if(type == "ROOM_TEXT"){
        mensajeSala(msg);
        return;
    }
    if(type == "ROOM_USERS"){
        getUsuariosSala(msg);
        return;
    }
    if(type ==  "LEAVE_ROOM"){
        abandonarSala(msg);
        return;

    }   
}


void ManejadorCliente::identificarCliente(std::string json_recibido){

    MensajeProtocolo msg_cliente = ConstructorMensajes::desarmarMensajes(json_recibido);

    std::string type = encontrarCampo("type",msg_cliente);
    if(type == "")
        return;

    if(type  != "IDENTIFY"){
        fprintf(stderr, "Error: El cliente intento hacer una operación antes de identificarse.\n");
        ejecutando = false;
        return;                           
    }

    std::string user = encontrarCampo("username",msg_cliente);
    if(user == "")
        if(!ejecutando)
            return;
    
    if (user.length() > 8){
        fprintf(stderr, "Error: El nombre del cliente supera el tamaño permitido.\n");
        ejecutando = false;
        return;
    }

    if(!contenedor.hayCliente(user)){
        cliente.username = user;
        cliente.estado = "ACTIVE";
        std::vector<int> sockets = contenedor.agregarcliente(cliente);

        MensajeProtocolo msg_servidor;
        msg_servidor.datos["type"]= "NEW_USER";
        msg_servidor.datos["username"]= cliente.username;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

        for (int sck : sockets)
            if(sck != cliente.socket_cliente)
              send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        
        msg_servidor=MensajeProtocolo{};
        msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "IDENTIFY";
        msg_servidor.datos["result"]= "SUCCESS";
        msg_servidor.datos["extra"]= cliente.username;

        mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);


    }else{
        MensajeProtocolo msg_servidor;                             
        msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "IDENTIFY";
        msg_servidor.datos["result"]= "USER_ALREADY_EXISTS";
        msg_servidor.datos["extra"]= "Kimberly" ;

        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        ejecutando = false;
        return;
    }

    identificado = true;
}

void ManejadorCliente::cambiarEstado(MensajeProtocolo &msg){

    std::string nuevo_status = encontrarCampo("status",msg);
    if(nuevo_status == "")
        return;

    if(nuevo_status != "AWAY" && nuevo_status != "ACTIVE" && nuevo_status != "BUSY"){
        fprintf(stderr, "Error: El nuevo estado del cliente es invalido.\n");
        desconectarcliente();
        return;
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
        if(sck != cliente.socket_cliente)
            send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}


void ManejadorCliente::desconectarcliente(){

    if(!identificado){
        ejecutando = false;
        return;
    }

    datosCliente estado_global = contenedor.darCliente(cliente.username);

    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "LEFT_ROOM";
    msg_servidor.datos["username"]= cliente.username;

    for(const std::string& sala : estado_global.salas){
       msg_servidor.datos["roomname"]= sala;
       std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

       contenedor.salirSala(cliente.username, sala);
       std::vector<datosCliente> integrantes = contenedor.cuartoUsuarios(sala);
       for (const datosCliente& integrante: integrantes)
            send(integrante.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
    }

    msg_servidor = MensajeProtocolo{};
    msg_servidor.datos["type"] = "DISCONNECTED";
    msg_servidor.datos["username"] = cliente.username;

    std::string mensaje_enviar = ConstructorMensajes::armarMensaje(msg_servidor);

    std::vector<int> sockets = contenedor.clientes_mensajePublico();
    for (int sck : sockets)
        if(sck != cliente.socket_cliente)
            send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);

    contenedor.eliminarCliente(cliente.username);
    ejecutando = false;
}


void ManejadorCliente::getListaUsuarios(){
    std::map<std::string,std::string> usuarios = contenedor.getListaClientes();
    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "USER_LIST";
    msg_servidor.users=usuarios;
    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}


void ManejadorCliente::mensajePrivado(MensajeProtocolo &msg){

    std::string usuario_destino = encontrarCampo("username",msg);
    if(!ejecutando)
        return;

    MensajeProtocolo msg_servidor;

    if(!contenedor.hayCliente(usuario_destino)){
        msg_servidor.datos["type"]= "RESPONSE";        
        msg_servidor.datos["operation"]= "TEXT";   
        msg_servidor.datos["result"]= "NO_SUCH_USER";
        msg_servidor.datos["extra"]= usuario_destino;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    std::string mensaje = encontrarCampo("text",msg);
    if(!ejecutando)
        return;

    datosCliente cliente_destino = contenedor.darCliente(usuario_destino);

    msg_servidor.datos["type"]=  "TEXT_FROM";
    msg_servidor.datos["username"]= cliente.username;
    msg_servidor.datos["text"]= mensaje;
    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    send(cliente_destino.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);

}


void ManejadorCliente::mensajePublico(MensajeProtocolo &msg){
    std::string mensaje =encontrarCampo("text",msg);
    if(!ejecutando)
     return;

    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"] = "PUBLIC_TEXT_FROM";
    msg_servidor.datos["username"] = cliente.username;
    msg_servidor.datos["text"] = mensaje;

    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

    std::vector<int> sockets = contenedor.clientes_mensajePublico();
    for (int sck : sockets)
        if(sck != cliente.socket_cliente)
            send(sck, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}


void ManejadorCliente::crearSala(MensajeProtocolo &msg){
    std::string sala = encontrarCampo("roomname",msg);
    if(!ejecutando)
            return;

    MensajeProtocolo msg_servidor;

    bool sala_creada =contenedor.crearSala(sala,cliente);

    if(!sala_creada){
        msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "NEW_ROOM";
        msg_servidor.datos["result"]= "ROOM_ALREADY_EXISTS";
        msg_servidor.datos["extra"]= sala;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "NEW_ROOM";
        msg_servidor.datos["result"]= "SUCCESS";
        msg_servidor.datos["extra"]= sala;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0); 
        cliente.salas.insert(sala);   
        contenedor.actualizarCliente(cliente);
}


void ManejadorCliente::invitarSala(MensajeProtocolo &msg){
    std::string sala = encontrarCampo("roomname",msg);
    if(!ejecutando)
        return;

    std::vector<std::string> usernames = msg.usernames;
    MensajeProtocolo msg_servidor;

    if(!contenedor.haySala(sala)){
        msg_servidor.datos["type"]= "RESPONSE";
        msg_servidor.datos["operation"]= "INVITE";
        msg_servidor.datos["result"]= "NO_SUCH_ROOM";
        msg_servidor.datos["extra"]= sala;
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    if(cliente.salas.find(sala) == cliente.salas.end())
        return;

    msg_servidor.datos["type"]= "RESPONSE";
    msg_servidor.datos["operation"]= "INVITE";
    msg_servidor.datos["result"]= "NO_SUCH_USER";

    for (std::string user : usernames)
        if(!contenedor.hayCliente(user)){
            msg_servidor.datos["extra"]= user;
            std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
            send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
            return;
        }
    
    msg_servidor = MensajeProtocolo{};  

    msg_servidor.datos["type"]= "INVITATION";
    msg_servidor.datos["username"]= cliente.username;
    msg_servidor.datos["roomname"]= sala;

    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    for (std::string user : usernames){
        datosCliente invitado = contenedor.darCliente(user);
        if(invitado.invitaciones.find(sala) != invitado.invitaciones.end() ||
            invitado.salas.find(sala) != invitado.salas.end() )
        continue;
        send(invitado.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        contenedor.agregarInvitacion(user, sala);
    }
}


void ManejadorCliente::aceptarInvitacion(MensajeProtocolo &msg){
    std::string sala = encontrarCampo("roomname",msg);
    if(!ejecutando)
        return;

    MensajeProtocolo msg_servidor;

    msg_servidor.datos["type"]= "RESPONSE";
    msg_servidor.datos["operation"]= "JOIN_ROOM";
    msg_servidor.datos["extra"]= sala;

    if(!contenedor.haySala(sala)){
        msg_servidor.datos["result"]= "NO_SUCH_ROOM";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    datosCliente estado_global = contenedor.darCliente(cliente.username);
    if(estado_global.invitaciones.find(sala) == estado_global.invitaciones.end()){
        msg_servidor.datos["result"]= "NOT_INVITED";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    msg_servidor.datos["result"]= "SUCCESS";
    contenedor.entrarSala(cliente.username,sala);

    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);


    msg_servidor = MensajeProtocolo{};

    msg_servidor.datos["type"]= "JOINED_ROOM";
    msg_servidor.datos["roomname"]= sala;
    msg_servidor.datos["username"]= cliente.username;

    mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    std::vector<datosCliente> integrantes = contenedor.cuartoUsuarios(sala);
    for (const datosCliente& integrante: integrantes)
        if(integrante.socket_cliente != cliente.socket_cliente)
            send(integrante.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}

void ManejadorCliente::mensajeSala(MensajeProtocolo &msg){
    std::string sala =encontrarCampo("roomname",msg);
    if(!ejecutando)
     return;

    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "RESPONSE";
    msg_servidor.datos["operation"]= "ROOM_TEXT";
    msg_servidor.datos["extra"]= sala;

    if(!contenedor.haySala(sala)){
        msg_servidor.datos["result"]= "NO_SUCH_ROOM";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    datosCliente estado_global = contenedor.darCliente(cliente.username);
    if(estado_global.salas.find(sala) == estado_global.salas.end()){
        msg_servidor.datos["result"]=  "NOT_JOINED";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }
    
    std::string mensaje =encontrarCampo("text",msg);
    if(!ejecutando)
     return;

    msg_servidor = MensajeProtocolo {};
    msg_servidor.datos["type"]= "ROOM_TEXT_FROM";
    msg_servidor.datos["roomname"]= sala;
    msg_servidor.datos["username"]= cliente.username;
    msg_servidor.datos["text"] = mensaje;

    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

    std::vector<datosCliente> integrantes = contenedor.cuartoUsuarios(sala);
    for (const datosCliente& integrante: integrantes)
        if(integrante.socket_cliente != cliente.socket_cliente)
            send(integrante.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}

void ManejadorCliente::getUsuariosSala(MensajeProtocolo &msg){
    std::string sala =encontrarCampo("roomname",msg);
    if(!ejecutando)
     return;

    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "RESPONSE";
    msg_servidor.datos["operation"]= "ROOM_USERS";
    msg_servidor.datos["extra"]= sala;

    if(!contenedor.haySala(sala)){
        msg_servidor.datos["result"]= "NO_SUCH_ROOM";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    datosCliente estado_global = contenedor.darCliente(cliente.username);
    if(estado_global.salas.find(sala) == estado_global.salas.end()){
        msg_servidor.datos["result"]=  "NOT_JOINED";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }
    std::vector<datosCliente> integrantes = contenedor.cuartoUsuarios(sala);
    std::map<std::string,std::string> integrantes_diccionario;
    for (const datosCliente& integrante: integrantes)
        integrantes_diccionario[integrante.username]=integrante.estado;

    msg_servidor = MensajeProtocolo{};
    msg_servidor.datos["type"]= "ROOM_USER_LIST";
    msg_servidor.datos["roomname"]= sala;
    msg_servidor.users=integrantes_diccionario;
    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
    send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}


void ManejadorCliente::abandonarSala(MensajeProtocolo &msg){
    std::string sala =encontrarCampo("roomname",msg);
    if(!ejecutando)
     return;

    MensajeProtocolo msg_servidor;
    msg_servidor.datos["type"]= "RESPONSE";
    msg_servidor.datos["operation"]= "LEAVE_ROOM";
    msg_servidor.datos["extra"]= sala;

    if(!contenedor.haySala(sala)){
        msg_servidor.datos["result"]= "NO_SUCH_ROOM";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    }

    datosCliente estado_global = contenedor.darCliente(cliente.username);
    if(estado_global.salas.find(sala) == estado_global.salas.end()){
        msg_servidor.datos["result"]=  "NOT_JOINED";
        std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);
        send(cliente.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
        return;
    };

    msg_servidor = MensajeProtocolo {};
    msg_servidor.datos["type"]= "LEFT_ROOM";
    msg_servidor.datos["roomname"]= sala;
    msg_servidor.datos["username"]= cliente.username;

    std::string mensaje_enviar =  ConstructorMensajes::armarMensaje(msg_servidor);

    contenedor.salirSala(cliente.username, sala);
    std::vector<datosCliente> integrantes = contenedor.cuartoUsuarios(sala);
    for (const datosCliente& integrante: integrantes)
            send(integrante.socket_cliente, mensaje_enviar.c_str(), mensaje_enviar.length(), 0);
}

