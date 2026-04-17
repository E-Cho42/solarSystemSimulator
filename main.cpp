#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "vector2.h" 
#include "RigidBody.h"

/**
 * @brief Represents a celestial object with physics properties and SFML rendering components.
 */
struct SpaceBody {
    RigidBody body;           // Physics state (mass, pos, vel)
    sf::CircleShape shape;    // Visual representation
    sf::Text label;           // Name display
    sf::VertexArray orbitLine; // Static orbital path indicator
    std::string name;
    float radius;

    SpaceBody(float mass, Vector2 pos, float rad, sf::Color color, std::string n, sf::Font& font, Vector2 sunPos) 
        : body(mass, pos), name(n), radius(rad), orbitLine(sf::PrimitiveType::Lines) {
        
        // Initialize visual properties
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);

        // Configure UI label
        label.setFont(font);
        label.setString(name);
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::White);

        // Generate a dotted circular path based on initial distance from the primary
        if (n != "Sun") {
            float dist = sqrt(pow(pos.x - sunPos.x, 2) + pow(pos.y - sunPos.y, 2));
            for (float a = 0; a < 360; a += 2) {
                float radVal = a * 3.14159f / 180.0f;
                sf::Vector2f point(sunPos.x + cos(radVal) * dist, sunPos.y + sin(radVal) * dist);
                
                // Create dashed effect
                if ((int)a % 4 == 0) {
                    sf::Color dotColor = color;
                    dotColor.a = 80; // Semi-transparent
                    orbitLine.append(sf::Vertex(point, dotColor));
                    orbitLine.append(sf::Vertex(sf::Vector2f(point.x + 1, point.y + 1), dotColor));
                }
            }
        }
    }

    /**
     * @brief Synchronizes SFML shapes with the current physics state.
     * @param zoomLevel Current view scale to keep text legible.
     */
    void sync(float zoomLevel) {
        shape.setPosition(body.position.x, body.position.y);
        label.setPosition(body.position.x + radius + 5, body.position.y - 10);
        label.setScale(zoomLevel, zoomLevel);
    }

    /**
     * @brief Checks if a world-space coordinate intersects the body.
     */
    bool isClicked(sf::Vector2f worldMouse) {
        float dx = worldMouse.x - body.position.x;
        float dy = worldMouse.y - body.position.y;
        return (dx * dx + dy * dy) <= ((radius + 10) * (radius + 10));
    }
};

int main() {
    // --- Window and Context Initialization ---
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Solar System - Pure Planets");
    window.setFramerateLimit(60);

    // Simulation Constants
    const float G = 1.0f; 
    float timeMultiplier = 1.0f;
    float warpLevels[] = {0.0f, 1.0f, 5.0f, 10.0f, 25.0f, 100.0f};
    int warpIndex = 1;

    // Viewport and Interaction State
    sf::View view(sf::FloatRect(0, 0, 1200, 800));
    float zoomLevel = 1.0f;
    bool dragging = false;
    sf::Vector2i lastMousePos;
    SpaceBody* focusedBody = nullptr;

    // Asset Loading
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }

    // --- Scene Setup ---
    Vector2 sunPos(600.0f, 400.0f);
    RigidBody sun(50000.0f, sunPos);
    sf::CircleShape sunShape(35.0f);
    sunShape.setFillColor(sf::Color(255, 240, 150));
    sunShape.setOrigin(35.0f, 35.0f);

    struct PlanetInfo { float dist; float rad; sf::Color col; std::string name; };
    std::vector<PlanetInfo> pData = {
        {90.0f,   4.0f,  sf::Color(170, 170, 170), "Mercury"},
        {150.0f,  8.0f,  sf::Color(255, 198, 73),  "Venus"},
        {220.0f,  9.0f,  sf::Color(50, 150, 255),   "Earth"},
        {300.0f,  6.0f,  sf::Color(255, 100, 50),   "Mars"},
        {480.0f,  22.0f, sf::Color(200, 170, 140), "Jupiter"},
        {680.0f,  18.0f, sf::Color(230, 220, 150), "Saturn"}
    };

    std::vector<SpaceBody> planets;
    for (auto& p : pData) {
        planets.emplace_back(1.0f, sunPos + Vector2(p.dist, 0), p.rad, p.col, p.name, font, sunPos);
        
        // Calculate initial orbital velocity: v = sqrt(GM/r)
        float vMag = sqrt((G * sun.mass) / p.dist);
        planets.back().body.velocity = Vector2(0, vMag);
    }

    // UI Elements
    sf::RectangleShape uiBg(sf::Vector2f(250, 200));
    uiBg.setFillColor(sf::Color(0, 0, 0, 200));
    uiBg.setPosition(15, 15);
    sf::Text uiText;
    uiText.setFont(font);
    uiText.setCharacterSize(14);
    uiText.setPosition(25, 25);

    // --- Main Simulation Loop ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Handle Interaction: Focusing and Panning
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f worldClick = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
                bool hit = false;
                for (auto& p : planets) {
                    if (p.isClicked(worldClick)) { focusedBody = &p; hit = true; break; }
                }
                if (!hit) {
                    // Check sun click manually (not in planet vector)
                    float dx = worldClick.x - sun.position.x;
                    float dy = worldClick.y - sun.position.y;
                    if (dx * dx + dy * dy < 50 * 50) focusedBody = nullptr;
                    else dragging = true; // Drag if clicking empty space
                }
                lastMousePos = sf::Mouse::getPosition(window);
            }

            if (event.type == sf::Event::MouseButtonReleased) dragging = false;

            // Handle Keyboard Input
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) { view.zoom(0.8f); zoomLevel *= 0.8f; }
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) { view.zoom(1.2f); zoomLevel *= 1.2f; }
                if (event.key.code == sf::Keyboard::Period && warpIndex < 5) warpIndex++;
                if (event.key.code == sf::Keyboard::Comma && warpIndex > 0) warpIndex--;
                timeMultiplier = warpLevels[warpIndex];
                if (event.key.code == sf::Keyboard::M) { focusedBody = nullptr; view.setCenter(600, 400); zoomLevel = 1.0f; }
            }
        }

        // Camera Panning Logic
        if (dragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f delta(static_cast<float>(lastMousePos.x - mousePos.x), static_cast<float>(lastMousePos.y - mousePos.y));
            view.move(delta * zoomLevel);
            lastMousePos = mousePos;
            if (delta.x != 0 || delta.y != 0) focusedBody = nullptr;
        }

        // --- Physics Sub-stepping (Improves Numerical Stability) ---
        int steps = 15; 
        float dt = (1.0f / 60.0f) * timeMultiplier / steps;

        for (int i = 0; i < steps; i++) {
            for (auto& p : planets) {
                // Newton's Law of Universal Gravitation: F = G * (m1 * m2) / r^2
                Vector2 sDiff = sun.position - p.body.position;
                float sd2 = sDiff.x * sDiff.x + sDiff.y * sDiff.y;
                float invDist = 1.0f / sqrt(sd2);
                
                // Directional force vector
                p.body.applyForce(sDiff * (G * sun.mass * p.body.mass / (sd2)) * invDist);
                
                // Integrate state
                p.body.update(dt); 
            }
        }
        for (auto& p : planets) p.sync(zoomLevel);

        // Update camera focus if a planet is selected
        if (focusedBody) view.setCenter(focusedBody->body.position.x, focusedBody->body.position.y);

        // --- Rendering ---
        window.clear(sf::Color(5, 5, 15));
        window.setView(view);

        // Draw World Objects
        for (auto& p : planets) window.draw(p.orbitLine);
        sunShape.setPosition(sun.position.x, sun.position.y);
        window.draw(sunShape);
        for (auto& p : planets) {
            window.draw(p.shape);
            window.draw(p.label);
        }

        // Draw UI Overlay (Reset view to default screen space)
        window.setView(window.getDefaultView());
        std::string wStr = (timeMultiplier == 0) ? "PAUSED" : std::to_string((int)timeMultiplier) + "x";
        uiText.setString("ORBITAL MONITOR\nWARP: " + wStr + "\nFOCUS: " + (focusedBody ? focusedBody->name : "Sun") + "\n------------------\nClick Planet to Focus\n[ . / , ] Warp Time\n[ + / - ] Zoom");
        window.draw(uiBg); window.draw(uiText);
        
        window.display();
    }
    return 0;
}