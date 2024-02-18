#ifndef OBJECT_H
#define OBJECT_H

#include "SFML/Graphics.hpp"

class object_T : sf::CircleShape {
public:
    object_T(sf::Vector2f position, float radius, sf::Color color);
    void update();
private:
    sf::Vector2f position;
    sf::Color    color;
    float        radius;
};

#endif