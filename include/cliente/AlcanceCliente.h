#ifndef ALCANCE_CLIENTE_H
#define ALCANCE_CLIENTE_H

#include <string>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <memory> 
#include <vector>

/**
 * @brief Estructura que almacena la información de un usuario.
 */
struct datosCliente {
    std::string username;
    std::string status;
};

/**
 * @brief Alias para la lista de usuarios global.
 */
using MapUsuarios = std::unordered_map<std::string, std::shared_ptr<datosCliente>>;

/**
 * @brief Alias para las salas de chat.
 */
using MapCuartos = std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<datosCliente>>>;

/**
 * @brief Clase que gestiona el estado de la sesión, usuarios, salas e invitaciones.
 */
class AlcanceCliente {
private:
    std::string username;
    std::string status;
    bool sesionActiva = true;

    MapUsuarios listaUsuarios;
    MapCuartos salas;
    std::unordered_set<std::string> invitaciones;

public:

    /**
     * @brief Constructor por defecto.
     */
    AlcanceCliente() = default;

    /**
     * @brief Establece el nombre de usuario.
     * @param username Nombre de usuario a asignar.
     */
    void setUsername(const std::string& username);

    /**
     * @brief Establece el estado actual del usuario (ACTIVE, BUSY, AWAY).
     * @param status string del nuevo estado.
     */
    void setStatus(const std::string& status);

    /**
     * @brief Actualiza el estado de la sesión del cliente.
     * @param estado_sesion Estado de la sesión.
     */
    void setSesionActiva(const bool& estado_sesion);


    /**
     * @brief Obtiene el nombre de usuario del cliente.
     * @return nombre de usuario.
     */
    const std::string& getUsername();

    /**
     * @brief Obtiene el estado actual del cliente.
     * @return estado.
     */
    const std::string& getStatus();

    /**
     * @brief Indica si la sesión se encuentra activa.
     * @return booleano de sesión activa.
     */
    const bool& getSesionActiva();

    /**
     * @brief Obtiene los nombres de las salas a las que está unido el cliente.
     * @return Vector con los nombres de las salas.
     */
    std::vector<std::string> getSalas();

    /**
     * @brief Obtiene el conjunto de invitaciones pendientes.
     * @return  nonombres de salas invitadas.
     */
    const std::unordered_set<std::string>& getInvitaciones();

    /**
     * @brief Agrega o actualiza un usuario en la lista global.
     * @param username Nombre del usuario.
     * @param status Estado del usuario.
     */
    void agregarUsuario(const std::string& username, const std::string& status);

    /**
     * @brief Registra a un usuario como integrante de una sala.
     * @param nombre_sala Nombre de la sala.
     * @param username Nombre del usuario.
     */
    void agregarUsuarioASala(const std::string& nombre_sala, const std::string& username);

    /**
     * @brief Reemplaza la lista global de usuarios con una nueva versión.
     * @param nuevaLista Mapa con la relación username-status.
     */
    void actualizarListaUsuarios(const std::map<std::string,std::string>& nuevaLista);

    /**
     * @brief Reemplaza la lista de integrantes de una sala con una nueva versión.
     * @param sala Nombre de la sala a actualizar.
     * @param nuevaLista Mapa con la relación username-status.
     */
    void actualizarListaSala(const std::string& sala, const std::map<std::string,std::string>& nuevaLista);

    /**
     * @brief Elimina a un usuario de la lista global.
     * @param username Nombre del usuario.
     */
    void quitarUsuario(const std::string& username);

    /**
     * @brief Remueve a un usuario de una sala.
     * @param nombre_sala Nombre de la sala.
     * @param username Nombre del usuario.
     */
    void quitarUsuarioSala(const std::string& nombre_sala, const std::string& username);

    /**
     * @brief Elimina una sala.
     * @param nombre_sala Nombre de la sala.
     */
    void eliminarSala(const std::string& nombre_sala);

    /**
     * @brief Registra una nueva sala.
     * @param nombre_sala Nombre de la sala.
     */
    void agregarSala(const std::string& nombre_sala);


    /**
     * @brief Agrega una invitación de sala.
     * @param nombre_sala Nombre de la sala.
     */
    void agregarInvitacion(const std::string& nombre_sala);

    /**
     * @brief Remueve una invitación de sala.
     * @param nombre_sala Nombre de la sala.
     */
    void quitarInvitacion(const std::string& nombre_sala);
    
};
#endif