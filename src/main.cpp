#include "game.h"
#include <iostream>
#include <functional>

int main() {
    // clear console
    //for (int i = 0; i < 20; ++i) {
    //    std::cout << "\n";
    //}

    Game_T Game;
    Game.loop();

    return 0;
}

/*
sf::RenderWindow window(sf::VideoMode(960, 540), "Dynamic Voxel Craft");
    sf::Font font;
    if (!font.loadFromFile("/home/noah/github/Dev-Box/resources/Arial.ttf")) {
        std::cerr << "Error loading font file!" << std::endl;
        return 1;
    }
    
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition(15, 15);

    sf::Clock clock;
    sf::Time elapsedTime;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        elapsedTime = clock.restart();
        float fps = 1.0f / elapsedTime.asSeconds();

        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

        window.clear(sf::Color::Black);

        window.draw(fpsText);

        window.display();
    }

    return 0;*/