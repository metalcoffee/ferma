#ifndef FERMA_BASEELEM_H
#define FERMA_BASEELEM_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <fstream>
#include <string>

#include <soci/session.h>
#include <soci/transaction.h>
#include <soci/backend-loader.h>
#include <soci/connection-parameters.h>

enum class Direction {Left, Right, Up, Down};


namespace custom_exceptions {
    struct unable_to_open_a_file : std::runtime_error {
        explicit unable_to_open_a_file(const std::string& name);
    };
}


struct BaseElem {
    float x = 0, y = 0, w = 0, h = 0;
    std::string FileName;
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Image mImage;

    BaseElem();
    explicit BaseElem(float x_, float y_, float w_, float h_, std::string&& name);
};


struct MapElem : BaseElem { // объекты, которые не грядка и не растение на грядке
    std::string type_id;
    int id;

    MapElem();
    explicit MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_);
    explicit MapElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_);
};


struct GardenBedElem : BaseElem { // растения, живущие на грядке
    std::string type_id;
    int id;

    GardenBedElem();
    explicit GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string&& type_id_, int id_);
    explicit GardenBedElem(float x_, float y_, float w_, float h_, std::string&& name, std::string& type_id_, int id_);
};


struct Map;

struct GardenBed : BaseElem { // грядка
    int id;
    GardenBedElem* product;
    int cur_fragment;

    GardenBed();
    explicit GardenBed(float x_, float y_, float w_, float h_, std::string&& name, int id_);
    void ModifyProduct(soci::session& sql);
    std::string WishToHarvestCrop(int player_x, int player_y, Map* MapPtr);
    void Draw(sf::RenderWindow &window, soci::session& sql);
    int dist(int other_x, int other_y);
};


struct Player : BaseElem {
    float dx = 0, dy = 0, speed = 0;
    Direction dir = Direction::Right;
    std::string name;

    Player();
    explicit Player(float x_, float y_, float w_, float h_, float recTop, float recLeft, std::string&& file_name, std::string& player_name);
    void InteractionWithMap(Map* MapPtr);
    void update(float time, Map* MapPtr);
    void GoLeft(float &CurrentFrame, float &time);
    void GoRight(float &CurrentFrame, float &time);
    void GoUp(float &CurrentFrame, float &time);
    void GoDown(float &CurrentFrame, float &time);
    void move(float &CurrentFrame, float &time);
};

#endif //FERMA_BASEELEM_H
