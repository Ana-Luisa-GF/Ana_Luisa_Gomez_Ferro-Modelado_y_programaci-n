#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>
#include "servidor.h"

/*Test que comprueba si se inicia el servidor correctamente con un puerto valido.*/
TEST(ServidorTest, ServidorConPuertoValido) {
    servidor servidor(1234);
    EXPECT_TRUE(servidor.iniciarServidor());
}

/*Test que comprueba que no se inicie un servidor con un puerto invalido.*/
TEST(ServidorTest, ServidorConPuertoInvalido) {
    servidor servidor(80);
    EXPECT_FALSE(servidor.iniciarServidor());
}

/*Test que comprueba que no se incie un sevidor con un puerto en uso.*/
TEST(ServidorTest,ServidorConPuertoEnUso) {
    servidor servidor1(1234);
    ASSERT_TRUE(servidor1.iniciarServidor()); 

    servidor servidor2(1234);
    EXPECT_FALSE(servidor2.iniciarServidor()); 
}

/*Test que comprueba que se destruya correctamente un servidor y libere el puerto.*/
TEST(ServidorTest, DestructorLiberaElPuerto) { 
    //Se crea y se destruye el servidor (y se libera el puerto).
    {
        servidor servidor1(1234);
        ASSERT_TRUE(servidor1.iniciarServidor());
    }
   
    servidor servidor2(1234);
    // Verificamos que pueda usar el puerto sin problema.
    EXPECT_TRUE(servidor2.iniciarServidor());
}

/*Test que comprueba que se guarde correctamente el socket y el username de un cliente*/
TEST(ServidorTest, GuardaraCliente){
    //creamos 2 sockets conectados
    int sv[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sv), 0);

    int socket_servidor = sv[0]; 
    int socket_cliente = sv[1]; 

    std::string username = "nombre\r\n";
    send(socket_cliente, username.c_str(), username.length(), 0);

    servidor servidor(1234);
    servidor.aceptarCliente(socket_servidor);

    //vemos que no e guarden los espacios
    EXPECT_TRUE(servidor.diccionario_clientes.find("nombre\r\n") == servidor.diccionario_clientes.end());

    //vemos que se guarde el nombre del cliente con su respectivo socket
    ASSERT_NE(servidor.diccionario_clientes.find("nombre"), servidor.diccionario_clientes.end());
    EXPECT_EQ(servidor.diccionario_clientes.at("nombre"), socket_servidor);


    close(sv[0]);
    close(sv[1]);
}

/*Test que comprueba que rechaze un username que ya esta en uso*/
TEST(ServidorTest, RechazaNombreRepetido){
    //creamos 2 sockets conectados
    int sv[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sv), 0);

    int socket_servidor = sv[0]; 
    int socket_cliente = sv[1]; 
    std::string username_repetido = "nombre\r\n";
    std::string username_nuevo = "nombre_nuevo\r\n";

    servidor servidor(1234);
    //guardamos un socket falso con el usarname "nombre" 
    servidor.diccionario_clientes["nombre"] = 100;

    //activamos el metodo aceptarCLiente en un hilo
    std::thread hilo_servidor([&]() {
        servidor.aceptarCliente(socket_servidor);
    });
   
    //mandamos el usermane "Nombre" que ya esta ocupado
    send(socket_cliente, username_repetido.c_str(), username_repetido.length(), 0);

    //Pausamos un poco para que el servidor vuelva  apedir el username
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    //mandamos el username nuevo
    send(socket_cliente, username_nuevo.c_str(), username_nuevo.length(), 0);

    hilo_servidor.join();
    
    //verificamos que el socket falso se mantenga
    EXPECT_EQ(servidor.diccionario_clientes.at("nombre"), 100);

    //verificamos que el socket del cliente se halla guardado con el username nuevo
    EXPECT_EQ(servidor.diccionario_clientes.at("nombre_nuevo"), socket_servidor);


    close(sv[0]);
    close(sv[1]);
}


