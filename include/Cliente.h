#ifndef CLIENTE_H
#define CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include "RecibirServ.h"
#include "PedirServ.h"
#include "AlcanceCliente.h"

class Cliente{
    private:

    int clientSocket;

    int puertoServ; 
    std::string ipServ; 
    struct sockaddr_in servidor_direccion; 
    


    public:

    Cliente(int puertoServ, std::string ipServ); 
    ~Cliente();

    bool iniciarCliente();
    
};
#endif
