#ifndef VISTA_CLIENTE_H
#define VISTA_CLIENTE_H

#include <map>
#include <string>
#include <vector>
#include "ControladorCliente.h"

class VistaCliente{

private:

    ControladorCliente* controlador;

public:

    VistaCliente(ControladorCliente* controlador);

    void pantallaInicio();

    void mostrarIdentificacionExitosa(const std::string& username);
    void mostrarUsernameExistente(const std::string& username);
    void mostrarNuevoUsuario(const std::string& username);
    void mostrarUsuarioCambioStatus(const std::string& username, const std::string& status);
    void mostrarListaUsuarios(const std::map<std::string, std::string>& lista);

    void mostrarMensajePrivado(const std::string& usuario, const std::string& mensaje);
    void mostrarFalloMensajePrivado(const std::string& user);
    void mostrarMensajePublico(const std::string& usuario, const std::string& mensaje);

    void mostrarSalaCreada(const std::string& sala);
    void mostrarFalloSalaExistente(const std::string& sala);
    void mostrarNuevaInvitacion(const std::string& sala, const std::string& username);
    void mostrarSalaNoExiste(const std::string& sala);
    void mostrarUsuarioNoExiste(const std::string& usuario);

    void mostrarEntroSala(const std::string& sala);
    void mostrarUsuarioNoInvitado(const std::string& sala);
    void mostrarListaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista);
    void mostrarUsuarioFueraDeSala(const std::string& sala);

    void mostrarNuevoUsuarioSala(const std::string& username, const std::string& sala);
    void mostrarMensajeSala(const std::string& sala, const std::string& user, const std::string& texto);
    void mostrarUsuarioAbandonoSala(const std::string& sala, const std::string& user);
    void mostrarUsuarioDesconectado(const std::string& user);
};
#endif
