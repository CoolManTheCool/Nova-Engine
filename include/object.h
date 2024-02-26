#ifndef OBJECT_H
#define OBJECT_H

#include "SFML/Graphics.hpp"

class object_T : public sf::CircleShape {
public:
    object_T(sf::Vector2f position, float radius, sf::Color color);
    void update();

    //O P E R A T O R   O V E R L O A D
    friend std::ostream& operator<<(std::ostream& os, const object_T& obj);

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color    color;
    float        radius;
};

#endif
