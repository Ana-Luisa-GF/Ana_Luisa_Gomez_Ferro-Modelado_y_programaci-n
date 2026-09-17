#ifndef CONSTRUCTOR_MENSAJES
#define CONSTRUCTOR_MENSAJES

#include <string>
#include <map>
#include <vector>

struct MensajeProtocolo {
    bool valido = false;                                
    std::map<std::string, std::string> datos;         
    std::vector<std::string> usernames;                
    std::map<std::string, std::string> users;        
};

class ConstructorMensajes {
public:
    
    static std::string armarMensaje(const MensajeProtocolo& msg);
    
    static MensajeProtocolo desarmarMensajes(const std::string& json);
};

#endif
