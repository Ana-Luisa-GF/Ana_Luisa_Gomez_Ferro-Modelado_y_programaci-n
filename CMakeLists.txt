cmake_minimum_required(VERSION 3.10)

project(proyecto_chat DESCRIPTION "Chat con cliente y servidor" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#compilacion del servidor
add_executable(servidor
 ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Servidor.cpp
    ${PROJECT_SOURCE_DIR}/src/ManejadorCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/ManejadorEstados.cpp
    ${PROJECT_SOURCE_DIR}/src/ConstructorMensajes.cpp  
    ${PROJECT_SOURCE_DIR}/src/cJSON.c
)

target_include_directories(servidor PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(Threads REQUIRED)
target_link_libraries(servidor PRIVATE Threads::Threads)

#compilacion del ciente
add_executable(cliente
    ${PROJECT_SOURCE_DIR}/src/mainCliente.cpp  
    ${PROJECT_SOURCE_DIR}/src/Cliente.cpp
    ${PROJECT_SOURCE_DIR}/src/AlcanceCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/ConstructorMensajes.cpp
    ${PROJECT_SOURCE_DIR}/src/PedirServ.cpp
    ${PROJECT_SOURCE_DIR}/src/RecibirServ.cpp
    ${PROJECT_SOURCE_DIR}/src/VistaCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/ControladorCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/cJSON.c
)

target_include_directories(cliente PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cliente PRIVATE Threads::Threads)


# Generación de Documentación con Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generando documentación HTML/LaTeX"
    )
else()
    message(STATUS "Doxygen no fue encontrado. La documentación no estara disponible")
endif()
