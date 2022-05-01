#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "View.h"

void menu(sf::RenderWindow& window);

int main() {

    enum class ViewType{ Map, Shop, Chat };
    auto CurrentView = ViewType::Map;

    View.reset(sf::FloatRect(0, 0, 1920, 1080));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "POTATO SIMULATOR");

    menu(window);


    try
    {
//============================================== Creating Elems ==============================================

        BaseElem MapBackground = BaseElem(0, 0, 2048, 2048, "weedcropped.jpg");
        BaseElem ShopBackground = BaseElem(0, 0, 2048, 2048, "ShopBG.png");
        Player Cat = Player(860, 440, 50, 92, 4, 8, "player.png");
        BaseElem Pause = BaseElem(1770, 50, 100, 100, "PauseIcon.png");
        BaseElem Close = BaseElem(1770, 50, 100, 100, "CloseIcon.png");
        BaseElem Shop = BaseElem(50, 200, 100, 100, "ShopIcon.png");
        BaseElem Chat = BaseElem(50, 50, 100, 100, "ChatIcon.png");

//============================================================================================================

        sf::Clock Clock;
        float CurrentFrame = 0;

        while (window.isOpen())
        {
            float time = Clock.getElapsedTime().asMicroseconds() / 800;
            Clock.restart();

            sf::Vector2i MousePos = sf::Mouse::getPosition(window);

            sf::Event event{};
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::KeyPressed)                                                // Keyboard
                {
                    if (event.key.code == sf::Keyboard::Tab)
                    {
                        if (CurrentView == ViewType::Map) {
                            CurrentView = ViewType::Shop;
                        }
                        else if (CurrentView == ViewType::Shop) {
                            CurrentView = ViewType::Map;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        CurrentView = ViewType::Map;
                    }
                }

                else if (event.type == sf::Event::MouseButtonPressed)                               // Mouse Buttons
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (CurrentView == ViewType::Map)
                        {
                            if (Shop.mSprite.getLocalBounds().contains(MousePos.x, MousePos.y)) {
                                CurrentView = ViewType::Shop;
                            }
                            else if (Pause.mSprite.getLocalBounds().contains(MousePos.x, MousePos.y)) {
                                return 0;
                            }
                            else if (Chat.mSprite.getLocalBounds().contains(MousePos.x, MousePos.y)) {
                                CurrentView = ViewType::Chat;
                            }
                        }

                        else if (CurrentView == ViewType::Shop)
                        {
                            if (Close.mSprite.getLocalBounds().contains(MousePos.x, MousePos.y)) {
                                CurrentView = ViewType::Map;
                            }
                        }

                        else // if (CurrentView == ViewType::Chat)
                        {
                            if (Close.mSprite.getLocalBounds().contains(MousePos.x, MousePos.y)) {
                                CurrentView = ViewType::Map;
                            }
                        }
                    }
                }
            }


            if (CurrentView == ViewType::Map)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    Cat.GoLeft(CurrentFrame, time);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    Cat.GoRight(CurrentFrame, time);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    Cat.GoUp(CurrentFrame, time);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    Cat.GoDown(CurrentFrame, time);
            }


//============================================== Draw ========================================================


            window.clear(sf::Color(95, 118, 45));

            ViewMap(time);
            Cat.update(time);
            ChangeView();

            window.setView(View);

            if (CurrentView == ViewType::Map) {
                for(int i = 0; i < 20; ++i) {
                    for(int j = 0; j < 20; ++j) {
                        /*if(TileMap[i][j] == '0')*/ {
                            MapBackground.mSprite.setTextureRect(sf::IntRect(0, 0, 346, 333));
                        }

                        MapBackground.mSprite.setPosition(j * 346, i * 333);
                        window.draw(MapBackground.mSprite);
                    }
                }
//                window.draw(MapBackground.mSprite);
                window.draw(Cat.mSprite);
                Shop.mSprite.setPosition(View.getCenter().x - 910, View.getCenter().y - 490);
                window.draw(Shop.mSprite);
                Chat.mSprite.setPosition(View.getCenter().x - 910, View.getCenter().y - 340);
                window.draw(Chat.mSprite);
                Pause.mSprite.setPosition(View.getCenter().x + 810, View.getCenter().y - 490);
                window.draw(Pause.mSprite);

            }
            else if (CurrentView == ViewType::Shop) {
                ShopBackground.mSprite.setPosition(View.getCenter().x - 960, View.getCenter().y - 540);
                window.draw(ShopBackground.mSprite);
                Close.mSprite.setPosition(View.getCenter().x + 810, View.getCenter().y - 490);
                window.draw(Close.mSprite);
            }
            else { // if (CurrentView == ViewType::Chat)
                Close.mSprite.setPosition(View.getCenter().x + 810, View.getCenter().y - 490);
                window.draw(Close.mSprite);
            }


//============================================= Display ==============================================================

            window.display();
        }
    }
    catch(const std::exception &e)
    {
        std::cout << e.what();
        return 0;
    }
}