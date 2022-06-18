#ifndef DRAFT1_VIEW_H
#define DRAFT1_VIEW_H

#include <SFML/Graphics.hpp>

sf::View View;

void GetPlayerCoordinatesForView(float x, float y) {
    if (x < 300) {
        x = 300;
    }
    if (y < 300) {
        y = 300;
    }
    if (x > 1500) {
        x = 1500;
    }
    if (y > 700) {
        y = 700;
    }
    View.setCenter(x, y);
}

void ChangeView() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        View.zoom(1.01f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        View.zoom(0.99f);
    }
}

sf::View ViewMap(float time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        View.move(0.5f * time, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        View.move(0, 0.5f * time);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        View.move(0, -0.5f * time);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        View.move(-0.5f * time, 0);
    }

    return View;
}

#endif //DRAFT1_VIEW_H
