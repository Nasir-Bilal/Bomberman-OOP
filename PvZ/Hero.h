#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"

using namespace sf;
int CHANGE = 2;
//hero

class Hero : public Entity {
private:
    sf::RectangleShape circle;
    sf::RectangleShape circle2;
    sf::Clock animationClock;  
    sf::Clock bombClock;
    int currentFrame = 0;      
    const float FRAME_WIDTH = 52;
    const float FRAME_HEIGHT = 50;
    const int TOTAL_FRAMES = 8;
    const float ANIMATION_SPEED = 0.1f; 
    
    Bomb* bomb;

public:
    Hero(int x = 52, int y = 52)
        : Entity(x, y), circle(sf::Vector2f(2, 2)), circle2(sf::Vector2f(2, 2)) {
        if (!texture.loadFromFile("../SFML/Images/Bman2.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }
        sprite.setTexture(texture);
        circle.setFillColor(sf::Color::Green);
        circle2.setFillColor(sf::Color::Green);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
        bomb = NULL;
        isPresent = true;
    }

    void draw(RenderWindow& window) override {
        circle2 = circle;
        sprite.setPosition(pos.x, pos.y);
       sprite.setScale(0.8f, 0.8f);
        circle.setPosition(pos.x, pos.y);
        circle2.setPosition(pos.x, pos.y + 128);
        window.draw(circle);
        window.draw(circle2);


        if (bomb != NULL)
        {
            bomb->draw(window);
        }
        window.draw(sprite);
    }

  
    bool canMove(char direction, Brick** bricks, int numBricks) {
        // Calculate new position based on direction
        sf::FloatRect newBounds = sprite.getGlobalBounds();

        switch (direction) {
        case 'U':
            newBounds.top -= CHANGE;
            break;
        case 'D':
            newBounds.top += CHANGE;
            break;
        case 'L':
            newBounds.left -= CHANGE;
            break;
        case 'R':
            newBounds.left += CHANGE;
            break;
        }



        // Check for collisions with all bricks
        for (int i = 0; i < numBricks; i++) {
            if (newBounds.intersects(bricks[i]->sprite.getGlobalBounds())) {
                return false; // Collision detected
            }
        }

        return true; // No collision detected
    }
    void move(Brick** bricks, int numBricks) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (canMove('U', bricks, numBricks)) {
                pos.updatePosition(0, -CHANGE);
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (canMove('D', bricks, numBricks)) {
                pos.updatePosition(0, CHANGE);
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (canMove('L', bricks, numBricks) ) {
                pos.updatePosition(-CHANGE, 0);
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (canMove('R', bricks, numBricks) ) {
                pos.updatePosition(CHANGE, 0);
            }
        }

        animation(); // Update animation if needed
    }

    void plantBomb()
    {
        if (bomb == NULL)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                bomb = new Bomb(pos.x,pos.y);
                bombClock.restart();
            }
        }
    }

   

    void boom(Brick** brick)
    {

        if(bombClock.getElapsedTime().asSeconds()>3)
        {
            if (bomb != NULL)
            {
                bomb->distroyBricks(brick);

                delete bomb;
                bomb = NULL;
            }
        }
    }


    void animation() {
        if (animationClock.getElapsedTime().asSeconds() >= ANIMATION_SPEED) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            sprite.setTextureRect(sf::IntRect(currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
            animationClock.restart(); // Restart the clock for the next frame update
        }
    }
};
