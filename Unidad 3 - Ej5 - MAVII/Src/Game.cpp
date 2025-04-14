#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo){
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
    // Dibujar limites
    sf::RectangleShape piso(sf::Vector2f(100, 5));
    piso.setFillColor(sf::Color::Blue);
    piso.setPosition(0, 95);
    wnd->draw(piso);

    sf::RectangleShape techo(sf::Vector2f(100, 5));
    techo.setFillColor(sf::Color::Blue);
    techo.setPosition(0, 0);
    wnd->draw(techo);

    sf::RectangleShape paredIzq(sf::Vector2f(5, 100)); 
    paredIzq.setFillColor(sf::Color::Blue);
    paredIzq.setPosition(0, 0);
    wnd->draw(paredIzq);

    sf::RectangleShape paredDer(sf::Vector2f(5, 100)); 
    paredDer.setFillColor(sf::Color::Blue);
    paredDer.setPosition(95, 0);
    wnd->draw(paredDer);

}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        if (evt.type == Event::Closed)
            wnd->close();
    }

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i pixelPos = Mouse::getPosition(*wnd);
        Vector2f worldPos = wnd->mapPixelToCoords(pixelPos);

        std::vector<std::pair<b2Body*, float>> cuerpos = {
            {cabezaBody, 1.5f},
            {torzoBody, 4.0f},
            {brazoIzqBody, 3.0f},
            {brazoDerBody, 3.0f},
            {piernaIzqBody, 5.0f},
            {piernaDerBody, 5.0f}
        };
        for (auto& par : cuerpos)
        {
            b2Body* body = par.first;
            float radio = par.second;

            Vector2f pos(body->GetPosition().x, body->GetPosition().y);
            float dist = std::hypot(worldPos.x - pos.x, worldPos.y - pos.y);

            if (dist <= radio)
            {
                body->SetTransform(b2Vec2(worldPos.x, worldPos.y), body->GetAngle());
                body->SetLinearVelocity(b2Vec2(0.f, 0.f));
                break;
            }
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

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    cabezaBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 3, 0.3, 0.2,0.0);
    cabezaBody->SetTransform(b2Vec2(50.0f, 44.5f), 0.0f);

    torzoBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5, 8, 0.3, 0.2, 0.0);
    torzoBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

    jointDef1.Initialize(cabezaBody, torzoBody, cabezaBody->GetWorldCenter(), torzoBody->GetWorldCenter() + b2Vec2(0.0f, -4.0f));
    jointDef1.collideConnected = true;
    b2LinearStiffness(jointDef1.stiffness, jointDef1.damping, 3.0f, 0.7f, jointDef1.bodyA, jointDef1.bodyB);
    (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef1);

    brazoIzqBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 6, 0.3, 0.2, 0.0);
    brazoIzqBody->SetTransform(b2Vec2(46.5f, 49.0f), 0.0f);

    jointDef2.Initialize(brazoIzqBody, torzoBody, brazoIzqBody->GetWorldCenter() + b2Vec2(0.0f, -3.0f),torzoBody->GetWorldCenter() + b2Vec2(-2.5f, -4.0f));
    jointDef2.collideConnected = true;
    b2LinearStiffness(jointDef2.stiffness, jointDef2.damping, 3.0f, 0.7f, jointDef2.bodyA, jointDef2.bodyB);
    (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef2);

    brazoDerBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 6, 0.3, 0.2, 0.0);
    brazoDerBody->SetTransform(b2Vec2(53.5f, 49.0f), 0.0f);

    jointDef3.Initialize(brazoDerBody, torzoBody, brazoDerBody->GetWorldCenter() + b2Vec2(0.0f, -3.0f), torzoBody->GetWorldCenter() + b2Vec2(2.5f, -4.0f));
    jointDef3.collideConnected = true;
    b2LinearStiffness(jointDef3.stiffness, jointDef3.damping, 3.0f, 0.7f, jointDef3.bodyA, jointDef3.bodyB);
    (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef3);

    piernaIzqBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 0.3, 0.2, 0.0);
    piernaIzqBody->SetTransform(b2Vec2(48.5f, 59.0f), 0.0f);

    jointDef4.Initialize(piernaIzqBody, torzoBody, piernaIzqBody->GetWorldCenter() + b2Vec2(0.0f, -5.0f), torzoBody->GetWorldCenter() + b2Vec2(- 1.0f, + 4.0f));
    jointDef4.collideConnected = true;
    b2LinearStiffness(jointDef4.stiffness, jointDef4.damping, 3.0f, 0.7f, jointDef4.bodyA, jointDef4.bodyB);
    (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef4);

    piernaDerBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 0.3, 0.2, 0.0);
    piernaDerBody->SetTransform(b2Vec2(51.5f, 59.0f), 0.0f);

    jointDef5.Initialize(piernaDerBody, torzoBody, piernaDerBody->GetWorldCenter() + b2Vec2(0.0f, -5.0f), torzoBody->GetWorldCenter() + b2Vec2( 1.0f, +4.0f));
    jointDef5.collideConnected = true;
    b2LinearStiffness(jointDef5.stiffness, jointDef5.damping, 3.0f, 0.7f, jointDef5.bodyA, jointDef5.bodyB);
    (b2DistanceJoint*)phyWorld->CreateJoint(&jointDef5);

}

// Destructor de la clase

Game::~Game(void)
{ }