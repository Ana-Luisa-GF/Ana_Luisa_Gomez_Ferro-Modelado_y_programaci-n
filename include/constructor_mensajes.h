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

class constructor_mensajes {
public:
    
    static std::string armarMensaje(const std::map<std::string, std::string>& datos, 
                                     const std::vector<std::string>& usernames = {},
                                     const std::map<std::string, std::string>& users = {});
    
    static MensajeProtocolo desarmarMensajes(const std::string& json);
};

#endif
