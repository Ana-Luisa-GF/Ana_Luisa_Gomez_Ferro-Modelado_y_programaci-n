#include "ControladorCliente.h"
#include "VistaCliente.h"
#include "ConstructorMensajes.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>


ControladorCliente::ControladorCliente(alcanceCliente datos){
    this->vista = nullptr;
    this->datos = datos;
    ejecutando = true;
}

void ControladorCliente::setVista(VistaCliente* vista){
    this->vista = vista;
}



void ControladorCliente::escuhar_cliente(){
    std::string linea_entrada;

    vista->mensjaeInicio(); //----------------------------------------------------------------------
    std::getline(std::cin, linea_entrada);

    while (ejecutando) {
        std::getline(std::cin, linea_entrada);

        if (linea_entrada.empty())
            continue;
        

        if (linea_entrada == "/salir") {
            ejecutando = false;
            break;
        }
        ejecutando = false;
    }

}

void ControladorCliente::capturarUsername(std::string user){
    std::map<std::string,std::string> mensaje;
    mensaje["type"]="IDENTIFY";
    MensajeProtocolo msg;
    msg.datos = mensaje;

}