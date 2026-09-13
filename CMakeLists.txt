cmake_minimum_required(VERSION 3.10)

project(proyecto_chat DESCRIPTION "Chat con cliente y servidor" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(servidor
 ${PROJECT_SOURCE_DIR}/src/main.cpp
 ${PROJECT_SOURCE_DIR}/src/servidor.cpp)

target_include_directories(servidor PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(Threads REQUIRED)
target_link_libraries(servidor PRIVATE Threads::Threads)

