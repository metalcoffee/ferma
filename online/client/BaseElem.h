#ifndef FERMA_BASEELEM_H
#define FERMA_BASEELEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
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


struct GardenBed : BaseElem { // грядка
    int id;
    GardenBedElem* product;

    GardenBed();
    explicit GardenBed(float x_, float y_, float w_, float h_, std::string&& name, int id_);
    void CheckProductCoords();
};




struct Map;

//struct Player : BaseElem {
//    float dx = 0, dy = 0, speed = 0;
//    Direction dir = Direction::Right;
//    std::string name;
//
//    int id = -1;
//    unsigned short port = 45000;
//    std::unique_ptr<sf::TcpSocket> socket;
//    sf::Time timeout;
//    bool is_ready = true;
//
//    Player();
//    explicit Player(float x_, float y_, float w_, float h_, float recTop, float recLeft,
//                    std::string&& file_name, std::string& player_name);
//    void InteractionWithMap(Map* MapPtr);
//    void update(float time, Map* MapPtr);
//    void GoLeft(float &CurrentFrame, float &time);
//    void GoRight(float &CurrentFrame, float &time);
//    void GoUp(float &CurrentFrame, float &time);
//    void GoDown(float &CurrentFrame, float &time);
//    void move(float &CurrentFrame, float &time);
//    int get_id() { return id; }
//    void set_id(int ID) { id = ID; is_ready = true; };
//};

#endif //FERMA_BASEELEM_H
