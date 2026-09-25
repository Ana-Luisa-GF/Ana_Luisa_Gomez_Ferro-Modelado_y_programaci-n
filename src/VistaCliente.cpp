#include "VistaCliente.h"
#include "ControladorCliente.h"
#include <iostream>
#define RESET   "\033[0m"
#define ROJO    "\033[1;31m"
#define VERDE   "\033[1;32m"
#define AMARILLO "\033[1;33m"
#define AZUL    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define GRIS    "\033[1;90m"


VistaCliente::VistaCliente(ControladorCliente* controlador) {
    this->controlador = controlador;
}

 void VistaCliente::pantallaInicio(){
     std::cout << MAGENTA << "CHAT PROYECTOOO :D" << RESET << "\n";
 }

void VistaCliente::mostrarIdentificacionExitosa(const std::string& username) {
    std::cout << VERDE << "[✔] Identificación exitosa. Bienvenido, " << username << "!" << RESET << "\n";
}

void VistaCliente::mostrarUsernameExistente(const std::string& username) {
    std::cout << ROJO << "[✘] Error: El nombre de usuario '" << username << "' ya está en uso." << RESET << "\n";
}

void VistaCliente::mostrarNuevoUsuario(const std::string& username) {
    std::cout << GRIS << "[+] El usuario '" << username << "' se ha conectado al servidor." << RESET << "\n";
}

void VistaCliente::mostrarUsuarioCambioStatus(const std::string& username, const std::string& status) {
    std::cout << AMARILLO << "[⚙] " << username << " cambió su estado a: " << status << RESET << "\n";
}

void VistaCliente::mostrarListaUsuarios(const std::map<std::string, std::string>& lista) {
    std::cout << MAGENTA << "=== LISTA DE USUARIOS CONECTADOS ===" << RESET << "\n";
    for (const auto& [user, status] : lista) {
        std::cout << " • " << user << " (" << status << ")\n";
    }
    std::cout << MAGENTA << "====================================" << RESET << "\n";
}

void VistaCliente::mostrarMensajePrivado(const std::string& usuario, const std::string& mensaje) {
    std::cout << AMARILLO << "[Privado de " << usuario << "]: " << RESET << mensaje << "\n";
}

void VistaCliente::mostrarFalloMensajePrivado(const std::string& user) {
    std::cout << ROJO << "[✘] Error al enviar mensaje privado: El usuario '" << user << "' no existe." << RESET << "\n";
}

void VistaCliente::mostrarMensajePublico(const std::string& usuario, const std::string& mensaje) {
    std::cout << VERDE << "[" << usuario << "]: " << RESET << mensaje << "\n";
}

void VistaCliente::mostrarSalaCreada(const std::string& sala) {
    std::cout << VERDE << "[✔] Sala '" << sala << "' creada exitosamente." << RESET << "\n";
}

void VistaCliente::mostrarFalloSalaExistente(const std::string& sala) {
    std::cout << ROJO << "[✘] Error: La sala '" << sala << "' ya existe." << RESET << "\n";
}

void VistaCliente::mostrarNuevaInvitacion(const std::string& sala, const std::string& username) {
    std::cout << MAGENTA << "[✉] El usuario '" << username << "' te ha invitado a la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarSalaNoExiste(const std::string& sala) {
    std::cout << ROJO << "[✘] Error: La sala '" << sala << "' no existe." << RESET << "\n";
}

void VistaCliente::mostrarUsuarioNoExiste(const std::string& usuario) {
    std::cout << ROJO << "[✘] Error: El usuario '" << usuario << "' no existe." << RESET << "\n";
}

void VistaCliente::mostrarEntroSala(const std::string& sala) {
    std::cout << VERDE << "[✔] Te has unido exitosamente a la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarUsuarioNoInvitado(const std::string& sala) {
    std::cout << ROJO << "[✘] Error: No has sido invitado a la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarListaUsuariosSala(const std::string& sala, const std::map<std::string, std::string>& lista) {
    std::cout << MAGENTA << "=== INTEGRANTES DE LA SALA: " << sala << " ===" << RESET << "\n";
    for (const auto& [user, status] : lista) {
        std::cout << " • " << user << " (" << status << ")\n";
    }
    std::cout << MAGENTA << "==========================================" << RESET << "\n";
}

void VistaCliente::mostrarUsuarioFueraDeSala(const std::string& sala) {
    std::cout << ROJO << "[✘] Error: No formas parte de la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarNuevoUsuarioSala(const std::string& username, const std::string& sala) {
    std::cout << GRIS << "[+] " << username << " se ha unido a la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarMensajeSala(const std::string& sala, const std::string& user, const std::string& texto) {
    std::cout << AZUL << "[" << sala << " | " << user << "]: " << RESET << texto << "\n";
}

void VistaCliente::mostrarUsuarioAbandonoSala(const std::string& sala, const std::string& user) {
    std::cout << AMARILLO << "[-] " << user << " ha salido de la sala '" << sala << "'." << RESET << "\n";
}

void VistaCliente::mostrarUsuarioDesconectado(const std::string& user) {
    std::cout << ROJO << "[-] El usuario '" << user << "' se ha desconectado." << RESET << "\n";
}