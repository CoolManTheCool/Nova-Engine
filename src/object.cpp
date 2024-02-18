#include "object.h"

object_T::object_T(sf::Vector2f position, float radius, sf::Color color) {
    this->position = position;
    this->radius = radius;
    this->color = color;
    setPosition(position);
}

void object_T::update() {
    setPosition(position);
}