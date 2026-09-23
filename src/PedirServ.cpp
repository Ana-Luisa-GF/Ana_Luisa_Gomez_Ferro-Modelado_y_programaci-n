#include "PedirServ.h"
#include <sys/socket.h>

PedirServ::PedirServ(int clientSocket) {
    this->clientSocket = clientSocket;
}

void PedirServ::identificarse(std::string username){
    MensajeProtocolo msg;
    msg.datos["type"]="IDENTIFY";
    msg.datos["username"]=username;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::cambiarStatus(std::string status){
    MensajeProtocolo msg;
    msg.datos["type"]="STATUS";
    msg.datos["status"]=status;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::getListaUsuarios() {
    MensajeProtocolo msg;
    msg.datos["type"] = "USERS";
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::mensajePrivado(std::string user, std::string mensaje) {
    MensajeProtocolo msg;
    msg.datos["type"] = "TEXT";
    msg.datos["username"] = user;
    msg.datos["text"] = mensaje;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::mensajePublico(std::string mensaje) {
    MensajeProtocolo msg;
    msg.datos["type"] = "PUBLIC_TEXT";
    msg.datos["text"] = mensaje;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::crearSala(std::string sala) {
    MensajeProtocolo msg;
    msg.datos["type"] = "NEW_ROOM";
    msg.datos["roomname"] = sala;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::invitarSala(std::string sala, std::vector<std::string> invitados) {
    MensajeProtocolo msg;
    msg.datos["type"] = "INVITE";
    msg.datos["roomname"] = sala;
    msg.usernames = invitados; 
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::unirseSala(std::string sala) {
    MensajeProtocolo msg;
    msg.datos["type"] = "JOIN_ROOM";
    msg.datos["roomname"] = sala;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::getUsuariosSala(std::string sala) {
    MensajeProtocolo msg;
    msg.datos["type"] = "ROOM_USERS";
    msg.datos["roomname"] = sala;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::mensajeSala(std::string sala, std::string mensaje) {
    MensajeProtocolo msg;
    msg.datos["type"] = "ROOM_TEXT";
    msg.datos["roomname"] = sala;
    msg.datos["text"] = mensaje;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::abandonarSala(std::string sala) {
    MensajeProtocolo msg;
    msg.datos["type"] = "LEAVE_ROOM";
    msg.datos["roomname"] = sala;
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}


void PedirServ::desconectarse() {
    MensajeProtocolo msg;
    msg.datos["type"] = "DISCONNECT";
    std::string json = ConstructorMensajes::armarMensaje(msg);
    send(clientSocket, json.c_str(), json.length(), 0);
}