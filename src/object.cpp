#include "object.h"
#include "settings.h"

object_T::object_T(sf::Vector2f position, float radius, sf::Color color) {
    setPosition(position);
    this->radius = radius;
    this->color = color;
    setRadius(radius);
    setPosition(sf::Vector2f(position.x, -position.y));
    setFillColor(color);
    velocity.x += 500;
    velocity.y += -100;
}

void object_T::update(float Delta) {
    sf::Vector2f position = getPosition();
    velocity += Settings::getInstance().gravity * Delta * 50.f; // gravity
    if(getPosition().y < 0) {
        velocity.y *= -1;
        position.y = 0;
    }
    if(getPosition().x < 0) {
        velocity.x *= -1;
        position.x = 0;
    }
    if(getPosition().x > Settings::getInstance().resolution.x) {
        velocity.x *= -1;
        position.x = Settings::getInstance().resolution.x;
    }
    if(getPosition().y > Settings::getInstance().resolution.y) {
        velocity.y *= -1;
        position.y = Settings::getInstance().resolution.y;
    }

    position += velocity * Delta;

    setPosition(sf::Vector2f(position.x, -position.y + Settings::getInstance().resolution.y));
}

std::ostream& operator<<(std::ostream& os, const object_T& obj) {
    os << "Position: (" << obj.getPosition().x << ", " << obj.getPosition().y << "), ";
    os << "Velocity: (" << obj.velocity.x << ", " << obj.velocity.y << "), ";
    os << "Color: (" << static_cast<int>(obj.color.r) << ", " << static_cast<int>(obj.color.g) << ", " << static_cast<int>(obj.color.b) << "), ";
    os << "Radius: " << obj.radius;
    return os;
}
