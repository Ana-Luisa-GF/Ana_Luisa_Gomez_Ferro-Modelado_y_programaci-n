#ifndef RECIBIR_SERV_H
#define RECIBIR_SERV_H

#include "comunes/ConstructorMensajes.h"
#include <map>
#include <string>
#include <vector>
#include "cliente/AlcanceCliente.h"

class ControladorCliente;

/**
 * @brief Clase encargada de recibir y procesar las respuestas JSON enviadas por el servidor.
 */
class RecibirServ {
private:

    std::string buffer_acumulador;
    bool ejecutando;
    int clientSocket;
    AlcanceCliente* datos;
    ControladorCliente* controlador;

    /**
     * @brief Elimina saltos de línea al final de una cadena.
     * @param cadena Cadena de texto a limpiar.
     */
    void limpiarCadena(std::string &cadena);

    /**
     * @brief Busca el valor de un campo específico dentro de una estructura de mensaje.
     * @param campo Nombre del campo a buscar.
     * @param msg Estructura del mensaje.
     * @return string con el valor del campo.
     */
    std::string encontrarCampo(std::string campo, MensajeProtocolo msg); 


    /**
     * @brief Procesa el resultado del intento de identificación del cliente.
     */
    void identificarse(MensajeProtocolo msg);

    /**
     * @brief Procesa la notificación de un nuevo usuario.
     */
    void nuevoUsuario(MensajeProtocolo msg);

    /**
     * @brief Procesa la actualización de estado de un usuario.
     */
    void usuarioCambioStatus(MensajeProtocolo msg);

    /**
     * @brief Procesa la recepción de la lista global de usuarios.
     */
    void listaUsuarios(MensajeProtocolo msg);

    /**
     * @brief Procesa un mensaje privado recibido.
     */
    void recibirMensajePrivado(MensajeProtocolo msg);

    /**
     * @brief Procesa el fallo al intentar enviar un mensaje privado.
     */
    void falloMensajePrivado(MensajeProtocolo msg);

    /**
     * @brief Procesa la recepción de un mensaje publico.
     */
    void recibirMensajePublico(MensajeProtocolo msg);

    /**
     * @brief Procesa la respuesta tras intentar crear una sala.
     */
    void crearSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la recepción de una invitación para unirse a una sala.
     */
    void recibirInvitacionSala(MensajeProtocolo msg);

    /**
     * @brief Procesa el error al fallar la invitación a una sala.
     */
    void falloInvitarSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la respuesta de solicitar unirse a una sala.
     */
    void unirseSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la recepción de la lista de integrantes de una sala.
     */
    void listaUsuariosSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la falla al consultar los integrantes de una sala.
     */
    void falloListaUsuariosSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la llegada de un nuevo integrante a una sala.
     */
    void nuevoUsuarioSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la recepción de un mensaje enviado dentro de una sala.
     */
    void mensajeSala(MensajeProtocolo msg);

    /**
     * @brief Procesa el fallo al intentar enviar un mensaje a una sala.
     */
    void falloMensajeSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la notificación cuando un usuario abandona una sala.
     */
    void usuarioAbandonoSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la falla de abandonar una sala.
     */
    void falloAbandonarSala(MensajeProtocolo msg);

    /**
     * @brief Procesa la desconexión de un usuario.
     */
    void usuarioDesconectado(MensajeProtocolo msg);

    /**
     * @brief Procesa respuestas del servidor que indican una operación no válida.
     */
    void operacionInvalida(MensajeProtocolo msg);

public:

    /**
     * @brief Inicializa el receptor con el socket del cliente, el modelo de datos y el controlador.
     * @param clientSocket Socket del cliente.
     * @param datos Modelo local con la información del cliente.
     * @param controlador Controlador encargado de actualizar la interfaz.
     */
    RecibirServ(int clientSocket, AlcanceCliente* datos, ControladorCliente* controlador);

    /**
     * @brief Bucle principal que lee continuamente datos del socket.
     */
    void escucha();

    /**
     * @brief Lee del socket y captura las cadenas.
     * @return Cadena JSON obtenida del servidor.
     */
    std::string recibirMensaje();

    /**
     * @brief traduce la cadena JSON enviada por el servidor y llamada al metodo correspondiente a la acción.
     * @param mensaje_servidor mensaje recibido del servidor.
     */
    void descifrarMensaje(std::string mensaje_servidor);

};
#endif