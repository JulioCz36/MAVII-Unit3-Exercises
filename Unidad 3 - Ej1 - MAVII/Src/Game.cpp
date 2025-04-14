#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{   
    //dibujar pelota
    sf::CircleShape pelota(5.0f);
    pelota.setFillColor(sf::Color::Yellow);
    pelota.setOrigin(5.0f, 5.0f);
    pelota.setPosition(pelotaBody->GetPosition().x, pelotaBody->GetPosition().y);
    wnd->draw(pelota);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close();
            break;
        }
    }

}

void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); 
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{

    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // ------------------------------------------------------------
    // Según el material, una colisión elástica puede interpretarse como una interacción similar a un resorte,
    // donde la energía cinética se conserva (sin pérdidas por fricción).
    // En esta simulación, se utiliza el parámetro "restitución = 1.0" en todos los cuerpos
    // para representar colisiones perfectamente elásticas.
    // Este modelo permite que la pelota rebote indefinidamente dentro de los límites,
    // manteniendo su velocidad tras cada colisión.
    // ------------------------------------------------------------
   
    // Piso
    pisoBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 5);
    pisoBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
    pisoBody->GetFixtureList()->SetRestitution(1.0f);

    // Techo
    techoBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 5);
    techoBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);
    techoBody->GetFixtureList()->SetRestitution(1.0f);

    // Pared izquierda
    paredIzqBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
    paredIzqBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);
    paredIzqBody->GetFixtureList()->SetRestitution(1.0f);

    // Pared derecha
    paredDerBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 100);
    paredDerBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);
    paredDerBody->GetFixtureList()->SetRestitution(1.0f);

    // Creo pelota
    pelotaBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.0, 1.0f);
    pelotaBody->SetTransform(b2Vec2(50.0f, 20.0f), 0.0f);

}

// Destructor de la clase

Game::~Game(void)
{ }