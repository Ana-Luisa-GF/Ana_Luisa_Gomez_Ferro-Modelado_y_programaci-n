#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

class servidor{
private:

    int sockfd; //descriptor de archivo (identificador del socket)
    int puerto;
    struct sockaddr_in direccion; //tiene la IP, puerto y familia

public:

    servidor(int puerto);

    ~servidor();

    bool iniciarServidor();
};

#endif
