#include "BaseElem.h"
#include "BaseStruct.h"
#include <cmath>



custom_exceptions::unable_to_open_a_file::unable_to_open_a_file(const std::string& name)
        : std::runtime_error("Unable to open file '" + name + "'\n") {}



// ============================================ BaseElem ==========================================================

BaseElem::BaseElem() = default;


BaseElem::BaseElem(float x_, float y_, float w_, float h_, std::string &&name) :
        x(x_), y(y_), w(w_), h(h_), FileName(std::move(name))
{
    std::ifstream f("images/" + FileName);
    if (!f.is_open()) {
        throw custom_exceptions::unable_to_open_a_file(FileName);
    }

    mImage.loadFromFile("images/" + FileName);
    mTexture.loadFromImage(mImage);
    mSprite.setTexture(mTexture);
    mSprite.setPosition(x, y);
}





// ============================================= MapElem ==========================================================

MapElem::MapElem() = default;


MapElem::MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          type_id(std::move(type_id_)), id(id_) {
    mSprite.setScale(1, 1);
}


MapElem::MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          type_id(type_id_), id(id_) {
    mSprite.setScale(1, 1);
}



// ============================================ GardenBed ==========================================================

GardenBed::GardenBed() : product(nullptr) {}


GardenBed::GardenBed(float x_, float y_, float w_, float h_, std::string&& name, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          id(id_), product(nullptr), cur_fragment(0) {
    mSprite.setScale(1, 1);
}


void GardenBed::ModifyProduct(soci::session& sql)
{
    if (product == nullptr) {
        return;
    }

    std::string time_start, time_finish, segment;
    sql << "select time_start, time_finish, segment from garden_bed_products where gar_bed_id=(:id)",
            soci::use(id), soci::into(time_start), soci::into(time_finish), soci::into(segment);

    std::string cur_time;
    sql << "select current_timestamp", soci::into(cur_time);

    int statement;
    sql << "select (:cur_time)::timestamp >= (:time_finish)::timestamp", soci::use(cur_time), soci::use(time_finish), soci::into(statement);

    if (statement) {
        if (cur_fragment == 5) {
            cur_fragment = 6;
            soci::transaction tr(sql);
            sql << "update garden_bed_products set cur_fragment = 6 where gar_bed_id=(:id)", soci::use(id);
            tr.commit();
        }
        else if (cur_fragment == 4) {
            cur_fragment = 5;
            time_start = cur_time;
            std::string type_id = product->type_id;
            sql << "select time_to_rot from time_defaults where type_id=(:type_id)", soci::use(type_id), soci::into(time_finish);
            sql << "select (:time_start)::timestamp + (:time_finish)::interval", soci::use(time_start), soci::use(time_finish), soci::into(time_finish);
            soci::transaction tr(sql);
            sql << "update garden_bed_products set time_start=(:time_start)::timestamp, time_finish=(:time_finish)::timestamp, cur_fragment = 5 where gar_bed_id=(:id)",
                    soci::use(time_start), soci::use(time_finish), soci::use(id);
            tr.commit();
        }
    }

    sql << "select (:time_start)::timestamp + (:segment)::interval * (:cur_fragment + 1) < (:cur_time)::timestamp",
            soci::use(time_start), soci::use(segment), soci::use(cur_fragment), soci::use(cur_time), soci::into(statement);

    if (statement) {
        cur_fragment = std::min(cur_fragment + 1, 5);
        sql << "update garden_bed_products set cur_fragment=(:cur_fragment) where gar_bed_id=(:id)",
                soci::use(cur_fragment), soci::use(id);
    }

    product->mSprite.setTextureRect(sf::IntRect(cur_fragment * 50, 0, 50, 110));

    product->x = x;
    product->y = y - product->h;
    product->mSprite.setPosition(product->x, product->y);
}


int GardenBed::dist(int other_x, int other_y) {
    return std::sqrt((x - other_x) * (x - other_x) + (y - other_y) * (y - other_y));
}


std::string GardenBed::WishToHarvestCrop(int player_x, int player_y, Map* MapPtr)
{
    if (dist(player_x, player_y) > 200) {
        return "Map";
    }

    if (product == nullptr) {
        MapPtr->ShopPtr->CurrentMode = Mode::BuyGardenBedPlant;
        MapPtr->ShopPtr->GardenBedPtr = this;
        return "Shop";
    }

    if (cur_fragment < 5) {
        return "Map";
    }

    if (cur_fragment == 5) { // plant is ripen
        std::string type_id = product->type_id;
        int profit;
        MapPtr->ShopPtr->sql << "select cost_sell from cost_defaults where type_id=(:type_id)", soci::use(type_id), soci::into(profit);
        MapPtr->Money.balance += profit;

        int balance = MapPtr->Money.balance;
        std::string name = MapPtr->PlayerPtr->name;
        soci::transaction tr(MapPtr->sql);
        MapPtr->sql << "update players set money=(:balance) where name=(:name)", soci::use(balance), soci::use(name);
        tr.commit();
    }

    soci::transaction tr(MapPtr->sql);
    MapPtr->sql << "delete from garden_bed_products where gar_bed_id=(:id)", soci::use(id);
    tr.commit();

    product->mSprite.setColor(sf::Color::Transparent);
    product = nullptr;
    cur_fragment = 0;

    return "Map";
}

void GardenBed::Draw(sf::RenderWindow &window, soci::session& sql)
{
    window.draw(mSprite);

    if (product != nullptr)
    {
        product->mSprite.setPosition(x + 10,y);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
        product->mSprite.setPosition(x + 10,y + 100);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
        product->mSprite.setPosition(x + 10,y - 100);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
        product->mSprite.setPosition(x + 110,y);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
        product->mSprite.setPosition(x + 110,y + 100);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
        product->mSprite.setPosition(x + 110,y - 100);
        if (cur_fragment == 5) {
            product->mSprite.setColor(sf::Color::Yellow);
        }
        window.draw(product->mSprite);
//        product->mSprite.setColor(sf::Color::White);
//        if (cur_fragment == 5) {
//            product->mSprite.setColor(sf::Color::Yellow);
//        }
//
//        window.draw(product->mSprite);
    }
}


// ============================================= GardenBedElem =====================================================

GardenBedElem::GardenBedElem() = default;


GardenBedElem::GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          type_id(std::move(type_id_)), id(id_) {
    mSprite.setScale(1.5, 1.5);
}


GardenBedElem::GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          type_id(type_id_), id(id_) {
    mSprite.setScale(1.5, 1.5);
}



// ============================================= Player ===========================================================

Player::Player() = default;


Player::Player(float x_, float y_, float w_, float h_, float recTop, float recLeft, std::string&& file_name, std::string& player_name) :
        BaseElem(x_, y_, w_, h_, std::move(file_name)), name(player_name)
{
    mSprite.setTextureRect(sf::IntRect(recLeft, recTop, w, h));
    mSprite.setScale(1.5, 1.5);
}


void Player::InteractionWithMap(Map* MapPtr)
{
    for (auto& it : MapPtr->BoughtItems) {
        for (auto& elem : it.second) {
            if (elem->mSprite.getGlobalBounds().intersects(sf::Rect(sf::Rect(x, y, w, h)))) {
                switch(dir) {
                    case Direction::Right: {
                        x = elem->x - w;
                    } break;
                    case Direction::Left: {
                        x = elem->x + elem->w;
                    } break;
                    case Direction::Up: {
                        y = elem->y + elem->h;
                    } break;
                    case Direction::Down: {
                        y = elem->y - h;
                    } break;
                }
            }
        }
    }

    for (auto& elem : MapPtr->GardenBeds) {
        if (elem->mSprite.getGlobalBounds().intersects(sf::Rect(sf::Rect(x, y, w, h)))) {
            switch(dir) {
                case Direction::Right: {
                    x = elem->x - w;
                } break;
                case Direction::Left: {
                    x = elem->x + elem->w;
                } break;
                case Direction::Up: {
                    y = elem->y + elem->h;
                } break;
                case Direction::Down: {
                    y = elem->y - h;
                } break;
            }
        }
    }

    if (x < 0) x = 0;
    else if (x > 1920 - w) x = 1920 - w;

    if (y < 0) y = 0;
    else if (y > 1080 - h) y = 1080 - h;
}


void Player::update(float time, Map* MapPtr)
{
    switch(dir) {
        case Direction::Right: {
            dx = speed, dy = 0;
        } break;
        case Direction::Left: {
            dx = -speed, dy = 0;
        } break;
        case Direction::Up: {
            dx = 0, dy = -speed;
        } break;
        case Direction::Down: {
            dx = 0, dy = speed;
        } break;
    }
    x += dx * time;
    y += dy * time;

    InteractionWithMap(MapPtr);

    speed = 0;
    mSprite.setPosition(x, y);
}


void Player::GoLeft(float &CurrentFrame, float &time)
{
    dir = Direction::Left;
    speed = 0.25;
    CurrentFrame += 0.005 * time;
    if (CurrentFrame > 4) CurrentFrame -= 4;
    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 96, 64, 96));
}


void Player::GoRight(float &CurrentFrame, float &time)
{
    dir = Direction::Right;
    speed = 0.25;
    CurrentFrame += 0.005 * time;
    if (CurrentFrame > 4) CurrentFrame -= 4;
    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 192, 64, 96));
}


void Player::GoUp(float &CurrentFrame, float &time)
{
    dir = Direction::Up;
    speed = 0.25;
    CurrentFrame += 0.005 * time;
    if (CurrentFrame > 4) CurrentFrame -= 4;
    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 288, 64, 96));
}


void Player::GoDown(float &CurrentFrame, float &time)
{
    dir = Direction::Down;
    speed = 0.25;
    CurrentFrame += 0.005 * time;
    if (CurrentFrame > 4) CurrentFrame -= 4;
    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 0, 64, 96));
}


void Player::move(float &CurrentFrame, float &time)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        GoLeft(CurrentFrame, time);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        GoRight(CurrentFrame, time);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        GoUp(CurrentFrame, time);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        GoDown(CurrentFrame, time);
}
