#pragma once
#include <SFML/Graphics.hpp>
class Game
{
public:
	Game();
	void run();

private: 
	sf::Time timePerFrame = sf::seconds(1.f/60.f);
	sf::RenderWindow window;
	sf::View mainCamera;
	sf::Font font;
	sf::Text text;
	sf::View map;
	sf::RectangleShape hud;
	sf::RectangleShape player;
	void processEvents();
	void update(sf::Time deltaTime);
	void draw();
	void draw_map();
	bool moveR, moveUp, moveL, moveD, zoomIn, zoomOut, rotateL, rotateR, moving;
};

