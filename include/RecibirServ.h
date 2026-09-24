#ifndef RECIBIR_SERV_H
#define RECIBIR_SERV_H
#include "ConstructorMensajes.h"
#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"

class ControladorCliente;

class RecibirServ{
private:

    std::string buffer_acumulador;
    bool ejecutando;
    int clientSocket;
    AlcanceCliente* datos;
    ControladorCliente* controlador;


    void limpiarCadena(std::string &cadena);
    std::string encontrarCampo(std::string campo, MensajeProtocolo msg); 


    void identificarse(MensajeProtocolo msg);
    void nuevoUsuario(MensajeProtocolo msg);

    void usuarioCambioStatus(MensajeProtocolo msg);
    void listaUsuarios(MensajeProtocolo msg);
    void recibirMensajePrivado(MensajeProtocolo msg);
    void falloMensajePrivado(MensajeProtocolo msg);
    void recibirMensajePublico(MensajeProtocolo msg);
    void crearSala(MensajeProtocolo msg);
    void recibirInvitacionSala(MensajeProtocolo msg);
    void falloInvitarSala(MensajeProtocolo msg);
    void unirseSala(MensajeProtocolo msg);
    void listaUsuariosSala(MensajeProtocolo msg);
    void falloListaUsuariosSala(MensajeProtocolo msg);
    void nuevoUsuarioSala(MensajeProtocolo msg);
    
    void mensajeSala(MensajeProtocolo msg);
    void falloMensajeSala(MensajeProtocolo msg);
    void usuarioAbandonoSala(MensajeProtocolo msg);
    void falloAbandonarSala(MensajeProtocolo msg);
    void usuarioDesconectado(MensajeProtocolo msg);

public:

    RecibirServ( int clientSocket, AlcanceCliente* datos,ControladorCliente* controlador);

    void escucha();
    std::string recibirMensaje();
    void descifrarMensaje(std::string mensaje_servidor);



};
#endif
