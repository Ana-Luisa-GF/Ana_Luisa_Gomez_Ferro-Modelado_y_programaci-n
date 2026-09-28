#ifndef VISTA_CLIENTE_H
#define VISTA_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include "cliente/ControladorCliente.h"

/**
 * @brief Clase encargada de desplegar en la terminal la interfaz gráfica para el ususario.
 */
class VistaCliente {

private:

    ControladorCliente* controlador;

public:

    /**
     * @brief Inicializa la vista y la vincula con el controlador.
     * @param controlador Controlador del programa.
     */
    VistaCliente(ControladorCliente* controlador);

    /**
     * @brief Muestra la pantalla inicial y solicita el nombre de usuario.
     */
    void pantallaInicio();

    /**
     * @brief Muestra el menú con la lista de comandos disponibles y su sintaxis.
     */
    void mostrarGuiaComandos();
    
    /**
     * @brief Muestra el mensaje de bienvenida tras conectarse e identificarse exitosamente.
     * @param username Nombre del usuario.
     */
    void mostrarIdentificacionExitosa(const std::string& username);

    /**
     * @brief Muestra un mensaje indicando que el nombre de usuario ya está ocupado.
     * @param username Nombre de usuario.
     */
    void mostrarUsernameExistente(const std::string& username);

    /**
     * @brief Muestra que un nuevo usuario se ha conectado al servidor.
     * @param username Nombre del nuevo usuario.
     */
    void mostrarNuevoUsuario(const std::string& username);

    /**
     * @brief Muestra el cambio de estado de un usuario.
     * @param username Nombre del usuario.
     * @param status Nuevo estado.
     */
    void mostrarUsuarioCambioStatus(const std::string& username, const std::string& status);

    /**
     * @brief Muestra la lista de usuarios con sus respectivos estados.
     * @param lista Mapa con los usuarios.
     */
    void mostrarListaUsuarios(const std::map<std::string, std::string>& lista);

    /**
     * @brief Muestra un mensaje privado recibido de otro usuario.
     * @param usuario Emisor del mensaje.
     * @param mensaje Contenido del mensaje.
     */
    void mostrarMensajePrivado(const std::string& usuario, const std::string& mensaje);

    /**
     * @brief Muestra un error al no poder enviar un mensaje privado.
     * @param user Nombre del destinatario.
     */
    void mostrarFalloMensajePrivado(const std::string& user);

    /**
     * @brief Muestra un mensaje público.
     * @param usuario Emisor del mensaje.
     * @param mensaje Contenido del mensaje.
     */
    void mostrarMensajePublico(const std::string& usuario, const std::string& mensaje);

    /**
     * @brief Muestra la confirmación de creación de una nueva sala.
     * @param sala Nombre de la sala.
     */
    void mostrarSalaCreada(const std::string& sala);

    /**
     * @brief Muestra un error al intentar crear una sala que ya existe.
     * @param sala Nombre de la sala.
     */
    void mostrarFalloSalaExistente(const std::string& sala);

    /**
     * @brief Muestra la invitación a una sala.
     * @param sala Nombre de la sala.
     * @param username Usuario que envía la invitación.
     */
    void mostrarNuevaInvitacion(const std::string& sala, const std::string& username);

    /**
     * @brief Muestra un error indicando que la sala especificada no existe.
     * @param sala Nombre de la sala buscada.
     */
    void mostrarSalaNoExiste(const std::string& sala);

    /**
     * @brief Muestra un error indicando que el usuario especificado no existe.
     * @param usuario Nombre del usuario buscado.
     */
    void mostrarUsuarioNoExiste(const std::string& usuario);

    /**
     * @brief Muestra que el usuario ha ingresado exitosamente a una sala.
     * @param sala Nombre de la sala.
     */
    void mostrarEntroSala(const std::string& sala);

    /**
     * @brief Muestra un error si el usuario intenta unirse a una sala sin contar con invitación.
     * @param sala Nombre de la sala restringida.
     */
    void mostrarUsuarioNoInvitado(const std::string& sala);

    /**
     * @brief Muestra la lista de usuarios que forman parte de una sala.
     * @param sala Nombre de la sala.
     * @param lista Mapa con los integrantes de la sala y sus estados.
     */
    void mostrarListaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista);

    /**
     * @brief Muestra el aviso de que el usuario no forma parte de una sala.
     * @param sala Nombre de la sala.
     */
    void mostrarUsuarioFueraDeSala(const std::string& sala);

    /**
     * @brief Muestra la lista de salas a las que el usuario está unido actualmente.
     * @param lista Vector con los nombres de las salas.
     */
    void mostrarSalas(const std::vector<std::string>& lista);

    /**
     * @brief Muestra el ingreso de un nuevo integrante a la sala.
     * @param username Nombre del integrante nuevo.
     * @param sala Nombre de la sala.
     */
    void mostrarNuevoUsuarioSala(const std::string& username, const std::string& sala);

    /**
     * @brief Muestra un mensaje recibido dentro de una sala.
     * @param sala Nombre de la sala.
     * @param user Remitente del mensaje.
     * @param texto Contenido del mensaje.
     */
    void mostrarMensajeSala(const std::string& sala, const std::string& user, const std::string& texto);

    /**
     * @brief Muestra que un usuario abandono una sala.
     * @param sala Nombre de la sala.
     * @param user Usuario que salió de la sala.
     */
    void mostrarUsuarioAbandonoSala(const std::string& sala, const std::string& user);

    /**
     * @brief Muestra la desconexión de un usuario del chat.
     * @param user Nombre del usuario desconectado.
     */
    void mostrarUsuarioDesconectado(const std::string& user);

    /**
     * @brief Muestra las invitaciones pendientes del cliente.
     * @param lista Conjunto con las salas invitadas.
     */
    void mostrarInvitaciones(const std::unordered_set<std::string>& lista);

    /**
     * @brief Muestra un mensaje de error cuando una operación es inválida.
     */
    void mostrarOperacionInvalida();
};
#endif