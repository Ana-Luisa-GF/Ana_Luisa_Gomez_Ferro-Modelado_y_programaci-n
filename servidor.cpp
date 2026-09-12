#include "servidor.h"
#include <unistd.h>
#include <cerrno>  
#include <cstring>
#include <iostream>
#include <cstdio>


servidor::servidor(int puerto){
    this->puerto = puerto;
    sockfd = -1;
    ejecutando = true;
}

servidor:: ~servidor(){
    if (sockfd >= 0) {
        close(sockfd);  
        sockfd = -1; 
    }
    ejecutando = false;
}

bool servidor:: iniciarServidor(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        fprintf(stderr,"Error al crear el socket: %s\n", strerror(errno));
        return false;
    }

    direccion.sin_family = AF_INET; 
    direccion.sin_addr.s_addr = INADDR_ANY;         
    direccion.sin_port = htons(puerto);

    int verificador = bind(sockfd, (struct sockaddr*)&direccion, sizeof(direccion));

    if(verificador == -1){
        fprintf(stderr,"Error al asociar el puerto o la IP al socket: %s\n", strerror(errno));
        if (sockfd >=0) {
        close(sockfd);  
        sockfd = -1; 
        }
        return false;
    }

    verificador = listen(sockfd, 5);

    if( verificador == -1){
        fprintf(stderr,"Error al pasar el socket a modo escucha: %s\n", strerror(errno));
        if (sockfd >= 0) {
        close(sockfd);  
        sockfd = -1; 
        }
        return false;
    }

    printf("El servidor se inicializo exitosamente en el puerto: %d\n", puerto);
    return true;
}

void servidor:: escuchar(){
    int cliente_socket;
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    std::string username;

    printf("Servidor escuchando...\n");

    while(ejecutando){

        cliente_socket = accept(sockfd, (struct sockaddr*)&clientAddress, &clientLen);
        if(cliente_socket == -1){
            fprintf(stderr,"Error al intentar conectar al cliente: %s\n", strerror(errno));
            continue;
        }
        sockets_clientes.push_back(cliente_socket);
        printf("Nuevo un cliente\n");
        printf("Esperando el username del cliente\n");
        username = recibir_mensaje(cliente_socket);

        if(diccionario_clientes.contains(username))
            printf("Este usuario ya existia, lo actualizamos.\n");

        diccionario_clientes.insert({username, cliente_socket});
    }
}

std::string servidor:: recibir_mensaje(int sock_cliente){
    
    char buffer[1024] = {0};

    ssize_t bytes = recv(sock_cliente, buffer, sizeof(buffer)-1, 0);

    if(bytes > 0){
        std::string mensaje(buffer);
        limpiarCadena(mensaje);
        return mensaje;
    }else{
        if(bytes == 0){
            return "";
        }else{
            fprintf(stderr, "Error al recibir datos del socket %d: %s\n", sock_cliente, strerror(errno));
            return "";
        }
    }
    
}

void servidor:: limpiarCadena(std::string &cadena){
    size_t pos = cadena.find_last_not_of("\r\n");

    if (pos != std::string::npos) {
        cadena.erase(pos + 1);
    } else {
        cadena.clear(); // Ocurre cuando el mensaje solo contenía "\r\n"
    }

}