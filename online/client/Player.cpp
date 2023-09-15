#include "Player.h"


Player::Player() : m_id(-1)
{
	if (!pTexture.loadFromFile("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/media/player.png"))
		std::cout << "Error loading player.png" << std::endl;
	this->setTexture(pTexture);
}


Player::~Player()
{
}
