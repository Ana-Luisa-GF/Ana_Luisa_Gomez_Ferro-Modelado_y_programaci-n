#ifndef ALCANCE_CLIENTE_H
#define ALCANCE_CLIENTE_H

#include <string>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <memory> 

struct datosCliente {
    std::string username;
    std::string status;
};

using MapUsuarios = std::unordered_map<std::string, std::shared_ptr<datosCliente>>;

using MapCuartos= std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<datosCliente>>>;

class AlcanceCliente {
private:
    std::string username;
    std::string status;
    bool sesionActiva= true;

    MapUsuarios listaUsuarios;
    MapCuartos salas;
    std::unordered_set<std::string> invitaciones;

public:

    AlcanceCliente() = default;
    void setUsername(const std::string& username);
    void setStatus(const std::string& status);
    void setSesionActiva(const bool& estado_sesion);

    const std::string getUsername();
    const std::string getStatus();
    const bool getSesionActiva();

    void agregarUsuario(const std::string& username, const std::string& status);
    void agregarUsuarioASala(const std::string& nombre_sala, const std::string& username);
    void actualizarListaUsuarios(const std::map<std::string,std::string>& nuevaLista);
    void actualizarListaSala(const std::string& sala, const std::map<std::string,std::string>& nuevaLista);
    void quitarUsuario(const std::string& username);
    void quitarUsuarioSala(const std::string& nombre_sala, const std::string& username);
    void eliminarSala(const std::string& nombre_sala);
    void agregarSala(const std::string& nombre_sala);
    void agregarInvitacion(const std::string& nombre_sala);
    void quitarInvitacion(const std::string& nombre_sala);
    const std::unordered_set<std::string>& getInvitaciones() const;
};
#endif