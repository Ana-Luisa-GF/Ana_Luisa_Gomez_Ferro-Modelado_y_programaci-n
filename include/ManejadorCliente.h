#ifndef MANEJADORCLIENTE
#define MANEJADORCLIENTE
#include "ConstructorMensajes.h"
#include "DatosCliente.h"
#include <map>
#include <string>
#include <vector>

class ManejadorEstados;

class ManejadorCliente{

private:
    datosCliente cliente;
    std::string buffer_acumulador;
    bool ejecutando;
    ManejadorEstados& contenedor;
    
    std::string recibirMensaje();

    void identificarCliente(MensajeProtocolo &msg);
    void cambiarEstado(MensajeProtocolo &msg);
    void desconectarcliente ();
    void getListaUsuarios();
    
public:

    ManejadorCliente(int sck_cliente, ManejadorEstados& estado);
    ~ManejadorCliente();

    void escuchar();
    void escucharCliente();
    void limpiarCadena(std::string &cadena);
    void descifrarMensaje(std::string mensaje);
    
};
#endif
