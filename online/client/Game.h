#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "Network.h"
#include "BaseElem.h"
#include "BaseStruct.h"
#pragma once


class Game
{
public:
	Game();
	void run();
	~Game();
private:
	void update();
	void procesEvents();
	void render();
	static const sf::Time timePerFrame;

	sf::RenderWindow mWindow;

	bool dir[4];

	sf::TcpSocket socket;

	unsigned short port;
	sf::IpAddress ipAdress;

	sf::Clock m_clock;

	std::vector<sf::Sprite> remotePlayers;

	Player pSprite;
	std::vector<std::unique_ptr<CoFarmer>> cofarmers;
	std::unique_ptr<Network> network;

	sf::Vector2f lastDirSent;
	bool movingDiagonal = false;

	sf::Time m_positionTimer;

    sf::Texture grass_texture;
    sf::Sprite grass_sprite;

    Map Map;
    Menu Menu;
    Shop Shop;
    Chat Chat;

    std::string view = "Menu";
};
