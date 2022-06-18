#include "BaseStruct.h"
#include <ctime>

Shop::Shop() : MapPtr(nullptr), GardenBedPtr(nullptr) {}

Shop::Shop(Map* map_ptr) :
        BG(BaseElem(0, 0, 2048, 2048, "shopBG.png")),
        Close(BaseElem(1770, 50, 100, 100, "close.png")),
        Frame(BaseElem(220, 220, 360, 360, "frame.png")),
        MapPtr(map_ptr),
        CurrentMode(Mode::BuyItem),
        GardenBedPtr(nullptr)
{

//------------------------------------------ Items ---------------------------------------------------

    Items["garden_bed"] = new BaseElem(0, 0, 360, 360, "garden_bed.png");
    Items["rock_meme"] = new BaseElem(0, 0, 300, 300, "rock_meme.png");
    Items["chuchello"] = new BaseElem(0, 0, 360, 360, "chuchello.png");

    int coord = 0;
    for (auto it = Items.begin(); it != Items.end(); it++, coord++) {
        it->second->x = 180 + 400 * (coord % 4);
        it->second->y = 180 + 400 * (coord / 4);
        it->second->mSprite.setPosition(it->second->x, it->second->y);
        if(it->first == "rock_meme") {
            it->second->mSprite.setPosition(it->second->x + 30, it->second->y + 25);
        }
        if(it->first == "chuchello") {
            it->second->mSprite.setPosition(it->second->x + 45, it->second->y);
        }
    }


//----------------------------------------- GardenBedPlants -------------------------------------------

    GardenBedPlants["tomato"] = new BaseElem(0, 0, 360, 360, "tomato.png");
    GardenBedPlants["cucumber"] = new BaseElem(0, 0, 360, 360, "cucumber.png");

    coord = 0;
    for (auto it = GardenBedPlants.begin(); it != GardenBedPlants.end(); it++, coord++) {
        it->second->x = 180 + 400 * (coord % 4);
        it->second->y = 180 + 400 * (coord / 4);
        it->second->mSprite.setPosition(it->second->x, it->second->y);
    }
}



Shop::~Shop() {
    for (auto& item : Items) {
        delete item.second;
    }
}

void Shop::Draw(sf::RenderWindow &window)
{
    window.draw(BG.mSprite);
    window.draw(Close.mSprite);
    MapPtr->Money.Draw(window);

    switch(CurrentMode) {
        case Mode::BuyItem:
            for (const auto& it : Items) {
                window.draw(Frame.mSprite);
                window.draw(it.second->mSprite);
            }
            break;
        case Mode::BuyGardenBedPlant:
            for (const auto& it : GardenBedPlants) {
                window.draw(Frame.mSprite);
                window.draw(it.second->mSprite);
            }
            break;
    }
}

std::string Shop::CheckBoundaries(sf::Vector2i& MousePos)
{
    if (Close.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        CurrentMode = Mode::BuyItem;
        return "Map";
    }

    switch(CurrentMode) {
        case Mode::BuyItem:
            for (auto& it : Items) {
                if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y))
                {
                    std::string type_id = it.first;

                    bool OK = MapPtr->CheckBalance(type_id);

                    if (!OK) {
                        continue;
                    }

                    double coord_x = MousePos.x - 25;
                    double coord_y = MousePos.y - 25;

                    std::size_t id = MapPtr->CreateStructForNewItem(type_id, coord_x, coord_y);

                    soci::transaction tr(sql);
                    sql << "insert into objects_on_map values ((:type_id), (:id), (:coord_x), (:coord_y))",
                            soci::use(type_id), soci::use(id), soci::use(coord_x), soci::use(coord_y);

                    tr.commit();

                    MapPtr->ChangeBalance(type_id);

                    return "Map";
                }
            }
            break;

        case Mode::BuyGardenBedPlant:
            for (auto& it : GardenBedPlants) {
                if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y))
                {
                    std::string type_id = it.first;

                    bool OK = MapPtr->CheckBalance(type_id);

                    if (!OK) {
                        continue;
                    }

                    std::size_t id = MapPtr->CreateStructForNewItem(type_id, 0, 0);

                    std::size_t gar_bed_id = GardenBedPtr->id;

                    GardenBedPtr->product = MapPtr->GardenBedPlants[type_id].back();
                    GardenBedPtr = nullptr;

                    CurrentMode = Mode::BuyItem;

                    MapPtr->ChangeBalance(type_id);


                    soci::transaction tr(sql);

                    std::string time_to_grow;
                    sql << "select time_to_grow from time_defaults where type_id=(:type_id)", soci::use(type_id), soci::into(time_to_grow);

                    std::string cur_time;
                    sql << "select current_timestamp", soci::into(cur_time);

                    sql << "insert into garden_bed_products values"
                           "((:type_id), (:id), (:gar_bed_id), "
                           "(:cur_time)::timestamp, (:cur_time)::timestamp + (:time_to_grow)::interval, (:time_to_grow)::interval / 5, 0)",
                            soci::use(type_id), soci::use(id), soci::use(gar_bed_id), soci::use(cur_time), soci::use(cur_time), soci::use(time_to_grow), soci::use(time_to_grow);

                    tr.commit();

                    return "Map";
                }
            }
    }

    return "Shop";
}


void Shop::ChangeColor(sf::RenderWindow& window, sf::Vector2i& MousePos) {
    Frame.mSprite.setColor(sf::Color::Transparent);
    Frame.mSprite.setPosition(180, 180);

    switch(CurrentMode) {
        case Mode::BuyItem:
            for (const auto& it : Items) {
                if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
                    Frame.mSprite.setPosition(it.second->x, it.second->y);
                    Frame.mSprite.setColor(sf::Color::Black);
                }
            }
            break;
        case Mode::BuyGardenBedPlant:
            for (const auto& it : GardenBedPlants) {
                if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
                    Frame.mSprite.setPosition(it.second->x, it.second->y);
                    Frame.mSprite.setColor(sf::Color::Black);
                }
            }
    }

}
