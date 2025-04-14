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
    // Dibujar el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

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

    //Codigo para mover pelotas con el mouse
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i pixelPos = Mouse::getPosition(*wnd);
        Vector2f worldPos = wnd->mapPixelToCoords(pixelPos);

        // Verificamos si el mouse está sobre la pelota
        Vector2f pelotaPos(pelota1Body->GetPosition().x, pelota1Body->GetPosition().y);
        float distancia = sqrt(pow(worldPos.x - pelotaPos.x, 2) + pow(worldPos.y - pelotaPos.y, 2));

        if (distancia <= 5.f)
        {
            pelota1Body->SetTransform(b2Vec2(worldPos.x, worldPos.y), pelota1Body->GetAngle());
            pelota1Body->SetLinearVelocity(b2Vec2(0.f, 0.f));
        }

        Vector2f pelota2Pos(pelota2Body->GetPosition().x, pelota2Body->GetPosition().y);
        float distancia2 = sqrt(pow(worldPos.x - pelota2Pos.x, 2) + pow(worldPos.y - pelota2Pos.y, 2));

        if (distancia2 <= 5.f)
        {
            pelota2Body->SetTransform(b2Vec2(worldPos.x, worldPos.y), pelota2Body->GetAngle());
            pelota2Body->SetLinearVelocity(b2Vec2(0.f, 0.f));
        }
    }
}


// Comprobación de colisiones (a implementar más adelante)
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
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes estáticas del mundo físico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    //Creo las 2 pelotas
    pelota1Body = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5.f, 1.0f, 0.5f, 0.1f);
    pelota1Body->SetTransform(b2Vec2(30.0f, 30.0f), 0.0f);

    pelota2Body = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5.f, 1.0f, 0.5f, 0.1f);
    pelota2Body->SetTransform(b2Vec2(60.0f, 30.0f), 0.0f);

    
    //Creo el joint
    jointDef.Initialize(pelota1Body, pelota2Body, pelota1Body->GetWorldCenter(), pelota2Body->GetWorldCenter());
    jointDef.collideConnected = true;
    b2LinearStiffness(jointDef.stiffness, jointDef.damping, 3.0f, 0.7f, jointDef.bodyA, jointDef.bodyB);
    b2DistanceJoint* distJoint = (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef);

   
}

// Destructor de la clase

Game::~Game(void)
{ }