#ifndef PEDIR_SERV_H
#define PEDIR_SERV_H

#include "comunes/ConstructorMensajes.h"
#include <map>
#include <string>
#include <vector>
#include "cliente/AlcanceCliente.h"

/**
 * @brief Clase encargada de construir y enviar peticiones JSON al servidor.
 */
class PedirServ {

private:
    int clientSocket;

public:
    /**
     * @brief Inicializa la clase con el socket del cliente.
     */
    PedirServ(int ClientSocket);

    /**
     * @brief Destructor.
     */
    ~PedirServ() = default;

    /**
     * @brief Envía el nombre de usuario.
     */
    void identificarse(std::string username);    

    /**
     * @brief Envía la solicitud para cambiar el estado del cliente.
     */
    void cambiarStatus(std::string status);

    /**
     * @brief Solicita la lista de usuarios conectados.
     */
    void getListaUsuarios();

    /**
     * @brief Envía un mensaje privado a un usuario.
     */
    void mensajePrivado(std::string user, std::string mensaje);

    /**
     * @brief Envía un mensaje al público.
     */
    void mensajePublico(std::string mensaje);

    /**
     * @brief Solicita la creación de una nueva sala.
     */
    void crearSala(std::string sala);

    /**
     * @brief Envía invitaciones a usuarios para unirse a una sala.
     */
    void invitarSala(std::string sala, std::vector<std::string> invitados);

    /**
     * @brief Solicita unirse a una sala.
     */
    void unirseSala(std::string sala);

    /**
     * @brief Solicita la lista de integrantes de una sala.
     */
    void getUsuariosSala(std::string sala);

    /**
     * @brief Envía un mensaje a todos los integrantes de una sala.
     */
    void mensajeSala(std::string sala, std::string mensaje);

    /**
     * @brief Solicita salir de una sala.
     */
    void abandonarSala(std::string sala);

    /**
     * @brief Envía la notificación de desconexión voluntaria del cliente.
     */
    void desconectarse();

};
#endif