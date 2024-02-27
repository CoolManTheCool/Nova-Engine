#include "object.h"
#include "settings.h"

object_T::object_T(sf::Vector2f position, float radius, sf::Color color) {
    this->position = position;
    this->radius = radius;
    this->color = color;
    setRadius(radius);
    setPosition(sf::Vector2f(position.x, -position.y));
    setFillColor(color);
    velocity.x += 500;
    velocity.y += -100;
}

void object_T::update(float Delta) {

    velocity += Settings::getInstance().gravity * Delta * 50.f; // gravity
    if(position.y < 0) {
        velocity.y *= -1;
        position.y = 0;
    }
    if(position.x < 0) {
        velocity.x *= -1;
        position.x = 0;
    }
    if(position.x > Settings::getInstance().resolution.x) {
        velocity.x *= -1;
        position.x = Settings::getInstance().resolution.x;
    }
    if(position.y > Settings::getInstance().resolution.y) {
        velocity.y *= -1;
        position.y = Settings::getInstance().resolution.y;
    }

    position += velocity * Delta;

    setPosition(sf::Vector2f(position.x, -position.y + Settings::getInstance().resolution.y));
}

std::ostream& operator<<(std::ostream& os, const object_T& obj) {
    os << "Position: (" << obj.position.x << ", " << obj.position.y << "), ";
    os << "Velocity: (" << obj.velocity.x << ", " << obj.velocity.y << "), ";
    os << "Color: (" << static_cast<int>(obj.color.r) << ", " << static_cast<int>(obj.color.g) << ", " << static_cast<int>(obj.color.b) << "), ";
    os << "Radius: " << obj.radius;
    return os;
}