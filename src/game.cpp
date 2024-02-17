#include "game.h"

Game_T::Game_T() {
    sf::ContextSettings cSettings;
    cSettings.antialiasingLevel = 4.0;
    window.create(sf::VideoMode(1920, 1080), "Dynamic Voxel Craft", sf::Style::Fullscreen, cSettings);
    if(!window.isOpen()) {
        std::cerr << "Error creating window!" << std::endl;
        abort(); // get an abortion
    }
    if (!font.loadFromFile("/home/noah/github/Dynamic-Voxel-Craft/resources/Arial.ttf")) {
        std::cerr << "Error loading font file!" << std::endl;
        abort();
    }
    if (!shader.loadFromFile(
        "resources/shaders/vertex.glsl",
        "resources/shaders/fragment.glsl"
    )) {
        std::cerr << "Error loading shader file!" << std::endl;
        abort();
    }
}

void Game_T::loop() {
    float Delta;

    while (window.isOpen()) {
        logic();
        Delta = frame_time.asSeconds();
        window.clear(sf::Color::Black);
        shader.setUniform("u_resolution", sf::Vector2f(window.getSize().x, window.getSize().y));
        shader.setUniform("runTime", start_clock.getElapsedTime().asSeconds());
        window.display();
        std::cout << 1/Delta << "\n";
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