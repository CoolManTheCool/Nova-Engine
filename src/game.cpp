#include "game.h"

Game_T::Game_T() {
    sf::VideoMode vm(settings.resolution.x, settings.resolution.y);
    window.create(vm, settings.title, settings.style, settings.context);
    if(!window.isOpen() ||
       !font.loadFromFile("/home/noah/github/Dynamic-Voxel-Craft/resources/Arial.ttf") ||
       !shader.loadFromFile("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl")
        ) {
       std::cerr << "Error duing game init.";
       abort(); 
    }
    //imgui

    

    //game data init
    //objects.resize(settings.max_objects);
    // Variables to use
    // settings.resolution
    // settings.inital_objects
    sf::Vector2i res(settings.resolution);
    int nobs(settings.inital_objects); //number of objects
    int spacing = std::sqrt((res.x * res.y) / nobs);
    int i = 1;
    for (int y = 1; y < res.y/spacing; y++) {
        for (int x = 1; x < res.x/spacing; x++)
        {
            int a = x*spacing;
            int b = y*spacing;
            std::cout << "Spawned object " << i << " at position: (" << a << ", " << b << ")" << std::endl;
            i++;
        }
        
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
    Delta = frame_clock.restart().asSeconds();
    handle_events();
}

void Game_T::draw() {
    window.clear(sf::Color::Black);

    for(auto &obj : objects) {
        window.draw(obj, &shader);
    }

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