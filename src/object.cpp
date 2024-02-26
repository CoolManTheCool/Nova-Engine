#include "object.h"

object_T::object_T(sf::Vector2f position, float radius, sf::Color color) {
    this->position = position;
    this->radius = radius;
    this->color = color;
    setPosition(position);
    setRadius(radius);
    setFillColor(color);
}

void object_T::update() {
    setPosition(position);
}

std::ostream& operator<<(std::ostream& os, const object_T& obj) {
    os << "Position: (" << obj.position.x << ", " << obj.position.y << "), ";
    os << "Velocity: (" << obj.velocity.x << ", " << obj.velocity.y << "), ";
    os << "Color: (" << static_cast<int>(obj.color.r) << ", " << static_cast<int>(obj.color.g) << ", " << static_cast<int>(obj.color.b) << "), ";
    os << "Radius: " << obj.radius;
    return os;
}