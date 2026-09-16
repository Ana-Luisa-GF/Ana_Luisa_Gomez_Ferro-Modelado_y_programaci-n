#include "constructor_mensajes.h"
#include "cJSON.h"
#include <string>
#include <map>
#include <vector>

std::string constructor_mensajes::armarMensaje(const std::map<std::string, std::string>& datos, 
                                                 const std::vector<std::string>& usernames,
                                                 const std::map<std::string, std::string>& users){

            cJSON *json = cJSON_CreateObject(); 

            for (const auto& par : datos) {
                cJSON_AddStringToObject(json, par.first.c_str() , par.second.c_str());
            }

            if (!usernames.empty()) {
                cJSON *arreglo = cJSON_CreateArray();

                for (const auto& usr : usernames) {
                    cJSON_AddItemToArray(arreglo, cJSON_CreateString(usr.c_str()));
                }
                cJSON_AddItemToObject(json, "usernames", arreglo);
            }

            if(!users.empty()){
                cJSON *usuarios = cJSON_CreateObject(); 
                for (const auto& par : users) {
                    cJSON_AddStringToObject(usuarios, par.first.c_str() , par.second.c_str());
                }
                cJSON_AddItemToObject(json, "users", usuarios); 
            }

            char *texto = cJSON_PrintUnformatted(json);
            std::string mensaje_final = std::string(texto) + "\r\n";

            cJSON_free(texto); 
            cJSON_Delete(json);  

            return mensaje_final;
}

MensajeProtocolo constructor_mensajes::desarmarMensajes(const std::string& json_text){

    MensajeProtocolo mensaje;
    cJSON *json = cJSON_Parse(json_text.c_str());

    if(json == NULL){
        mensaje.valido = false; 
        return mensaje;
    }

    mensaje.valido = true;

    cJSON *elemento = json->child;

    std::string llave="";

    while(elemento != NULL){
        llave = elemento->string ? elemento->string : "";
        if (elemento->valuestring)
        if(llave !="users" &&  llave!="usernames"){
            
            mensaje.datos[llave] = elemento->valuestring;
            elemento = elemento->next;
            continue;
        }

        if(llave == "users" && cJSON_IsObject(elemento)){
            cJSON *usuarios = NULL;

            cJSON_ArrayForEach(usuarios, elemento) 
                mensaje.users[usuarios->string] = usuarios->valuestring;
            
            elemento = elemento->next;
            continue;
        }

        cJSON *usr_name = NULL;
            cJSON_ArrayForEach(usr_name, elemento) 
                if (cJSON_IsString(usr_name) && usr_name->valuestring) 
                    mensaje.usernames.push_back(usr_name->valuestring);
        elemento = elemento->next;
    }    

    cJSON_Delete(json);
    return mensaje;
}

