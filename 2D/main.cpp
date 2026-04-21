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
        
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);

        label.setFont(font);
        label.setString(name);
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::White);

        if (n != "Sun") {
            float dist = sqrt(pow(pos.x - sunPos.x, 2) + pow(pos.y - sunPos.y, 2));
            for (float a = 0; a < 360; a += 2) {
                float radVal = a * 3.14159f / 180.0f;
                sf::Vector2f point(sunPos.x + cos(radVal) * dist, sunPos.y + sin(radVal) * dist);
                
                if ((int)a % 4 == 0) {
                    sf::Color dotColor = color;
                    dotColor.a = 80; 
                    orbitLine.append(sf::Vertex(point, dotColor));
                    orbitLine.append(sf::Vertex(sf::Vector2f(point.x + 1, point.y + 1), dotColor));
                }
            }
        }
    }

    void sync(float zoomLevel) {
        shape.setPosition(body.position.x, body.position.y);
        label.setPosition(body.position.x + radius + 5, body.position.y - 10);
        label.setScale(zoomLevel, zoomLevel);
    }

    bool isClicked(sf::Vector2f worldMouse) {
        float dx = worldMouse.x - body.position.x;
        float dy = worldMouse.y - body.position.y;
        return (dx * dx + dy * dy) <= ((radius + 10) * (radius + 10));
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Solar System - Pure Planets");
    window.setFramerateLimit(60);

    Vector2 sunPos(600.0f, 400.0f);
    sf::CircleShape sunShape(35.0f);
    sunShape.setFillColor(sf::Color(255, 240, 150));
    sunShape.setOrigin(35.0f, 35.0f);
    RigidBody sun(50000.0f, sunPos);

    const float DAYS_PER_YEAR = 365.25f;
    const float SECONDS_PER_DAY = 1.0f; 
    const float YEAR = DAYS_PER_YEAR * SECONDS_PER_DAY;
    float earthDist = 149.0f;

    float G = (4 * 3.1415926f * 3.1415926f * pow(earthDist, 3)) / (YEAR * YEAR * sun.mass); 
    
    float timeMultiplier = 1.0f;
    // Updated Warp Levels
    float warpLevels[] = {0.0f, 1.0f, 5.0f, 10.0f, 25.0f, 100.0f, 500.0f, 1000.0f};
    int warpIndex = 1;

    sf::View view(sf::FloatRect(0, 0, 1200, 800));
    float zoomLevel = 1.0f;
    bool dragging = false;
    sf::Vector2i lastMousePos;
    SpaceBody* focusedBody = nullptr;

    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }

    struct PlanetInfo { float dist; float rad; sf::Color col; std::string name; };
    std::vector<PlanetInfo> pData = {
        {58.0f,   2.4f,  sf::Color(170, 170, 170), "Mercury"},
        {108.0f,  6.0f,  sf::Color(255, 198, 73),  "Venus"},
        {149.0f,  6.3f,  sf::Color(50, 150, 255),   "Earth"},
        {228.0f,  3.4f,  sf::Color(255, 100, 50),   "Mars"},
        {778.0f,  43.4f, sf::Color(200, 170, 140), "Jupiter"},
        {1434.0f,  58.2f, sf::Color(230, 220, 150), "Saturn"},
        {2871.0f, 15.8f, sf::Color(50,50,230), "Uranus"},
        {4459.0f, 15.2f, sf::Color(20,20,250), "Neptune"}
    };

    std::vector<SpaceBody> planets;
    for (auto& p : pData) {
        planets.emplace_back(1.0f, sunPos + Vector2(p.dist, 0), p.rad, p.col, p.name, font, sunPos);
        float vMag = sqrt((G * sun.mass) / p.dist); 
        planets.back().body.velocity = Vector2(0, vMag);
    }

    sf::RectangleShape uiBg(sf::Vector2f(260, 240));
    uiBg.setFillColor(sf::Color(0, 0, 0, 200));
    uiBg.setPosition(15, 15);
    sf::Text uiText;
    uiText.setFont(font);
    uiText.setCharacterSize(14);
    uiText.setPosition(25, 25);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f worldClick = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
                bool hit = false;
                for (auto& p : planets) {
                    if (p.isClicked(worldClick)) { focusedBody = &p; hit = true; break; }
                }
                if (!hit) {
                    float dx = worldClick.x - sun.position.x;
                    float dy = worldClick.y - sun.position.y;
                    if (dx * dx + dy * dy < 50 * 50) focusedBody = nullptr;
                    else dragging = true;
                }
                lastMousePos = sf::Mouse::getPosition(window);
            }

            if (event.type == sf::Event::MouseButtonReleased) dragging = false;

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) { view.zoom(0.8f); zoomLevel *= 0.8f; }
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) { view.zoom(1.2f); zoomLevel *= 1.2f; }
                
                // Updated index bounds (0 to 7 now)
                if (event.key.code == sf::Keyboard::Period && warpIndex < 7) warpIndex++;
                if (event.key.code == sf::Keyboard::Comma && warpIndex > 0) warpIndex--;
                
                timeMultiplier = warpLevels[warpIndex];
                if (event.key.code == sf::Keyboard::M) { focusedBody = nullptr; view.setCenter(600, 400); zoomLevel = 1.0f; }
            }
        }

        if (dragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f delta(static_cast<float>(lastMousePos.x - mousePos.x), static_cast<float>(lastMousePos.y - mousePos.y));
            view.move(delta * zoomLevel);
            lastMousePos = mousePos;
            if (delta.x != 0 || delta.y != 0) focusedBody = nullptr;
        }

        // Adaptive physics steps: More steps at high warp to maintain orbit stability
        int steps = (timeMultiplier > 100.0f) ? 100 : 25; 
        float dt = (SECONDS_PER_DAY / 60.0f) * timeMultiplier / steps;

        for (int i = 0; i < steps; i++) {
            for (auto& p : planets) {
                Vector2 sDiff = sun.position - p.body.position;
                float sd2 = sDiff.x * sDiff.x + sDiff.y * sDiff.y;
                float dist = sqrt(sd2);
                
                if (dist > 0) {
                    p.body.applyForce(sDiff * (G * sun.mass * p.body.mass / sd2) * (1.0f / dist));
                }
                p.body.update(dt); 
            }
        }
        for (auto& p : planets) p.sync(zoomLevel);

        if (focusedBody) view.setCenter(focusedBody->body.position.x, focusedBody->body.position.y);

        window.clear(sf::Color(5, 5, 15));
        window.setView(view);

        for (auto& p : planets) window.draw(p.orbitLine);
        sunShape.setPosition(sun.position.x, sun.position.y);
        window.draw(sunShape);
        for (auto& p : planets) {
            window.draw(p.shape);
            window.draw(p.label);
        }

        window.setView(window.getDefaultView());
        std::string wStr = (timeMultiplier == 0) ? "PAUSED" : std::to_string((int)timeMultiplier) + "x";
        
        uiText.setString("ORBITAL MONITOR\n"
                         "WARP: " + wStr + "\n"
                         "FOCUS: " + (focusedBody ? focusedBody->name : "Sun") + "\n"
                         "------------------\n"
                         "SCALE: Real Orbital Physics\n"
                         "TIME: 1 Second = 1 Day (at 1x)\n"
                         "------------------\n"
                         "Click Planet to Focus\n"
                         "[ . / , ] Warp Time\n"
                         "[ + / - ] Zoom\n"
                         "[ M ] Reset Camera");
                         
        window.draw(uiBg); window.draw(uiText);
        window.display();
    }
    return 0;
}