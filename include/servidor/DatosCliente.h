
#ifndef DATOS_CLIENTE_H
#define DATOS_CLIENTE_H

#include <string>
#include <unordered_set>

/**
 * @struct datosCliente
 * @brief Estructura que guarda los datos de un cliente.
 * 
 * Funciona como un contenedor para almacenar la información que 
 * identifica a cada cliente. 
 */
struct datosCliente {
    int socket_cliente; /**< Descriptor de archivo asociado al socket del cliente. */                          
    std::string username; /**< Nombre de usuario del cliente. */
    std::string estado; /**< Estado o disponibilidad del usuario */
    std::unordered_set<std::string> salas;/**< Salas en las que esta el cliente. */
    std::unordered_set<std::string> invitaciones;/**< Salas a las que se le ha invitado al cliente y no ha aceptado. */
};

#endif