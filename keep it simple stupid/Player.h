#ifndef DRAFT_PLAYER_H
#define DRAFT_PLAYER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

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



struct Player : BaseElem {
    float dx = 0, dy = 0, speed = 0;
    Direction dir = Direction::Right;

    Player();
    explicit Player(float x_, float y_, float w_, float h_, float recTop, float recLeft, std::string&& name);
    void update(float time);
    void GoLeft(float &CurrentFrame, float &time);
    void GoRight(float &CurrentFrame, float &time);
    void GoUp(float &CurrentFrame, float &time);
    void GoDown(float &CurrentFrame, float &time);
    void move(float &CurrentFrame, float &time);
};



struct BaseStruct {
    virtual std::string CheckBoundaries(sf::Vector2i& MousePos) = 0;
};


struct Menu : BaseStruct {
    BaseElem NewGame;
    BaseElem Exit;
    BaseElem Carrot;

    Menu();
    void Draw(sf::RenderWindow& window) const;
    void ChangeColor(sf::RenderWindow& window);
    std::string CheckBoundaries(sf::Vector2i& MousePos) override;
};


struct Map : BaseStruct {
    BaseElem BG;
    BaseElem Shop;
    BaseElem Chat;
    BaseElem Pause;
    std::unordered_map<std::string, std::vector<BaseElem*>> BoughtItems;
    BaseElem* IsMove;
    int dx;
    int dy;

    Map();
    ~Map();
    std::string CheckBoundaries(sf::Vector2i& MousePos) override;
    void Draw(sf::RenderWindow& window);
};


struct Shop : BaseStruct {
    BaseElem BG;
    BaseElem Close;
    BaseElem Frame;
    std::unordered_map<std::string, BaseElem*> Items;
    Map* MapPtr;

    Shop();
    explicit Shop(Map* map_ptr);
    ~Shop();
    void Draw(sf::RenderWindow& window);
    std::string CheckBoundaries(sf::Vector2i& MousePos) override;
    void ChangeColor(sf::RenderWindow& window, sf::Vector2i& MousePos);
};


struct Chat : BaseStruct {
    BaseElem Close;

    Chat();
    ~Chat();
    void Draw(sf::RenderWindow& window) const;
    std::string CheckBoundaries(sf::Vector2i& MousePos) override;
};


struct Money : BaseElem {
    sf::Font font;
    sf::Text text;
    int balance = 100; // храним в бд

    Money() {
        font.loadFromFile("CyrilicOld.ttf");
        text = sf::Text("", font, 50);
        text.setFillColor(sf::Color::Black);
        text.setOutlineColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
    }

    void Draw(sf::RenderWindow& window) {
        text.setString("Balance: " + std::to_string(balance));
        text.setPosition(1450, 50);
        window.draw(text);
    }
};

#endif //DRAFT_PLAYER_H
