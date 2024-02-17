#include "game.h"

Game_T::Game_T() {
    sf::ContextSettings cSettings;
    cSettings.antialiasingLevel = 4.0;
    window.create(sf::VideoMode(960, 540), "Dynamic Voxel Craft", sf::Style::Default, cSettings);
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
        std::cerr << "Error loading font file!" << std::endl;
        abort();
    }
}

void Game_T::loop() {
    double Delta;
    sf::VertexArray screen(sf::TriangleStrip, 4);

    // Append vertices for a rectangle
    screen[0].position = sf::Vector2f(-1, -1);
    screen[1].position = sf::Vector2f(-1, 1);
    screen[2].position = sf::Vector2f(1, -1);
    screen[3].position = sf::Vector2f(1, 1);
    while (window.isOpen()) {
        logic();
        Delta = frame_time.asSeconds();
        window.clear(sf::Color::Black);

        shader.setUniform("time", static_cast<float>(start_clock.getElapsedTime().asSeconds()));
        shader.setUniform("resolution", sf::Vector2f(window.getSize().x, window.getSize().y));

        window.draw(screen, &shader);

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
    std::cout << "Game object deconstructed! :O BYE BYE!\n";
}