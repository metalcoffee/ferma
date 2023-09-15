#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "BaseElem.h"
#include "BaseStruct.h"

// abracadabra!
enum class ViewType{ Map, Shop, Chat, Menu, CloseWindow };

ViewType CheckCurrentView(BaseStruct& Struct, sf::Vector2i& MousePos) {
    std::string Type = Struct.CheckBoundaries(MousePos);
    if (Type == "Map") return ViewType::Map;
    if (Type == "Shop") return ViewType::Shop;
    if (Type == "Chat") return ViewType::Chat;
    if (Type == "Menu") return ViewType::Menu;
    if (Type == "CloseWindow") return ViewType::CloseWindow;

    std::cout << "Error while getting ViewType from string\n";
    return ViewType::Map;
}


int main()
{
    auto CurrentView = ViewType::Menu;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "POTATO SIMULATOR");

    try
    {
//============================================== Creating Elems ==============================================

        std::string playername = "abober";

        Player farmer = Player(0, 0, 64, 96, 4, 8, "player.png", playername);

        Map Map(&farmer);
        Menu Menu(&Map, &farmer);
        Shop Shop(&Map);
        Map.ShopPtr = &Shop;
        Chat Chat;

//============================================================================================================

        sf::Clock Clock;
        float CurrentFrame = 0;

            while (window.isOpen())
            {
                float time = Clock.getElapsedTime().asMicroseconds() / 800;
                Clock.restart();

                sf::Vector2i MousePos = sf::Mouse::getPosition(window);

                sf::Event event{};
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;
                        case sf::Event::KeyPressed:
                            if (event.key.code == sf::Keyboard::Tab)
                            {
                                if (CurrentView == ViewType::Map) {
                                    CurrentView = ViewType::Shop;
                                }
                                else if (CurrentView == ViewType::Shop) {
                                    CurrentView = ViewType::Map;
                                }
                            } else if (event.key.code == sf::Keyboard::Escape) {
                                if (CurrentView == ViewType::Chat || CurrentView == ViewType::Shop) {
                                    CurrentView = ViewType::Map;
                                }
                                else if (CurrentView == ViewType::Map) {
                                    CurrentView = ViewType::Menu;
                                }
                                else if (CurrentView == ViewType::Menu) {
                                    window.close();
                                }
                            }
                            break;
                        case sf::Event::MouseButtonPressed:
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (CurrentView == ViewType::Map) {
                                    CurrentView = CheckCurrentView(Map, MousePos);
                                }
                                else if (CurrentView == ViewType::Shop) {
                                    CurrentView = CheckCurrentView(Shop, MousePos);
                                }
                                else if (CurrentView == ViewType::Chat) {
                                    CurrentView = CheckCurrentView(Chat, MousePos);
                                }
                                else if (CurrentView == ViewType::Menu) {
                                    CurrentView = CheckCurrentView(Menu, MousePos);
                                }
                            }
                            break;
                        case sf::Event::MouseButtonReleased:
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (CurrentView == ViewType::Map) {
                                    Map.CheckOverlap(MousePos);
                                }
                            }
                            break;
                    }
                }


//============================================== Draw ========================================================


            window.clear(sf::Color(34, 177, 76));

                switch (CurrentView) {
                    case ViewType::Shop:
                        Shop.ChangeColor(window, MousePos);
                        Shop.Draw(window);
                        break;
                    case ViewType::Chat:
                        Chat.Draw(window);
                        break;
                    case ViewType::Menu:
                        Menu.ChangeColor(window);
                        Menu.Draw(window);
                        break;
                    case ViewType::CloseWindow:
                        window.close();
                        break;
                    case ViewType::Map:
                        farmer.move(CurrentFrame, time);
                        farmer.update(time, &Map);

                        soci::transaction tr(Map.sql);

                        double x = farmer.x, y = farmer.y;
                        playername = farmer.name;

                        Map.sql << "update players set x=(:x), y=(:y) where name=(:playername)",
                                soci::use(x), soci::use(y), soci::use(playername);

                        tr.commit();

                        Map.Draw(window, farmer);
                        break;
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
