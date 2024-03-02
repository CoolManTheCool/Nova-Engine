#ifndef OBJECT_H
#define OBJECT_H

#include "SFML/Graphics.hpp"
#include <array>

class object_T : public sf::Drawable, public sf::Transformable {
public:
    object_T() : m_shader(nullptr) {}

    void setVertices(const std::vector<sf::Vector3f>& vertices) {
        m_vertices = vertices;
    }

    void setTriangles(const std::vector<std::array<int, 3>>& triangles) {
        m_triangles = triangles;
    }

    void setShader(const sf::Shader& shader) {
        m_shader = &shader;
    }



private:
    std::vector<sf::Vector3f> m_vertices;
    std::vector<std::array<int, 3>> m_triangles; // Stores indices of vertex triples forming triangles
    const sf::Shader* m_shader;
};

#endif
