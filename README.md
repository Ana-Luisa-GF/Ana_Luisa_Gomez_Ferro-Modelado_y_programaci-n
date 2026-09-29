Proyecto 1 - Creación de una aplicación chat (Servidor y Cliente)

Nombre: Gomez Ferro Ana Luisa

Materia: Modelado y Programación

Descripción del proyecto.

Aplicación chat conformada por un programa Servidor y un programa Cliente desarrollada en C++17 que gestiona conexiones TCP concurrentes e intercambia mensajes formateados en JSON mediante sockets POSIX. El servidor maneja múltiples clientes, salas de conversación y reenvío de mensajes, mientras que el cliente ofrece una interfaz en consola basada en la arquitectura MVC.

Tecnologías usadas.

Lenguajes: C++17 y C11

Sistema de construcción: CMake (v3.10+) y Make

Procesamiento para JSON: cJSON (con los archivos src/cJSON.c e include/cJSON.h)

Pruebas unitarias: Google Test (GTest)

Generación de documentación: Doxygen y Graphviz

Compilador: se requiere gcc y g++

Compilación y Ejecución (comandos).

Para preparar el entorno:
cmake -B build

Para compilar:
cmake --build build

Para compilar y generar la documentación:
cmake --build build --target doc

Para ejecutar el servidor:
./build/servidor

Para ejecutar el cliente:
./build/cliente
