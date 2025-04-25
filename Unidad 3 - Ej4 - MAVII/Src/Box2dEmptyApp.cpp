#include "Game.h" 
#include <tchar.h>

using namespace sf; 

int _tmain(int argc, _TCHAR* argv[])
{
    // Crear el objeto de la clase Game
    Game* Juego;
    Juego = new Game(800, 600, "Unidad 3 - Ejercicio 4 - MAVII");
    Juego->Loop(); // Ejecutar el bucle principal del juego

    return 0; // Retorna 0 indicando que el programa se ha ejecutado correctamente
}
