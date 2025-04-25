#include "Game.h" 
#include <tchar.h> 

using namespace sf; 

int _tmain(int argc, _TCHAR* argv[])
{
    Game* Juego;
    Juego = new Game(800, 600, "Unidad 3 - Ejercicio 1 - MAVII");
    Juego->Loop(); // Ejecutar el bucle principal del juego

    return 0; 
}
