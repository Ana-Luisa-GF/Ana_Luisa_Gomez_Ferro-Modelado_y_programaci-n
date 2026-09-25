#ifndef PEDIR_SERV_H
#define PEDIR_SERV_H
#include "ConstructorMensajes.h"
#include <map>
#include <string>
#include <vector>
#include "AlcanceCliente.h"

class PedirServ{

    private:
        int clientSocket;

    public:
    PedirServ(int ClientSocket);
    ~PedirServ()=default;

    void identificarse(std::string username);
    void cambiarStatus(std::string status);
    void getListaUsuarios();
    void mensajePrivado(std::string user, std::string mensaje);
    void mensajePublico(std::string mensaje);
    void crearSala(std::string sala);
    void invitarSala(std::string sala, std::vector<std::string> invitados);
    void unirseSala(std::string sala);
    void getUsuariosSala(std::string sala);
    void mensajeSala(std::string sala, std::string mensaje);
    void abandonarSala(std::string sala);
    void desconectarse();


};
#endif
