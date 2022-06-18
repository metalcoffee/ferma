#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player : public sf::Sprite
{
public:
	Player();
	~Player();

	void setID(int id){ m_id = id; m_isReady = true; }
	int getID(){ return m_id; }

	bool isReady(){ return m_isReady; }

	void setName(std::string name) { m_name = name; }
	std::string getName(){ return m_name; }

	void setDirection(sf::Vector2f direction) { m_direction = direction; }
	sf::Vector2f getDirection(){ return m_direction; }
private:
	// Player sprite and texture
	sf::Texture pTexture;
	int m_id;
	bool m_isReady;
	std::string m_name;

	sf::Vector2f m_direction;
};

