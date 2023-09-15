#include "BaseElem.h"
#include "BaseStruct.h"


custom_exceptions::unable_to_open_a_file::unable_to_open_a_file(const std::string& name)
    : std::runtime_error("Unable to open file '" + name + "'\n") {}



// ============================================ BaseElem ==========================================================

BaseElem::BaseElem() = default;


BaseElem::BaseElem(float x_, float y_, float w_, float h_, std::string &&name) :
    x(x_), y(y_), w(w_), h(h_), FileName(std::move(name))
{
    std::ifstream f("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/images/" + FileName);
    if (!f.is_open()) {
        throw custom_exceptions::unable_to_open_a_file(FileName);
    }

    mImage.loadFromFile("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/images/" + FileName);
    mTexture.loadFromImage(mImage);
    mSprite.setTexture(mTexture);
    mSprite.setPosition(x, y);
}





// ============================================= MapElem ==========================================================

MapElem::MapElem() = default;


MapElem::MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_)
    : BaseElem(x_, y_, w_, h_, std::move(name)),
      type_id(std::move(type_id_)), id(id_) {}


MapElem::MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          type_id(type_id_), id(id_) {}



// ============================================ GardenBed ==========================================================

GardenBed::GardenBed() : product(nullptr) {}


GardenBed::GardenBed(float x_, float y_, float w_, float h_, std::string&& name, int id_)
        : BaseElem(x_, y_, w_, h_, std::move(name)),
          id(id_), product(nullptr) {}


void GardenBed::CheckProductCoords()
{
    if (product == nullptr) {
        return;
    }
    product->x = x;
    product->y = y + h - product->h;
    product->mSprite.setPosition(product->x, product->y);
}




// ============================================= GardenBedElem =====================================================

GardenBedElem::GardenBedElem() = default;


GardenBedElem::GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_)
    : BaseElem(x_, y_, w_, h_, std::move(name)),
      type_id(std::move(type_id_)), id(id_) {}


GardenBedElem::GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_)
    : BaseElem(x_, y_, w_, h_, std::move(name)),
      type_id(type_id_), id(id_) {}



// ============================================= Player ===========================================================

//Player::Player() = default;


//Player::Player(float x_, float y_, float w_, float h_, float recTop, float recLeft,
//               std::string&& file_name, std::string& player_name) :
//    BaseElem(x_, y_, w_, h_, std::move(file_name)), name(player_name)
//{
//    mSprite.setTextureRect(sf::IntRect(recLeft, recTop, w, h));
//}


//void Player::InteractionWithMap(Map* MapPtr)
//{
//    for (auto& it : MapPtr->BoughtItems) {
//        for (auto& elem : it.second) {
//            if (elem->mSprite.getGlobalBounds().intersects(sf::Rect(sf::Rect(x, y, w, h)))) {
//                switch(dir) {
//                    case Direction::Right: {
//                        x = elem->x - w;
//                    } break;
//                    case Direction::Left: {
//                        x = elem->x + elem->w;
//                    } break;
//                    case Direction::Up: {
//                        y = elem->y + elem->h;
//                    } break;
//                    case Direction::Down: {
//                        y = elem->y - h;
//                    } break;
//                }
//            }
//        }
//    }
//    if (x < 0) x = 0;
//    else if (x > 1920 - w) x = 1920 - w;
//
//    if (y < 0) y = 0;
//    else if (y > 1080 - h) y = 1080 - h;
//}


//void Player::update(float time, Map* MapPtr)
//{
//    switch(dir) {
//        case Direction::Right: {
//            dx = speed, dy = 0;
//        } break;
//        case Direction::Left: {
//            dx = -speed, dy = 0;
//        } break;
//        case Direction::Up: {
//            dx = 0, dy = -speed;
//        } break;
//        case Direction::Down: {
//            dx = 0, dy = speed;
//        } break;
//    }
//    x += dx * time;
//    y += dy * time;
//
//    InteractionWithMap(MapPtr);
//
//    speed = 0;
//    mSprite.setPosition(x, y);
//}


//void Player::GoLeft(float &CurrentFrame, float &time)
//{
//    dir = Direction::Left;
//    speed = 0.25;
//    CurrentFrame += 0.005 * time;
//    if (CurrentFrame > 4) CurrentFrame -= 4;
//    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 96, 64, 96));
//}


//void Player::GoRight(float &CurrentFrame, float &time)
//{
//    dir = Direction::Right;
//    speed = 0.25;
//    CurrentFrame += 0.005 * time;
//    if (CurrentFrame > 4) CurrentFrame -= 4;
//    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 192, 64, 96));
//}


//void Player::GoUp(float &CurrentFrame, float &time)
//{
//    dir = Direction::Up;
//    speed = 0.25;
//    CurrentFrame += 0.005 * time;
//    if (CurrentFrame > 4) CurrentFrame -= 4;
//    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 288, 64, 96));
//}


//void Player::GoDown(float &CurrentFrame, float &time)
//{
//    dir = Direction::Down;
//    speed = 0.25;
//    CurrentFrame += 0.005 * time;
//    if (CurrentFrame > 4) CurrentFrame -= 4;
//    mSprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 0, 64, 96));
//}


//void Player::move(float &CurrentFrame, float &time)
//{
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//        GoLeft(CurrentFrame, time);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//        GoRight(CurrentFrame, time);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//        GoUp(CurrentFrame, time);
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//        GoDown(CurrentFrame, time);
//}
