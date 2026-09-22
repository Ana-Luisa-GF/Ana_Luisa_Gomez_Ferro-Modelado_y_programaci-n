#ifndef MANEJADOR_ESTADOS_H
#define MANEJADOR_ESTADOS_H
#include "DatosCliente.h"
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <shared_mutex>
#include <mutex>

/**
 * @class ManejadorEstados
 * @brief Administra el registro y peticiones de los clientes conectados al servidor.
 * 
 * Esta clase actua como un contenedor en memoria para almacenar la información 
 * de cada usuario activo y los cuartos que existen. Utiliza un exclusión mutua (std::mutex) para 
 * garantizar la integridad de los datos cuando múltiples hilos de clientes acceden o modifican 
 * el estado simultáneamente.
 */
class ManejadorEstados{

    private:

        /**
         * @brief Sobrenombre para la estructura que almacena los cuartos.
         */
        using cuartos = std::unordered_map<std::string, std::unordered_map<std::string, datosCliente>>;

        cuartos contenedor_cuartos; /**< Contenedor global de las salas y sus miembros. */
        std::map<std::string, datosCliente> clientes; /**< Diccionario que asocia el nombre del usuario con su estructura de datos.*/
        mutable std::shared_mutex mutex_clientes; /**< Mutex para proteger el acceso concurrente al diccionario 'clientes'*/
        mutable std::shared_mutex mutex_cuartos;/**< Mutex para proteger el acceso concurrente al diccionario 'contenedor_cuartos'*/
    public:
        /**
         * @brief Constructor por defecto de ManejadorEstados.
         */
        ManejadorEstados();

        /**
         * @brief Destructor de ManejadorEstados.
         */
        ~ManejadorEstados();
        
        /**
         * @brief Verifica si un usuario ya se encuentra registrado.
         * @param username Nombre del usuario a consultar.
         * @return true si el usuario existe, false en caso contrario.
         */
        bool hayCliente(const std::string& username) const;

        /**
         * @brief Obtiene una copia de los datos de un cliente.
         * @param username Nombre de usuario del cliente.
         * @return Estructura datosCliente del usuario.
         */
        datosCliente darCliente(const std::string& username) const;
        
        /**
         * @brief da la lista de descriptores de sockets de todos los usuarios.
         * @return std::vector<int> Lista con los sockets de los usuarios.
         */
        std::vector<int> clientes_mensajePublico()const;

        /**
         * @brief Registra un cliente en el servidor.
         * @param nuevo_cliente Estructura datosCliente con la información del cliente.
         */
        std::vector<int> agregarcliente(datosCliente nuevo_cliente);

        /**
         * @brief Remueve a un cliente de la lista principal de usuarios y de los cuartos a los que pertenecia.
         * @param username Nombre del usuario a eliminar.
         */
        void eliminarCliente(const std::string& username);

        /**
         * @brief Entrega la lista de clientes.
         *@return Diccionario con el nombre de usuario de cada cliente asociado a su estado.
         */
        std::map<std::string, std::string> getListaClientes()const; 

        /**
         * @brief Crea un nuevo cuarto y añade al creador como primer integrante.
         * @param roomname Nombre de la sala.
         * @param cliente Datos del creador de la sala.
         * @return true si la sala se creó con éxito, false si ya existía.
         */
        bool crearSala(const std::string& roomname, datosCliente cliente);

        /**
         * @brief Verifica si una sala ya existe.
         * @param roomname Nombre de la sala.
         * @return true si la sala existe, false en caso contrario.
         */
        bool haySala(const std::string& roomname) const;

        /**
         * @brief Actualiza la información y estado de un cliente en el registro global.
         * @param cliente Estructura datosCliente con los campos actualizados.
         */
        void actualizarCliente(datosCliente cliente);

        /**
         * @brief Registra una invitación a una sala.
         * @param username Nombre del usuario invitado.
         * @param sala Nombre del cuarto al que es invitado.
         */
        void agregarInvitacion(const std::string& username, const std::string& sala);

        /**
         * @brief Mete a un usuario previamente invitado a una sala.
         * @param username Nombre del usuario.
         * @param roomname Nombre de la sala.
         */
        void entrarSala(const std::string& username, const std::string& roomname);

        /**
         * @brief Obtiene la lista de usuarios de una sala.
         * @param roomname Nombre de la sala.
         * @return std::vector<datosCliente> Lista con los datos de cada miembro de la sala.
         */
        std::vector<datosCliente> cuartoUsuarios(const std::string& roomname)const;

        /**
         * @brief Remueve a un cliente de una sala y destruye la sala si queda vacía.
         * @param username Nombre del usuario.
         * @param roomname Nombre de la sala.
         */
        void salirSala(const std::string& username, const std::string& roomname);
        
};
#endif
