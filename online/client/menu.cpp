#include "BaseStruct.h"


Menu::Menu() :
    Continue("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/CyrilicOld.ttf", 65, sf::Color::White, "Play"),
    NewGame("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/CyrilicOld.ttf", 65, sf::Color::White, "Settings"),
    Settings("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/CyrilicOld.ttf", 65, sf::Color::White, ""
                                                     "Exit"),
    Exit("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/CyrilicOld.ttf", 65, sf::Color::White, ""),
    Carrot(BaseElem(600, -150, 918, 950, "carrot.png"))

{
    Continue.mText.setPosition(100, 100);
    NewGame.mText.setPosition(100, 220);
    Settings.mText.setPosition(100, 340);
    Exit.mText.setPosition(100, 460);

    Carrot.mSprite.setRotation(15);
    Carrot.mSprite.setScale(1.2f, 1.2f);
}


void Menu::Draw(sf::RenderWindow &window) const
{
    window.draw(Carrot.mSprite);
    window.draw(Continue.mText);
    window.draw(NewGame.mText);
    window.draw(Settings.mText);
    window.draw(Exit.mText);
}


void Menu::ChangeColor(sf::RenderWindow &window)
{
    Continue.mText.setFillColor(sf::Color::White);
    Continue.mText.setOutlineColor(sf::Color::White);

    NewGame.mText.setFillColor(sf::Color::White);
    NewGame.mText.setOutlineColor(sf::Color::White);

    Settings.mText.setFillColor(sf::Color::White);
    Settings.mText.setOutlineColor(sf::Color::White);

    Exit.mText.setFillColor(sf::Color::White);
    Exit.mText.setOutlineColor(sf::Color::White);

    if (Continue.mText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
    {
//        double coord_x = INT_MAX;
//        std::string playername = PlayerPtr->name;
//        sql << "select x from players where name=(:playername)", soci::use(playername), soci::into(coord_x);

//        if (coord_x != INT_MAX) {
            Continue.mText.setFillColor(sf::Color::Blue);
            Continue.mText.setOutlineColor(sf::Color::Blue);
//        }
    }
    else if (NewGame.mText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
        NewGame.mText.setFillColor(sf::Color::Blue);
        NewGame.mText.setOutlineColor(sf::Color::Blue);
    }
    else if (Settings.mText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
        Settings.mText.setFillColor(sf::Color::Blue);
        Settings.mText.setOutlineColor(sf::Color::Blue);
    }
    else if (Exit.mText.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
        Exit.mText.setFillColor(sf::Color::Blue);
        Exit.mText.setOutlineColor(sf::Color::Blue);
    }
}


std::string Menu::CheckBoundaries(sf::Vector2i& MousePos)
{
//    std::string playername = PlayerPtr->name;

    if (Continue.mText.getGlobalBounds().contains(MousePos.x, MousePos.y))
    {
//        double coord_x = INT_MAX;
//        double coord_y;

//        sql << "select x, y from players where name=(:playername)",
//                soci::use(playername), soci::into(coord_x), soci::into(coord_y);


//        if (coord_x != INT_MAX)    //  => есть сохранённая игра у игрока с именем playername
//        {
//            PlayerPtr->x = coord_x;
//            PlayerPtr->y = coord_y;

//            if (MapPtr->BoughtItems.empty())  // только начали сохранённую игру
//            {
//                std::string type_id;
//                int id;

//                soci::statement st = (sql.prepare << "select * from objects_on_map",
//                                      soci::into(type_id), soci::into(id), soci::into(coord_x), soci::into(coord_y));

//                st.execute();

//                while (st.fetch()) {
//                    MapPtr->CreateStructForNewItem(type_id, coord_x, coord_y); // TODO функция возвращает значение, но кого это волнует
//                }
//            }
            return "Map";
//        }

//        return "Menu";
    }

//    if (NewGame.mText.getGlobalBounds().contains(MousePos.x, MousePos.y))
//    {
//        MapPtr->Clear();

//        soci::transaction tr(sql);

//        sql << "delete from players where name=(:playername)", soci::use(playername);
//        sql << "delete from objects_on_map";

//        PlayerPtr->x = 950;
//        PlayerPtr->y = 500;
//        PlayerPtr->mSprite.setTextureRect(sf::IntRect(2 * 64, 0, 64, 96));

//        sql << "insert into players values ((:playername), 950, 500, 100)", soci::use(playername);

//        tr.commit();

//        return "Map";
//    }

    if (Settings.mText.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "CloseWindow";
    }

    return "Menu";
}

