#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "settings.h"
#include "object.h"

class Game_T {
public:
    Game_T();
    void loop();
    void logic();
    void handle_events();
    void draw();
    ~Game_T();


private:
    sf::RenderWindow window;
    sf::Clock start_clock; // clock that runs throught the entire program and doesn't stop or reset.
    sf::Clock frame_clock; // reset at the start of every frame
    // float Delta;       // Time since the last frame began
    sf::Font font;
    sf::Shader shader;     // Frag and vert, no geometry
    settings_T settings = settings_T();

    //Game Data
    std::vector<object_T> objects;
};

#endif