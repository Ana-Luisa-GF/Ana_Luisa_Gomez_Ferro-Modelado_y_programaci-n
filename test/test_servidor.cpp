#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Servidor.h"

/*Test que comprueba si se inicia el servidor correctamente con un puerto valido.*/
TEST(ServidorTest, ServidorConPuertoValido) {
    Servidor servidor(1234);
    EXPECT_TRUE(servidor.iniciarServidor());
}

/*Test que comprueba que no se inicie un servidor con un puerto invalido.*/
TEST(ServidorTest, ServidorConPuertoInvalido) {
    Servidor servidor(80);
    EXPECT_FALSE(servidor.iniciarServidor());
}

/*Test que comprueba que no se incie un sevidor con un puerto en uso.*/
TEST(ServidorTest,ServidorConPuertoEnUso) {
    Servidor servidor1(1234);
    ASSERT_TRUE(servidor1.iniciarServidor()); 

    Servidor servidor2(1234);
    EXPECT_FALSE(servidor2.iniciarServidor()); 
}

/*Test que comprueba que se destruya correctamente un servidor y libere el puerto.*/
TEST(ServidorTest, DestructorLiberaElPuerto) { 
    //Se crea y se destruye el servidor (y se libera el puerto).
    {
        Servidor servidor1(1234);
        ASSERT_TRUE(servidor1.iniciarServidor());
    }
   
    Servidor servidor2(1234);
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

    Servidor servidor(1234);
    servidor.aceptarCliente(socket_servidor);

    //vemos que no e guarden los espacios
    EXPECT_TRUE(servidor.diccionario_clientes.find("nombre\r\n") == servidor.diccionario_clientes.end());

    //vemos que se guarde el nombre del cliente con su respectivo socket
    ASSERT_NE(servidor.diccionario_clientes.find("nombre"), servidor.diccionario_clientes.end());
    EXPECT_EQ(servidor.diccionario_clientes.at("nombre"), socket_servidor);


    close(sv[0]);
    close(sv[1]);
}
