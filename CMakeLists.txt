cmake_minimum_required(VERSION 3.10)

project(proyecto_chat DESCRIPTION "Chat con cliente y servidor" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
