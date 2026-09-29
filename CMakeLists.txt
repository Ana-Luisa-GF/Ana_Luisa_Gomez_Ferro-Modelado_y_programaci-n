cmake_minimum_required(VERSION 3.10)

project(proyecto_chat DESCRIPTION "Chat con cliente y servidor" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# archivos compartidos
set(FUENTES_COMUNES
    ${PROJECT_SOURCE_DIR}/src/comunes/cJSON.c
    ${PROJECT_SOURCE_DIR}/src/comunes/ConstructorMensajes.cpp
)

#compilacion del servidor
add_executable(servidor
${PROJECT_SOURCE_DIR}/src/servidor/main.cpp
    ${PROJECT_SOURCE_DIR}/src/servidor/Servidor.cpp
    ${PROJECT_SOURCE_DIR}/src/servidor/ManejadorCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/servidor/ManejadorEstados.cpp
    ${FUENTES_COMUNES}
)

target_include_directories(servidor PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/comunes)

find_package(Threads REQUIRED)
target_link_libraries(servidor PRIVATE Threads::Threads)

#compilacion del ciente
add_executable(cliente
    ${PROJECT_SOURCE_DIR}/src/cliente/mainCliente.cpp  
    ${PROJECT_SOURCE_DIR}/src/cliente/Cliente.cpp
    ${PROJECT_SOURCE_DIR}/src/cliente/AlcanceCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/cliente/PedirServ.cpp
    ${PROJECT_SOURCE_DIR}/src/cliente/RecibirServ.cpp
    ${PROJECT_SOURCE_DIR}/src/cliente/VistaCliente.cpp
    ${PROJECT_SOURCE_DIR}/src/cliente/ControladorCliente.cpp
    ${FUENTES_COMUNES}
)

target_include_directories(cliente PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/comunes)
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

find_package(GTest REQUIRED)

#Test.
add_executable(test_servidor 
    test/test_servidor.cpp
    src/servidor/Servidor.cpp
    src/servidor/ManejadorCliente.cpp
    src/servidor/ManejadorEstados.cpp
    src/comunes/ConstructorMensajes.cpp
    src/comunes/cJSON.c
)

target_include_directories(test_servidor PRIVATE 
    include 
    include/servidor 
    include/comunes
)

target_link_libraries(test_servidor PRIVATE GTest::gtest GTest::gtest_main pthread)

# Ejecución de los test.
add_custom_command(
    TARGET test_servidor POST_BUILD
    COMMAND $<TARGET_FILE:test_servidor>
    COMMENT "Ejecutando pruebas de Servidor..."
)

add_dependencies(servidor test_servidor)