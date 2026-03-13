#include <iostream>  
#include <SFML/Graphics.hpp>
#include "vector2.h" 
#include "RigidBody.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "PhysEngine");
    window.setFramerateLimit(60); 

    float radius = 20.0f;
    RigidBody ball(1.0f, Vector2(600.0f, 400.0f));
    
    sf::CircleShape ballVisual(radius); 
    ballVisual.setFillColor(sf::Color::Green);
    ballVisual.setOrigin(radius, radius); 

    Vector2 gravity(0.0f, 500.0f); 
    float bounciness = -0.7f;

    while(window.isOpen()){
        sf::Event event;
        
        // --- EVENT LOOP (Handles Input) ---
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    float dx = (float)mousePos.x - ball.position.x;
                    float dy = (float)mousePos.y - ball.position.y;
                    
                    // We use a high multiplier because applyForce is dampened by mass/time
                    Vector2 launchForce(dx * 50.0f, dy * 50.0f);
                    ball.applyForce(launchForce);
                }
            }
        } // Event loop ends here

        // PHYSICS UPDATE (Happens every frame) 
        ball.applyForce(gravity);
        ball.update(1.0f / 60.0f);

        // BOUNDARIES 
        if (ball.position.y + radius > 800.0f) {
            ball.position.y = 800.0f - radius;
            ball.velocity.y *= bounciness;
            ball.velocity.x *= 0.98f; 
        }
        else if (ball.position.y - radius < 0.0f) {
            ball.position.y = 0.0f + radius;
            ball.velocity.y *= bounciness;
        }

        if (ball.position.x + radius > 1200.0f) {
            ball.position.x = 1200.0f - radius;
            ball.velocity.x *= bounciness;
        }
        else if (ball.position.x - radius < 0.0f) {
            ball.position.x = 0.0f + radius;
            ball.velocity.x *= bounciness;
        }

        // RENDERING 
        ballVisual.setPosition(ball.position.x, ball.position.y);

        window.clear();
        window.draw(ballVisual); 
        window.display();
    }

    return 0;
}