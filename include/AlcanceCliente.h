
#ifndef ALCANCE_CLIENTE_H
#define ALCANCE_CLIENTE_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "AlcanceCliente.h"

/**
 * @struct alcanceCLiente
 * @brief Estructura que guarda los datos del cliente y todo lo que puede conocer en el chat.
 * 
 * Funciona como un contenedor para almacenar la información que 
 * identifica al cliente y la informacion de otros usuarios que puede poseer. 
 */
struct alcanceCliente {
        int socket_cliente; /**< Descriptor de archivo asociado al socket del cliente. */                          
        std::string username; /**< Nombre de usuario del cliente. */
        std::string estado; /**< Estado o disponibilidad del usuario */

        /**
         * @brief Sobrenombre para la estructura que almacena los cuartos.
         */
        using cuartos = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

        cuartos contenedor_cuartos; /**< Contenedor global de las salas a las que pertenece el cliente y sus miembros. */
        std::unordered_map<std::string, std::string> clientes; /**< Diccionario de todos los clinentes que asocia el nombre del usuario con su estado.*/
};
#endif