#ifndef CONTROLADOR_CLIENTE_H
#define CONTROLADOR_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "cliente/AlcanceCliente.h"
#include "cliente/PedirServ.h"

class VistaCliente;

/**
 * @brief Controlador que coordina la interfaz de usuario, el modelo de datos y el envío de peticiones.
 */
class ControladorCliente {

private:
    bool ejecutando;
    VistaCliente* vista;
    AlcanceCliente* datos; 
    PedirServ* pedirServ;

public:

    /**
     * @brief Inicializa el controlador.
     */
    ControladorCliente(AlcanceCliente* datos, PedirServ* pedirServ);

    /**
     * @brief Asigna la vista vinculada al controlador.
     */
    void setVista(VistaCliente* vista);

    /**
     * @brief Bucle que mantiene la interacción del usuario con la terminal.
     */
    void escuhar_cliente();

    /**
     * @brief Captura y envía el nombre de usuario introducido.
     * @param user Nombre de usuario.
     */
    void capturarUsername(std::string user);

    /**
     * @brief Procesa el mensaje del cliente, procesa comandos como /msg, /crear_sala, /ayuda, etc y envía la petición.
     * @param mensaje Mensaje del usuario.
     */
    void capturarInstruccion(std::string mensaje);

    //Metodos que procesan respuestas

    /**
     * @brief Notifica que el nombre de usuario ya está ocupado.
     */
    void usernameExistente(const std::string& username);

    /**
     * @brief Notifica la correcta identificación del cliente.
     */
    void identificacionExitosa();

    /**
     * @brief Notifica la llegada de un nuevo usuario al chat.
     */
    void nuevoUsusario(const std::string& username);

    /**
     * @brief Notifica el cambio de estado de un usuario.
     */
    void usuarioCambioStatus(const std::string& username, const std::string& status);

    /**
     * @brief Manda la lista completa de usuarios conectados.
     */
    void listaUsuarios(const std::map<std::string, std::string>& lista);
    
    /**
     * @brief Notifica un mensaje privado.
     */
    void mensajePrivado(const std::string& usuario, const std::string& mensaje);

    /**
     * @brief Notifica el fallo al enviar un mensaje privado.
     */
    void falloMensajePrivado(const std::string& user);

    /**
     * @brief Notifica un mensaje público.
     */
    void recibirMensajePublico(const std::string& usuario, const std::string& mensaje);
    
    /**
     * @brief Confirma la creación de una sala de chat.
     */
    void salaCreada(const std::string& sala);

    /**
     * @brief Notifica que la sala especificada ya existe.
     */
    void falloSalaExistente(const std::string& sala);

    /**
     * @brief Notifica una invitación para unirse a una sala.
     */
    void nuevaInvitación(const std::string& invitacion, const std::string& username);

    /**
     * @brief Notifica que la sala consultada no existe.
     */
    void salaNoExiste(const std::string& sala);

    /**
     * @brief Notifica que el usuario especificado no existe.
     */
    void usuarioNoExiste(const std::string& usuario);
    
    /**
     * @brief Confirma el ingreso exitoso a una sala.
     */
    void entroSala(const std::string& sala);

    /**
     * @brief Notifica que el usuario no tiene invitación para acceder a la sala.
     */
    void usuarioNoInvitado(const std::string& sala);

    /**
     * @brief Manda la lista de integrantes de una sala.
     */
    void listaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista);

    /**
     * @brief Notifica que un usuario abandono una sala.
     */
    void usuarioFueraDeSala(const std::string& sala);
    
    /**
     * @brief Notifica el ingreso de un nuevo integrante a la sala.
     */
    void nuevoUsusarioSala(const std::string& username, const std::string& sala);

    /**
     * @brief Notifica un mensaje enviado dentro de una sala.
     */
    void recibirMensajeSala(const std::string& sala, const std::string& user, const std::string& texto);

    /**
     * @brief Notifica que un usuario abandonó la sala.
     */
    void usuarioAbandonoSala(const std::string& sala, const std::string& user);

    /**
     * @brief Notifica la desconexión del usuario.
     */
    void usuarioDesconectado(const std::string& user);

    /**
     * @brief Notifica un error o fallo en la ejecución de una operación.
     */
    void operacionInvalida();

};
#endif