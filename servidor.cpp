#include "servidor.h"
#include <unistd.h>
#include <cerrno>  
#include <cstring>
#include <iostream>


servidor::servidor(int puerto){
    this->puerto = puerto;
    sockfd = -1;
}

servidor:: ~servidor(){
    if (sockfd >= 0) {
        close(sockfd);  
        sockfd = -1; 
    }
}

bool servidor:: iniciarServidor(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        std::cerr << "Error al crear el socket: " << std::strerror(errno)<<std::endl;
        return false;
    }

    direccion.sin_family = AF_INET; 
    direccion.sin_addr.s_addr = INADDR_ANY;         
    direccion.sin_port = htons(puerto);

    int verificador = bind(sockfd, (struct sockaddr*)&direccion, sizeof(direccion));

    if(verificador == -1){
        std::cerr << "Error al asociar el puerto o la IP al socket: " << std::strerror(errno)<<std::endl;
        if (sockfd >=0) {
        close(sockfd);  
        sockfd = -1; 
        }
        return false;
    }

    verificador = listen(sockfd, 5);

    if( verificador == -1){
        std::cerr << "Error al pasar el socket a modo escucha: " << std::strerror(errno)<<std::endl;
        if (sockfd >= 0) {
        close(sockfd);  
        sockfd = -1; 
        }
        return false;
    }

    std::cout << "El servidor se inicializo exitosamente en el pueto: " << puerto << std::endl<<std::endl;
    return true;
}
