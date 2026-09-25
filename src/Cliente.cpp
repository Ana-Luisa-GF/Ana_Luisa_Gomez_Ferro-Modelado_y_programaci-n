#include "Cliente.h"
#include <arpa/inet.h> 
#include <unistd.h>
#include <cstring>

Cliente::Cliente(int puertoServ,std::string ipServ){
    this->puertoServ = puertoServ;
    this->ipServ = ipServ;
    this->clientSocket = -1; 
}


Cliente::~Cliente() {
    if (clientSocket >= 0)
        close(clientSocket);
}


 bool Cliente::iniciarCliente(){
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == -1){
        fprintf(stderr,"Error al crear el socket: %s\n", strerror(errno));
        return false;
    }

    servidor_direccion.sin_family = AF_INET;
    servidor_direccion.sin_port = htons(puertoServ);
    servidor_direccion.sin_addr.s_addr = inet_addr(ipServ.c_str());

    if (connect(clientSocket, (struct sockaddr*)&servidor_direccion, sizeof(servidor_direccion)) == -1) {
        fprintf(stderr,"Error al conectarse al servidor: %s\n", strerror(errno));
        close(clientSocket);
        return false;
    }

    return true;
 }

 int Cliente::getSocket(){
    return clientSocket;
 }
