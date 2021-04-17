#include "Game.h"
#include <iostream>
#include <list>

Game::Game()
{
	window.create(sf::VideoMode(800, 800), "SFML");
	//window.setFramerateLimit(60);

	//создание камеры
	mainCamera.setCenter(400, 400); //центр камеры
	mainCamera.setSize(800, 800);   //её размер (растягивается на всё окно если не задан параметр view port)  

    //интерфейс, который будет отображаться поверх карты
    font.loadFromFile("20451.ttf"); //загрузка шрифта
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(24);
    text.setPosition(105, 110);
    hud.setSize(sf::Vector2f(150.f, 50.f));
    hud.setPosition(100, 100);
    hud.setFillColor(sf::Color(192, 192, 192));
    hud.setOutlineColor(sf::Color(222, 184, 135));
    hud.setOutlineThickness(3.f);
    //мини карта
    map.setCenter(750, 750);
    map.setSize(1500, 1500);

    //положение камер на экране
    mainCamera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    map.setViewport(sf::FloatRect(0.75f, 0.75f, 0.25f, 0.25f));

    player.setSize(sf::Vector2f(30.f, 30.f));
    player.setOrigin(50, 50);
    player.setFillColor(sf::Color::Red);
    player.setPosition(mainCamera.getCenter());

    //запрещаем взаимодействие с камерой пока пользователь не нажмет клавишу
    moveR = 0; moveL = 0; moveUp = 0; moveD = 0; zoomIn = 0; zoomOut = 0; rotateR = 0; rotateL = 0;
}

void Game::run() 
{
    sf::Clock clock; //запускаем таймер
    sf::Time timeSinceLastUpdate = sf::Time::Zero;//время с прошедшей итерации цикла 

    while (window.isOpen()) 
    {
        timeSinceLastUpdate += clock.restart(); 
        while (timeSinceLastUpdate > timePerFrame) // если времени больше то обновляем логику нужное колличество раз
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
            std::cout << timeSinceLastUpdate.asSeconds() <<" \n";
        }

        //если время 
        //sf::Time time = clock.restart();
        //processEvents();
        //update(time);
        draw();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        //управление камерой/персонажем
        if (event.type == sf::Event::KeyPressed) {
            moving = 1;
            if (event.key.code == sf::Keyboard::W) moveUp = 1;  //mainCamera.move(0, -10);
            if (event.key.code == sf::Keyboard::S) moveD = 1;   //mainCamera.move(0, 10);
            if (event.key.code == sf::Keyboard::A) moveL = 1;   //mainCamera.move(-10, 0);
            if (event.key.code == sf::Keyboard::D) moveR = 1;   //mainCamera.move(10, 0);
            if (event.key.code == sf::Keyboard::O) zoomIn = 1;  //mainCamera.zoom(0.9);
            if (event.key.code == sf::Keyboard::P) zoomOut = 1; //mainCamera.zoom(1.1);
            if (event.key.code == sf::Keyboard::R) rotateR = 1; //mainCamera.rotate(10);
            if (event.key.code == sf::Keyboard::T) rotateL = 1; //mainCamera.rotate(-10);
        }
        //есть небольшой баг из-за того что moving отвечает за всё движение а не конкретное
        if (event.type == sf::Event::KeyReleased) {
            moving = 0;
            if (event.key.code == sf::Keyboard::W) moveUp = 0;  //mainCamera.move(0, -10);
            if (event.key.code == sf::Keyboard::S) moveD = 0;   //mainCamera.move(0, 00);
            if (event.key.code == sf::Keyboard::A) moveL = 0;   //mainCamera.move(-00, 0);
            if (event.key.code == sf::Keyboard::D) moveR = 0;   //mainCamera.move(00, 0);
            if (event.key.code == sf::Keyboard::O) zoomIn = 0;  //mainCamera.zoom(0.9);
            if (event.key.code == sf::Keyboard::P) zoomOut = 0; //mainCamera.zoom(0.0);
            if (event.key.code == sf::Keyboard::R) rotateR = 0; //mainCamera.rotate(00);
            if (event.key.code == sf::Keyboard::T) rotateL = 0; //mainCamera.rotate(-10);
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    if (moving) {
        
        //при движении умножаем на прошедшее время
        if (moveUp) player.move(0, -50 * deltaTime.asSeconds());
        if (moveD)  player.move(0, 50 * deltaTime.asSeconds());
        if (moveR)  player.move(50 * deltaTime.asSeconds(), 0);
        if (moveL)  player.move(-50 * deltaTime.asSeconds(), 0);
        if (zoomIn) mainCamera.zoom(0.9);
        if (zoomOut) mainCamera.zoom(1.1);
        if (rotateR) mainCamera.rotate(10);
        if (rotateL) mainCamera.rotate(-10);
        mainCamera.setCenter(player.getPosition());
    }
}

void Game::draw()
{
    //устанавливаем камеру на окно
    window.setView(mainCamera);
    //рисуем градент для нашей камеры
    window.clear(sf::Color::Yellow);
    
    draw_map();
    
    //меняем камеру на стандартную для отрисовки hud
    window.setView(window.getDefaultView());
    window.draw(hud);
    std::string s;
    s = "(" + std::to_string(static_cast<int>(mainCamera.getCenter().x)) + " , "
        + std::to_string(static_cast<int>(mainCamera.getCenter().y)) + ")";
    text.setString(s);
    window.draw(text);

    //рисуем миникарту
    window.setView(map);
    draw_map();

   
    window.display();
}
void Game::draw_map()
{
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 25; j++)
        {
            //Рисуем квадраты
            sf::RectangleShape rect(sf::Vector2f(60.f, 60.f));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1.f);
            rect.setFillColor(sf::Color(100, 0 + j * 10, 0 + (i * 10)));
            rect.setPosition(i * 60, j * 60);
            window.draw(rect);
        }
    window.draw(player);
    
}
