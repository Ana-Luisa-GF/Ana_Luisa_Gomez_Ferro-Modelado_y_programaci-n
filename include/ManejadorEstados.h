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

        using cuartos = std::unordered_map<std::string, std::unordered_map<std::string, datosCliente>>;

        cuartos contenedor_cuartos; 
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
         
        bool hayCliente(const std::string& username) const;

        datosCliente darCliente(const std::string& username) const;
        
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

        bool crearSala(const std::string& roomname, datosCliente cliente);
        bool haySala(const std::string& roomname) const;
        void actualizarCliente(datosCliente cliente);
        bool agregarInvitacion(const std::string& username, const std::string& sala);
        void entrarSala(const std::string& username, const std::string& roomname);
        std::vector<datosCliente> cuartoUsuarios(const std::string& roomname)const;
        
};
#endif
