#ifndef FERMA_TEXTELEM_H
#define FERMA_TEXTELEM_H

#include <iostream>
#include <SFML/Graphics.hpp>

struct TextElem {
    sf::Font mFont;
    sf::Text mText;
    std::string FileName;

    TextElem();
    explicit TextElem(std::string&& filename, int CharSize, sf::Color Color, std::string&& TextLine);
    ~TextElem();
};


struct Money : TextElem {
    int balance = 100; // храним в бд

    Money();
    void Draw(sf::RenderWindow& window);
    ~Money();
};

#endif //FERMA_TEXTELEM_H
