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
