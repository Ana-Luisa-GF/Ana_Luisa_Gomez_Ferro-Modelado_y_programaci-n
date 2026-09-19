#ifndef CONSTRUCTOR_MENSAJES
#define CONSTRUCTOR_MENSAJES

#include <string>
#include <map>
#include <vector>

/**
 * @struct MensajeProtocolo
 * @brief Estructura de datos que representa el contenido de un mensaje del protocolo.
 * 
 * Funciona como un contenedor para almacenar la información recibida o por enviar 
 * entre el cliente y el servidor. 
 */
struct MensajeProtocolo {
    bool valido = false; /**< Bandera que indica si el JSON fue parseado correctamente o si el mensaje es válido. */                               
    std::map<std::string, std::string> datos; /**<Diccionario que contiene los campos del JSON que solo usan cadenas simples*/        
    std::vector<std::string> usernames; /**< Colección de nombres de usuario (utilizada para crear un cuarto). */
    std::map<std::string, std::string> users; /**< Mapa de usuarios y su estado actual. */
};

/**
 * @class ConstructorMensajes
 * @brief Clase encargada del armado y desarmado de mensajes JSON.
 */
class ConstructorMensajes {
public:
    /**
     * @brief Arma una cadena JSON a partir de una estructura MensajeProtocolo.
     * @param msg Referencia a la estructura MensajeProtocolo con los datos a empaquetar.
     * @return std::string Cadena JSON.
     * 
     * Recorre las estructuras internas de 'msg' (datos, usernames, users) y utiliza la librería cJSON 
     * para construir el texto JSON correspondiente.
     */
    static std::string armarMensaje(const MensajeProtocolo& msg);
    
    /**
     * @brief Desarma una cadena de texto en formato JSON y la convierte en una estructura MensajeProtocolo.
     * @param json Cadena de texto recibida.
     * @return MensajeProtocolo Objeto con los datos parseados y la bandera 'valido' activada si fue exitoso.
     * 
     * Analiza sintácticamente el JSON recibido usando cJSON. Si la cadena está corrupta o mal formada, 
     * retorna un MensajeProtocolo con 'valido = false'.
     */
    static MensajeProtocolo desarmarMensajes(const std::string& json);
};

#endif
