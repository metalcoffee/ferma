#include "BaseStruct.h"

Chat::Chat() : Close(BaseElem(1770, 50, 100, 100, "close.png")) {}

Chat::~Chat() = default;

void Chat::Draw(sf::RenderWindow& window) const {
    window.draw(Close.mSprite);
};

std::string Chat::CheckBoundaries(sf::Vector2i& MousePos) {
    if (Close.mSprite.getGlobalBounds().contains(MousePos.x, MousePos.y)) {
        return "Map";
    }
    return "Chat";
}
