#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>

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
    sf::Time  frame_time;  // time between start of last frame and this frame
    sf::Font font;
};

#endif