#ifndef CLIENTE_H
#define CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include "cliente/RecibirServ.h"
#include "cliente/PedirServ.h"
#include "cliente/AlcanceCliente.h"

/**
 * @brief Clase encargada de establecer la conexión del cliente con el servidor.
 */
class Cliente {
private:

    int clientSocket;

    int puertoServ; 
    std::string ipServ; 
    struct sockaddr_in servidor_direccion; 

public:

    /**
     * @brief Inicializa la dirección IP y el puerto del servidor.
     * @param puertoServ Puerto de red del servidor.
     * @param ipServ Dirección IP del servidor.
     */
    Cliente(int puertoServ, std::string ipServ); 

    /**
     * @brief Cierra el socket del cliente.
     */
    ~Cliente();

    /**
     * @brief Devuelve el descriptor de archivo del socket.
     * @return Entero con el descriptor del socket.
     */
    int getSocket();

    /**
     * @brief Crea el socket y establece la conexión con el servidor.
     * @return true si la conexión fue exitosa, false en caso contrario.
     */
    bool iniciarCliente();
    
};
#endif