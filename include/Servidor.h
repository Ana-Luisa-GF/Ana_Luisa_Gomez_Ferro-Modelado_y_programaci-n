#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "ManejadorEstados.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <gtest/gtest.h>


class Servidor{
private:

    FRIEND_TEST(ServidorTest, GuardaraCliente);
    FRIEND_TEST(ServidorTest, RechazaNombreRepetido);

    int sockfd; //descriptor de archivo (identificador del socket)
    int puerto;
    struct sockaddr_in direccion; //tiene la IP, puerto y familia
    bool ejecutando;
    ManejadorEstados contenedor;

public:

    Servidor(int puerto);
    ~Servidor();

    void escuchar();
    bool iniciarServidor();
    void aceptarCliente(int cliente_socket); //esto va a hacer el hilo
};

#endif
