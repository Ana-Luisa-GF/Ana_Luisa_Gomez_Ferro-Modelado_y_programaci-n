#ifndef MANEJADORCLIENTE
#define MANEJADORCLIENTE
#include "ConstructorMensajes.h"
#include "DatosCliente.h"
#include <map>
#include <string>
#include <vector>

// Declaración adelantada de la clase para evitar inclusiones circulares
class ManejadorEstados;

/**
 * @class ManejadorCliente
 * @brief Atiende la comunicación y el estado de una conexión individual (un solo cliente).
 * 
 * Cada instancia de esta clase se ejecuta dentro de su propio hilo de ejecución, creado 
 * por el servidor principal. Administra la recepción del flujo de datos en el socket, 
 * procesa las solicitudes en formato JSON y coordina las respuestas hacia el cliente o las 
 * actualizaciones dentro del contenedor compartido ManejadorEstados.
 */
class ManejadorCliente{

private:
    datosCliente cliente; /**< Estructura con la información del cliente (socket, IP, username). */
    std::string buffer_acumulador; /**< Búfer de memoria para guardar mensajes incompletos o partidos del socket. */
    bool ejecutando; /**< Bandera de control para mantener activo el bucle de lectura del socket. */
    ManejadorEstados& contenedor; /**< contenedor global de estados (compartido entre todos los hilos). */
    
    /**
     * @brief Lee bloques de bytes del socket llamando a la funcion recv().
     * @return std::string Cadena con los datos leídos del socket o vacía si ocurre un error/desconexión.
     */
    std::string recibirMensaje();

    /**
     * @brief Procesa el mensaje inicial registro del usuario.
     * @param msg Estructura con los datos  de la petición "IDENTIFY".
     * 
     * Valida que el nombre de usuario no esté duplicado en el ManejadorEstados, que no mida 
     * mas de 8 caracteres, registra la cliente, notifica a los otros clientes y responde 
     * con una confirmación o rechazo en formato JSON.
     */
    void identificarCliente(MensajeProtocolo &msg);

    /**
     * @brief Procesa la solicitud para cambiar el estado del cliente.
     * @param msg Estructura del protocolo con los datos de la petición "STATUS".
     */
    void cambiarEstado(MensajeProtocolo &msg);

    /**
     * @brief Finaliza la sesión del cliente y libera los recursos.
     * 
     * Remueve la cliente del ManejadorEstados y cierra el socket con close(). 
     * Termina bucle principal para que el hilo acabe.
     */
    void desconectarcliente ();

    /**
     * @brief Consulta la lista de usuarios conectados y la transmite al cliente.
     * 
     * Construye una respuesta JSON mediante ConstructorMensajes con la lista de usuarios activos 
     * obtenida de ManejadorEstados y la envía por el socket.
     */
    void getListaUsuarios();
    
public:
    /**
     * @brief Constructor de ManejadorCliente.
     * @param sck_cliente Descriptor de archivo del socket devuelto por la función accept().
     * @param estado Referencia a la instancia única del ManejadorEstados central.
     */
    ManejadorCliente(int sck_cliente, ManejadorEstados& estado);

    /**
     * @brief Destructor de ManejadorCliente.
     */
    ~ManejadorCliente();

    /**
     * @brief Inicia el ciclo principal para escuchar las peticiones del cliente.
     * 
     * Captura los mensajes del cliente y los procesa con desifrarMensaje().
     */
    void escuchar();

    /**
     * @brief Elimina caracteres los caracteres '\r', '\n' de una cadena.
     * @param cadena Referencia al cadena que será limpiada.
     */
    void limpiarCadena(std::string &cadena);

    /**
     * @brief Parsea el JSON entrante y delega la acción al método auxiliar correspondiente.
     * @param mensaje Cadena de texto en formato JSON recibida a través del socket.
     * 
     * Utiliza los métodos estáticos de ConstructorMensajes para desarmar el JSON y evalúa 
     * el campo "type" para invocar a identificarCliente(), cambiarEstado(), etc.
     */
    void descifrarMensaje(std::string mensaje);
    
};
#endif
