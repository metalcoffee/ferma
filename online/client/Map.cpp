#include "BaseStruct.h"


BaseStruct::BaseStruct() : sql("postgresql", "dbname=ferma user=postgres password=Julius_Deshur_Theorem") {};

Map::Map() :
        IsMove(nullptr), dx(0), dy(0),
        BG(BaseElem(0, 0, 2048, 2048, "grassBG.png")), Shop(BaseElem(50, 50, 100, 100, "shop.png")),
        Chat(BaseElem(50, 200, 100, 100, "chat.png")), Pause(BaseElem(1770, 50, 100, 100, "pause.png")) {}



Map::~Map() {
    for (auto& it : BoughtItems) {
        for (auto& elem : it.second) {
            delete elem;
        }
    }
}


std::string Map::CheckBoundaries(sf::Vector2i& MousePos)
{
    if (Shop.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "Shop";
    }
    if (Pause.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "Menu";
    }
    if (Chat.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "Chat";
    }


    for (auto& GardenBed : GardenBeds) {
        if (GardenBed->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
            IsMove = GardenBed;
            dx = MousePos.x - GardenBed->mSprite.getPosition().x;
            dy = MousePos.y - GardenBed->mSprite.getPosition().y;
            GardenBed->CheckProductCoords();
            return "Map";
        }
    }


    for (auto& it : BoughtItems)
    {
        for (std::size_t i = 0; i < it.second.size(); ++i) {
            if (it.second[i]->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y))
            {
                IsMove = it.second[i];

                dx = MousePos.x - it.second[i]->mSprite.getPosition().x;
                dy = MousePos.y - it.second[i]->mSprite.getPosition().y;

                return "Map";
            }
        }
    }

    return "Map";
}


void Map::CheckOverlap(sf::Vector2i& MousePos)
{
    if (IsMove == nullptr) {
        return;
    }


    /*  TODO
     * нужно условие типа if Ismove = GardenBed && clock == мало => произошло нажатие на грядку, игрок хочет посадить растение
     *                                             clock == немало => игрок передвигает грядку, проверяем, чтобы не было наездов между объектами
     */


    bool Repeat = true;
    while (Repeat)
    {
        Repeat = false;
        bool NeedBreak = false;
        for (auto& it : BoughtItems) {
            if (NeedBreak) {
                break;
            }
            for (auto& elem : it.second)
            {
                if (elem == IsMove) {
                    continue;
                }
//                if (elem->mSprite.getGlobalBounds().intersects(sf::Rect(IsMove->x, IsMove->y, IsMove->w, IsMove->h))) {
//                    IsMove->x = elem->x + elem->w;
//                    IsMove->y = elem->y;
//                    Repeat = true;
                    NeedBreak = true;
//                    break;
//                }
            }
        }

        for (auto& garden_bed : GardenBeds) {
            if (garden_bed == IsMove) {
                continue;
            }
//            if (garden_bed->mSprite.getGlobalBounds().intersects(sf::Rect(IsMove->x, IsMove->y, IsMove->w, IsMove->h))) {
//                IsMove->x = garden_bed->x + garden_bed->w;
//                IsMove->y = garden_bed->y;
                Repeat = true;
//                break;
//            }
        }
    }

    IsMove->mSprite.setColor(sf::Color::White);
    IsMove->mSprite.setPosition(IsMove->x, IsMove->y);
    IsMove = nullptr;
}


void Map::Draw(sf::RenderWindow &window)
{
    for(int i = 0; i < 20; ++i) {
        for(int j = 0; j < 20; ++j) {
            BG.mSprite.setTextureRect(sf::IntRect(0, 0, 346, 333));
            BG.mSprite.setPosition(j * 346, i * 333);
            window.draw(BG.mSprite);
        }
    }

    if (IsMove != nullptr)
    {
        IsMove->mSprite.setColor(sf::Color::Green);

        double coord_x = sf::Mouse::getPosition(window).x - dx;
        double coord_y = sf::Mouse::getPosition(window).y - dy;

        IsMove->x = coord_x;
        IsMove->y = coord_y;

        IsMove->mSprite.setPosition(coord_x, coord_y);

        std::string type_id = IsMove_type_id;
        int id = IsMove_id;

        soci::transaction tr(sql);
        sql << "update objects_on_map set x=(:coord_x), y=(:coord_y) where type_id=(:type_id) and id=(:id)",
                soci::use(coord_x), soci::use(coord_y), soci::use(type_id), soci::use(id);
        tr.commit();
    }

    for (auto& GardenBed : GardenBeds) {
        window.draw(GardenBed->mSprite);
    }

    for (auto& it : GardenBedPlants) {
        for (auto& plant : it.second) {
            window.draw(plant->mSprite);
        }
    }

    for (auto& it : BoughtItems) {
        for (auto& elem : it.second) {
            elem->mSprite.setPosition(elem->x, elem->y);
            window.draw(elem->mSprite);
        }
    }

    window.draw(Shop.mSprite);
    window.draw(Chat.mSprite);
    window.draw(Pause.mSprite);
    Money.Draw(window);
}



void Map::Clear() {
    BoughtItems.clear();
    GardenBedPlants.clear();
    GardenBeds.clear();
}


int Map::CreateStructForNewItem(std::string& type_id, int coord_x, int coord_y)
{
    if (type_id == "garden_bed") {
        int id = GardenBeds.size();
        GardenBeds.push_back(new GardenBed(coord_x, coord_y, 50, 50, type_id + "50x50.png", id));
        IsMove = GardenBeds.back();
        dx = 25, dy = 25;
        return id;
    }

    if (std::count(AllGardenBedPlantsNames.begin(), AllGardenBedPlantsNames.end(), type_id) > 0) { // хотим купить растение для грядки
        int id = GardenBedPlants.size();
        GardenBedPlants[type_id].push_back(new GardenBedElem(coord_x, coord_y, 50, 50, type_id + "50x50.png", type_id, id));
        return id;
    }

    int id = BoughtItems.size();
    BoughtItems[type_id].push_back(new MapElem(coord_x, coord_y, 50, 50, type_id + "50x50.png", type_id, id));
    IsMove = BoughtItems[type_id].back();
    dx = 25, dy = 25;
    return id;
}
