#include "Servidor.h"
#include "ManejadorCliente.h"
#include <unistd.h> 
#include <cerrno>  
#include <cstring>
#include <cstdio>


Servidor::Servidor(int puerto){
    this->puerto = puerto;
    sockfd = -1;
    ejecutando = true;
    direccion = {};
}

Servidor:: ~Servidor(){
    if (sockfd >= 0) {
        close(sockfd);  
        sockfd = -1; 
    }
    ejecutando = false;
}

bool Servidor:: iniciarServidor(){
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

void Servidor:: escuchar(){
    int cliente_socket;
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress); 

    printf("Servidor escuchando...\n");

    while(ejecutando){

        cliente_socket = accept(sockfd, (struct sockaddr*)&clientAddress, &clientLen);

        if(cliente_socket == -1){
            fprintf(stderr,"Error al intentar conectar al cliente: %s\n", strerror(errno));
            continue;
        }

       ManejadorCliente* cliente = new ManejadorCliente(cliente_socket , contenedor);
        
        std::thread([cliente]() {
            cliente->escuchar();
            delete cliente; 
        }).detach();
    }
}
