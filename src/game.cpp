#include "game.h"

Game_T::Game_T() {
    window.create(settings.resolution, settings.title, settings.style, settings.context);
    if(!window.isOpen() ||
       !font.loadFromFile("/home/noah/github/Dynamic-Voxel-Craft/resources/Arial.ttf") ||
       !shader.loadFromFile("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl")
        ) {
       std::cerr << "Error duing game init.";
       abort(); 
    }
}

void Game_T::loop() {
    float Delta;

    while (window.isOpen()) {
        logic(); // Compute physics, user input.
        draw();  // Display results.
    }
}

void Game_T::logic() {
    DeltaTime = frame_clock.restart().asSeconds();
    handle_events();
}

void Game_T::draw() {
    window.clear(sf::Color::Black);
    window.display();
}

void Game_T::handle_events() {
    sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close(); // Close window when Escape key is pressed
                    else if (event.key.code == sf::Keyboard::Space)
                        shader.loadFromFile("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
                    break;
                default:
                    break;
            }
        }
}

Game_T::~Game_T() {
    std::cout << "Game object deconstructed! :O BYE BYE!\n";
}