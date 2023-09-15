#include "TextElem.h"


TextElem::TextElem() = default;

TextElem::~TextElem() = default;

TextElem::TextElem(std::string&& filename, int CharSize, sf::Color Color, std::string&& TextLine)
{
    FileName = std::move(filename);
    mFont.loadFromFile(FileName);
    mText = sf::Text(TextLine, mFont, CharSize);
    mText.setFillColor(Color);
    mText.setOutlineColor(Color);
}



Money::Money() : TextElem("C:/Users/mi/labs_cpp/labs-JuliaKud/ferma/for julius/client/CyrilicOld.ttf", 50, sf::Color::Black, "")
{
    mText.setStyle(sf::Text::Bold);
    mText.setPosition(1450, 50);
}

Money::~Money() = default;


void Money::Draw(sf::RenderWindow& window)
{
    mText.setString("Balance: " + std::to_string(balance));
    window.draw(mText);
}
