#include "game.h"
#include <iostream>

Game_T::Game_T() {
    sf::ContextSettings cSettings;
    cSettings.antialiasingLevel = 16.0;
    window.create(sf::VideoMode(960, 540), "Dynamic Voxel Craft", sf::Style::Default, cSettings);
    if(!window.isOpen()) {
        std::cerr << "Error creating window!" << std::endl;
        abort(); // get an abortion
    }
    if (!font.loadFromFile("/home/noah/github/Dev-Box/resources/Arial.ttf")) {
        std::cerr << "Error loading font file!" << std::endl;
        abort();
    }
}

void Game_T::loop() {

    sf::Text FpsText;
    FpsText.setFont(font);
    FpsText.setCharacterSize(20);
    FpsText.setFillColor(sf::Color::Green);
    FpsText.setPosition(15, 15);
    sf::ConvexShape triangle;
    triangle.setPointCount(3); // Set the number of points (vertices) in the triangle
    triangle.setPoint(0, sf::Vector2f(200, 50)); // Set the first point
    triangle.setPoint(1, sf::Vector2f(100, 300)); // Set the second point
    triangle.setPoint(2, sf::Vector2f(300, 300)); // Set the third point
    triangle.setFillColor(sf::Color::Green); // Set the fill color of the triangle


    while (window.isOpen()) {
        logic();
        float fps = 1.0f / frame_time.asSeconds();

        FpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

        window.clear(sf::Color::Black);

        window.draw(FpsText);
        window.draw(triangle);

        window.display();
    }
}

void Game_T::logic() {
    handle_events();
    frame_time = frame_clock.restart();
}

void Game_T::draw() {

}

void Game_T::handle_events() {
    sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
}

Game_T::~Game_T() {
    std::cout << "Game object deconstructed! :O BYE BYE!";
}