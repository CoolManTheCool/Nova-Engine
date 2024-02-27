#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "object.h"
#include "settings.h"

class Game_T {
public:
    Game_T();
    void init_objects();

    void loop();
    void logic();
    void handle_events();
    void draw();

    void q_loop(); //loop called at a maximum of 4 time per second

    ~Game_T();


private:
    sf::RenderWindow window;
    sf::Clock start_clock; // clock that runs throught the entire program and doesn't stop or reset.
    sf::Clock frame_clock; // reset at the start of every frame
    float Delta;       // Time since the last frame began
    sf::Font font;
    sf::Shader shader;     // Frag and vert, no geometry
    Settings& settings = Settings::getInstance();

    //fps text
    sf::Text fps_label;
    std::list<float> fps_iterations;
    float q_time = 0; // time passed, reset every 1/4 seconds

    //Game Data
    std::vector<object_T> objects;
};

#endif