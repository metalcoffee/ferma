#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unordered_map>
#include <vector>
#include <string>

struct BaseStruct {
//    soci::session sql;

    BaseStruct();
    virtual std::string CheckBoundaries(sf::Vector2i& MousePos) = 0;
};

struct TextElem {
    sf::Font mFont;
    sf::Text mText;
    std::string FileName;

    TextElem();
    explicit TextElem(std::string&& filename, int CharSize, sf::Color Color, std::string&& TextLine);
    ~TextElem();
};

struct BaseElem {
    float x = 0, y = 0, w = 0, h = 0;
    std::string FileName;
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Image mImage;

    BaseElem();
    explicit BaseElem(float x_, float y_, float w_, float h_, std::string&& name);
};

struct Menu : virtual BaseStruct {
    TextElem NewGame;
    TextElem Continue;
    TextElem Settings;
    TextElem Exit;
    BaseElem Carrot;
//    Map *MapPtr;
//    Player* PlayerPtr;

    explicit Menu(/* Map *m, Player *pm */);
    void Draw(sf::RenderWindow& window) const;
    void ChangeColor(sf::RenderWindow& window);
    std::string CheckBoundaries(sf::Vector2i& MousePos) override;
};
