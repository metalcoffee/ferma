#include "BaseStruct.h"


Shop::Shop() : MapPtr(nullptr) {}

Shop::Shop(Map* map_ptr) :
        BG(BaseElem(0, 0, 2048, 2048, "shopBG.png")),
        Close(BaseElem(1770, 50, 100, 100, "close.png")),
        Frame(BaseElem(220, 220, 360, 360, "frame.png")),
        MapPtr(map_ptr)
    {
        Items["garden_bed"] = new BaseElem(0, 0, 360, 360, "garden_bed.png");
        Items["donut"] = new BaseElem(0, 0, 360, 360, "donut.png");
        Items["diploma"] = new BaseElem(0, 0, 360, 360, "diploma.png");
        Items["tomato"] = new BaseElem(0, 0, 360, 360, "tomato.png");
        Items["cucumber"] = new BaseElem(0, 0, 360, 360, "cucumber.png");

        int coord = 0;
        for (auto it = Items.begin(); it != Items.end(); it++, coord++) {
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

void Shop::Draw(sf::RenderWindow &window) {
    window.draw(BG.mSprite);
    window.draw(Close.mSprite);
    for (const auto& it : Items) {
        window.draw(Frame.mSprite);
        window.draw(it.second->mSprite);
    }
}

std::string Shop::CheckBoundaries(sf::Vector2i& MousePos)
{
    if (Close.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "Map";
    }
    for (auto& it : Items) {
        if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y))
        {
            std::string ItemName = it.second->FileName.substr(0, it.second->FileName.find('.'));

            double coord_x = MousePos.x - 25;
            double coord_y = MousePos.y - 25;

            std::size_t id = MapPtr->CreateStructForNewItem(ItemName, coord_x, coord_y);

            soci::transaction tr(sql);
            sql << "insert into objects_on_map values ((:type_id), (:id), (:coord_x), (:coord_y))",
                    soci::use(ItemName), soci::use(id), soci::use(coord_x), soci::use(coord_y);
            int money;
            std::string name = "abober";
            sql << "select money from players where name=(:name)", soci::use(name), soci::into(money);
            money -= 10;
            sql << "update players set money = :money", soci::use(money);
            tr.commit();

            return "Map";
        }
    }
    return "Shop";
}


void Shop::ChangeColor(sf::RenderWindow& window, sf::Vector2i& MousePos) {
    Frame.mSprite.setColor(sf::Color::Transparent);
    Frame.mSprite.setPosition(180, 180);
    for (const auto& it : Items) {
        if (it.second->mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
            Frame.mSprite.setPosition(it.second->x, it.second->y);
            Frame.mSprite.setColor(sf::Color::Black);
        }
    }
}
