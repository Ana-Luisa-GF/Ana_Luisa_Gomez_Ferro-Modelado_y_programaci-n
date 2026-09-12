#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>


class servidor{
private:

    int sockfd; //descriptor de archivo (identificador del socket)
    int puerto;
    struct sockaddr_in direccion; //tiene la IP, puerto y familia
    bool ejecutando;
    std::map<std::string, int> diccionario_clientes; //a futuro cuando autentique usuarios
    std::vector<int> sockets_clientes;

    std::string recibir_mensaje(int sock_clente);

    void limpiarCadena(std::string &cadena);

public:

    servidor(int puerto);
    ~servidor();

    void escuchar();
    bool iniciarServidor();
};

#endif
