
#include "Cliente.h"
#include "AlcanceCliente.h"
#include "VistaCliente.h"
#include "ControladorCliente.h"
#include "RecibirServ.h"
#include <thread>
#include <unistd.h>
#include <iostream>
#include <cstdio>

int main() {

    int puertoServ; 
    std::string ipServ;

    std::cout << "Puerto del servidor:\n";
    std::cin >> puertoServ;
    std::cout << "ip del servidor:\n";
    std::cin >> ipServ;

    Cliente mi_cliente(puertoServ, ipServ);
    if(!mi_cliente.iniciarCliente()){
        std::cout << "No se conecto :(\n";
        return 0;
    } 

    int clientSocket = mi_cliente.getSocket();

    AlcanceCliente alcance_cliente;

    PedirServ pedir_serv(clientSocket);

    ControladorCliente controlador(&alcance_cliente, &pedir_serv);

    RecibirServ recibir_serv(clientSocket,&alcance_cliente,&controlador);

    VistaCliente vista (&controlador);

    controlador.setVista(&vista);

    RecibirServ recibirServ(clientSocket, &alcance_cliente, &controlador);
    std::thread hiloRecibir(&RecibirServ::escucha, &recibirServ);

    controlador.escuhar_cliente();

    hiloRecibir.join();
    close(clientSocket);

    return 0;
}
