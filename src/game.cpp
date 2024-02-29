#include "game.h"
#include <cmath>
#include <string>
#include <iostream>

Game_T::Game_T() {
    sf::VideoMode vm(settings.resolution.x, settings.resolution.y);
    window.create(vm, settings.title, settings.style, settings.context);
    if(!window.isOpen()
    || !font.loadFromFile("/home/noah/github/Dynamic-Voxel-Craft/resources/Arial.ttf")
       || !load_shaders()
        ) {
       std::cerr << "Error duing game init.";
       abort(); 
    }
    //TODO: ImGUI

    //fps label
    for (int i = 0; i < settings.fps_smoothing; ++i) {
        fps_iterations.push_back(99999);
    }
    
    fps_label.setFillColor(sf::Color::Red);
    fps_label.setFont(font);
    fps_label.setPosition(sf::Vector2f(15, 15));

    init_objects();
    //for(object_T &obj : objects) {
    //    std::cout << obj << "\n";
    //}
}

bool Game_T::load_shaders() {
    bool is_success
    if(settings.shaders) {
        shader.loadFromFile("../resources/shaders/vertex.glsl", "../resources/shaders/fragment.glsl");
    }
    return true;
}

void Game_T::init_objects() {
    sf::Vector2i res(settings.resolution);
    int nobs(settings.initial_objects); //number of objects
    int count = 1;
    int spacing = std::sqrt((res.x * res.y) / (nobs));
    for (int y = 1; y < res.y/spacing+1; y++) {
        for (int x = 1; x < res.x/spacing+5; x++)
        {
            int a = x*spacing;
            int b = y*spacing;
            objects.push_back(object_T(sf::Vector2f((float)a, (float)b), 5, sf::Color::Green));
            count++;
        }
        
    }
}

void Game_T::loop() {
    while (window.isOpen()) {
        logic(); // Compute physics, user input.
        draw();  // Display results.
    }
}

void Game_T::q_loop() {
    float average_fps = 0;
    for(float fps : fps_iterations) {
        average_fps += fps;
    }
    average_fps /= settings.fps_smoothing;
    fps_label.setString("Average FPS: " + std::to_string(static_cast<int>(average_fps)));
}

void Game_T::logic() {
    Delta = frame_clock.restart().asSeconds();
    fps_iterations.push_back(1.f / Delta);
    fps_iterations.pop_front();
    q_time += Delta;
    if(q_time >= 0.25f) {
        q_time = 0;
        q_loop();
    }

    for(object_T &obj : objects) {
        obj.update(Delta);
    }

    handle_events();
}

void Game_T::draw() {
    window.clear(sf::Color::Black);

    for(object_T &obj : objects) {
        window.draw(obj);
    }
    window.draw(fps_label);

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
                        load_shaders();
                    break;
                default:
                    break;
            }
        }
}

Game_T::~Game_T() {
    //TODO: Make this not pointless
}
