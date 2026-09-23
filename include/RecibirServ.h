#ifndef RECIBIR_SERV_H
#define RECIBIR_SERV_H
#include "ConstructorMensajes.h"
#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"

class RecibirServ{
    private:

    std::string buffer_acumulador;
    bool ejecutando;
    int clientSocket;
    alcanceCliente datos;


    void limpiarCadena(std::string &cadena);
    std::string encontrarCampo(std::string campo, MensajeProtocolo msg); 

    void agregarUsername(MensajeProtocolo msg);

    public:

    RecibirServ( int clientSocket, alcanceCliente datos);
    void escucha();
    std::string recibirMensaje();
    void descifrarMensaje(std::string mensaje_servidor);

};
#endif
