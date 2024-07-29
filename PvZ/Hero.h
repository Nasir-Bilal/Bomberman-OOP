#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"
#include "moveable.h"

using namespace sf;
const int CHANGE = 3;
//hero

class Hero : public Moveable{
private:
    sf::RectangleShape circle;
    sf::RectangleShape circle2;
    sf::Clock bombClock;
    int lives;
    Bomb* bomb;

public:
    Hero(int x = 52, int y = 52)
        : Moveable(x, y,::CHANGE,8), circle(sf::Vector2f(2, 2)),
        circle2(sf::Vector2f(2, 2)),lives(3) {
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

  
   
    void move(Brick** bricks, int numBricks) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (canMove('U', bricks, numBricks)) {
                pos.updatePosition(0, -CHANGE);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (canMove('D', bricks, numBricks)) {
                pos.updatePosition(0, CHANGE);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (canMove('L', bricks, numBricks) ) {
                pos.updatePosition(-CHANGE, 0);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (canMove('R', bricks, numBricks) ) {
                pos.updatePosition(CHANGE, 0);
                animation(); // Update animation if needed
            }
        }
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

   

    void boom(Brick** brick,Enemy** creep)
    {

        if(bombClock.getElapsedTime().asSeconds()>3)
        {
            if (bomb != NULL)
            {
                bomb->distroyBricks(brick,creep);

                delete bomb;
                bomb = NULL;
            }
        }
    }

    int getLives() { return 3; }
   
};
