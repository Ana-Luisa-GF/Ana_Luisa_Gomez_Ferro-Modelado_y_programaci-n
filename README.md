Proyecto 1 - Creación de una aplicación chat (Servidor y Cliente) 
Nombre: Gomez Ferro Ana Luisa 
Materia: Modelado y Programación

Descripción del proyecto.
Aplicacion chat conformada por un programa Servidor y un programa Cliente desarrollado en C++17 que gestiona conexiones TCP en con concurrencia e intercambia mensajes formateados en JSON mediante sockets POSIX.

Teclonogías usadas.
Lenguajes: C++17 y C11
Sistema de construcción: CMake (v3.10+) y Make
Procesamiento para JSON: cJSON (con los archivos src/cJSON.c e include/cJSON.h)
Generación de documentación: Doxygen & Graphviz
Compilador:se requiere gcc y g++

Compilación y Ejecución (comandos).
Para preparar el entorno: cmake -B build
Para compilar: cmake --build build
Para compilar y generar la documentación: cmake --build build --target doc
Para ejecutar el servidor:./build/servidor

