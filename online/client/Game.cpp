#include "Game.h"

const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
{
	m_positionTimer = sf::Time::Zero;
	port = 45000;
	ipAdress = "localhost";

	network = std::make_unique<Network>(ipAdress, port);

    std::cout << "Enter your name:" << std::flush;
    std::string name = "abober"; // default (doesnt matter)
//    std::cin >> name;
	pSprite.setName(name);

	mWindow.create(sf::VideoMode(810, 450), "POTATO SIMULATOR");
	socket.setBlocking(false);
	mWindow.setFramerateLimit(60);

	dir[0] = 0;
	dir[1] = 0;
	dir[2] = 0;
	dir[3] = 0;

	srand(time(0));

    grass_texture.loadFromFile("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/media/grass.png");
    grass_sprite.setTexture(grass_texture);
}

Game::~Game()
{
	network->disconnect(&pSprite);
}
void Game::run()
{
	while (mWindow.isOpen())
	{
		procesEvents();
		update();
		render();
	}
}


void Game::procesEvents()
{
	sf::Event event;
    sf::Vector2i MousePos = sf::Mouse::getPosition(mWindow);

	while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
            break;
        }
        if (view == "Menu") {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        mWindow.close();
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i MousePos = sf::Mouse::getPosition(mWindow);
                        view = Menu.CheckBoundaries(MousePos);
                    }
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (view == "Map") {
                            Map.CheckOverlap(MousePos);
                        }
                    }
                    break;
            }
        } else if(view == "Map") {
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                view = "Menu";
            }
            else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (view == "Map") {
                    view = Map.CheckBoundaries(MousePos);
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    if (view == "Map") {
                            Map.CheckOverlap(MousePos);
                    }
            }
        } else if(view == "Chat") {
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                view = Chat.CheckBoundaries(MousePos);
            }
        }

            if(view == "CloseWindow") {
                mWindow.close();
                break;
            }
    }

    if(mWindow.hasFocus()) {
        if (view == "Map") {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !movingDiagonal) {
                pSprite.move(0, -2);
                pSprite.setDirection(sf::Vector2f(0, -2));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !movingDiagonal) {
                pSprite.move(-2, 0);
                pSprite.setDirection(sf::Vector2f(-2, 0));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !movingDiagonal) {
                pSprite.move(0, 2);
                pSprite.setDirection(sf::Vector2f(0, 2));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !movingDiagonal) {
                pSprite.move(2, 0);
                pSprite.setDirection(sf::Vector2f(2, 0));
            }


            //Up Right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                movingDiagonal = true;
                pSprite.move(1.4, -1.4);
                pSprite.setDirection(sf::Vector2f(1.4, -1.4));
            }
                //Up Left
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                movingDiagonal = true;
                pSprite.move(-1.4, -1.4);
                pSprite.setDirection(sf::Vector2f(-1.4, -1.4));
            }
                //Down Right
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                movingDiagonal = true;
                pSprite.move(1.4, 1.4);
                pSprite.setDirection(sf::Vector2f(1.4, 1.4));
            }
                //Down Left
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                movingDiagonal = true;
                pSprite.move(-1.4, 1.4);
                pSprite.setDirection(sf::Vector2f(-1.4, 1.4));
            } else
                movingDiagonal = false;

            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                pSprite.setDirection(sf::Vector2f(0, 0));
        }

            sf::Vector2i MousePos = sf::Mouse::getPosition(mWindow);
            switch (event.type) {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Tab) {
                        if (view == "Map") {
                            view = "Shop";
                        } else if (view == "Shop") {
                            view = "Map";
                        }
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        if (view == "Chat" || view == "Shop") {
                            view = "Map";
                        } else if (view == "Map") {
                            view = "Menu";
                        }
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                         if (view == "Shop") {
                            view = Shop.CheckBoundaries(MousePos);
                        } else if (view == "Chat") {
                            view = Chat.CheckBoundaries(MousePos);
                        } else if (view == "Menu") {
                            view = Menu.CheckBoundaries(MousePos);
                        }
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (view == "Map") {
                            Map.CheckOverlap(MousePos);
                        }
                    }
                    break;
            }
	}
}


void Game::update()
{
    if(view == "Map") {
        //Send direction
        if (lastDirSent != pSprite.getDirection()) {
            network->send(&pSprite); // send move data
            lastDirSent = pSprite.getDirection();
        }
        //Send position 1 time every 1 second
        m_positionTimer += m_clock.restart();
        if (m_positionTimer >= sf::seconds(1)) {
            network->sendPosition(&pSprite);
            m_positionTimer = sf::Time::Zero;
        }

        //Recive data from network
        network->receive(cofarmers, &pSprite);

        for (unsigned int i = 0; i < cofarmers.size(); i++) {
            cofarmers[i]->move(cofarmers[i]->getDirection());
        }
    }
}


void Game::render()
{
    if(view == "Menu") {
        mWindow.clear(sf::Color(34, 177, 76));
        Menu.ChangeColor(mWindow);
        Menu.Draw(mWindow);
    }
    if(view == "Map") {
        mWindow.clear();
        Map.Draw(mWindow);

        for (unsigned int i = 0; i < cofarmers.size(); i++)
            mWindow.draw(*cofarmers[i]);

        mWindow.draw(pSprite);
    }
    if(view == "Chat") {
        mWindow.clear(sf::Color(34, 177, 76));
        Chat.Draw(mWindow);
    }
    if(view == "Shop") {
        std::cout << "in shop\n";
        sf::Vector2i MousePos = sf::Mouse::getPosition(mWindow);
        Shop.ChangeColor(mWindow, MousePos);
        Shop.Draw(mWindow);
    }

    mWindow.display();
}
