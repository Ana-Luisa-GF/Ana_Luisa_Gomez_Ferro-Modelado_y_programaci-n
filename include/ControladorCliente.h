#ifndef CONTROLADOR_CLIENTE_H
#define CONTROLADOR_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"
#include "PedirServ.h"

class VistaCliente;

class ControladorCliente{

private:
    bool ejecutando;
    VistaCliente* vista;
    AlcanceCliente* datos; 
    PedirServ* pedirServ;

    void distribuidor_provicional(std::string mensaje);

public:

    ControladorCliente(AlcanceCliente* datos, PedirServ* pedirServ);
    void setVista(VistaCliente* vista);
    void escuhar_cliente();

    void capturarUsername(std::string user);



    //Metodos que captan respuestas

    void usernameExistente(const std::string& username);
    void identificacionExitosa();
    void nuevoUsusario(const std::string& username);
    void usuarioCambioStatus(const std::string& username, const std::string& status);
    void listaUsuarios(const std::map<std::string, std::string>& lista);
    
    void mensajePrivado(const std::string& usuario, const std::string& mensaje);
    void falloMensajePrivado(const std::string& user);
    void recibirMensajePublico(const std::string& usuario, const std::string& mensaje);
    
    void salaCreada(const std::string& sala);
    void falloSalaExistente(const std::string& sala);
    void nuevaInvitación(const std::string& invitacion, const std::string& username);
    void salaNoExiste(const std::string& sala);
    void usuarioNoExiste(const std::string& usuario);
    
    void entroSala(const std::string& sala);
    void usuarioNoInvitado(const std::string& sala);
    void listaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista);
    void usuarioFueraDeSala(const std::string& sala);
    
    void nuevoUsusarioSala(const std::string& username, const std::string& sala);
    void recibirMensajeSala(const std::string& sala, const std::string& user, const std::string& texto);
    void usuarioAbandonoSala(const std::string& sala, const std::string& user);
    void usuarioDesconectado(const std::string& user);

};
#endif
