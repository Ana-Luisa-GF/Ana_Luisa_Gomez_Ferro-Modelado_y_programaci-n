#include "VistaCliente.h"
#include "ControladorCliente.h"
#include  <cstdio>
#include <iostream>
#include <string>

VistaCliente::VistaCliente(ControladorCliente* controlador) {
    this->controlador = controlador;
}

void VistaCliente::mensjaeInicio(){
    printf("=== CLIENTE DE CHAT EN TERMINAL ===");
    printf("Escribe tu usuario y presiona ENTER:");
}
