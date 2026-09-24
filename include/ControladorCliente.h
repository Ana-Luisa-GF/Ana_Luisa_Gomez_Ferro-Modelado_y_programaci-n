#ifndef CONTROLADOR_CLIENTE_H
#define CONTROLADOR_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"
#include "VistaCliente.h"


class ControladorCliente{

private:
    bool ejecutando;
    VistaCliente* vista;
    alcanceCliente datos;

public:
    ControladorCliente(alcanceCliente datos);
    void setVista(VistaCliente* vista);
    void escuhar_cliente();

    void capturarUsername(std::string user);



    //Metodos que captan respuestas

    void usernameExistente(std::string username);
    void identificacionExitosa();
    void nuevoUsusario(std::string username);
    void usuarioCambioStatus(std::string username, std::string status);
    void listaUsuarios(std::map<std::string,std::string> lista);
    void mensajePrivado(std::string usuario, std::string menaje);
    void falloMensajePrivado(std::string user);
    void recibirMensajePublico(std::string usuario, std::string menaje);
    void salaCreada(std::string sala);
    void falloSalaExistente(std::string sala);
    void nuevaInvitación(std::string invitacion, std::string username);
    void salaNoExiste(std::string sala);
    void usuarioNoExiste(std::string usuario);
    void entroSala(std::string sala);
    void usuarioNoInvitado(std::string sala);
    void listaUsuariosSala(std::string sala,std::map<std::string,std::string> lista);
    void usuarioFueraDeSala(std::string sala);
    void nuevoUsusarioSala(std::string username,std::string sala);
    void recibirMensajeSala(std::string sala, std::string user, std::string texto);
    void usuarioAbandonoSala(std::string sala, std::string user);
    void usuarioDesconectado(std::string user);

};
#endif
