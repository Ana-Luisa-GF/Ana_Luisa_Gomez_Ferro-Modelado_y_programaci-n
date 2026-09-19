#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorEstados.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <gtest/gtest.h>

/**
 * @class Servidor
 * @brief Gestiona el socket principal de escucha y el ciclo de vida de las conexiones de los clientes.
 * 
 * La clase Servidor inicializa el socket de escucha, le asigna un puerto específico por el que 
 * acepta las conexiones de los clientes y lanza un hilo de ejecucion por cada uno.
 * Ademas mantiene una instancia de la clase ManejadorEstados a la que cada cliente tiene acceso.
 */
class Servidor{
private:
    /**
     * @brief Permite que la prueba unitaria 'GuardaraCliente' acceda a los atributos privados.
     */
    FRIEND_TEST(ServidorTest, GuardaraCliente);

    int sockfd; /**< Descriptor de archivo asignado al socket principal de escucha (-1 si no está iniciado). */
    int puerto; /**< Puerto de red en el cual el servidor aceptará conexiones. */
    struct sockaddr_in direccion; /**< Estructura de configuración del socket (Familia AF_INET, puerto e IP local). */
    bool ejecutando; /**< Bandera de control para el bucle principal de escucha. */
    ManejadorEstados contenedor /**< Administrador de estados para registrar los clientes activos y sus peticiones.*/;

public:
    /**
     * @brief Constructor de la clase Servidor.
     * @param puerto Número de puerto donde el servidor se iniciará.
     * @post Inicializa los atributos y establece 'ejecutando' en false.
     */
    Servidor(int puerto);

    /**
     * @brief Destructor de la clase Servidor.
     * Se encarga de liberar los recursos cerrando el socket principal 
     * mediante close() si continuaba activo.
     */
    ~Servidor();

    /**
     * @brief Configura el socket y lo pone en modo pasivo (o de esucha).
     * 
     * Crea el socket mediante socket(), enlaza la dirección e IP local usando bind() 
     * y habilita la captura de conexiones con listen().
     * 
     * @return true si el socket se creó, enlazó y puso en escucha correctamente; false en caso de error.
     */
    bool iniciarServidor();

    /**
     * @brief Inicia el bucle para aceptar clientes en el socket principal.
     * 
     * Ejecuta accept() dentro de un ciclo continuo mientras 'ejecutando' sea true. 
     * Por cada conexión lanza un hilo de ejecucion con una instancia de la clase ManejadorCliente.
     * @pre El método iniciarServidor() debe haber sido invocado previamente con éxito.
     */
    void escuchar();
    
   
};

#endif
