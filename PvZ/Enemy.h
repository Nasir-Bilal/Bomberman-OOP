#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"
#include "moveable.h"

using namespace std;
const int CHANGE1 = 1;


class Enemy : public Moveable
{
protected:
    sf::RectangleShape circle;
    sf::RectangleShape circle2;
    static int nEnemey;
    char ch;
public:
    Enemy(int x = 52, int y = 52)
        : Moveable(x, y, ::CHANGE1,6), circle(sf::Vector2f(2, 2))
        , circle2(sf::Vector2f(2, 2)), ch('A')
    {
        if (!texture.loadFromFile("../SFML/Images/creep.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }
        
        sprite.setTexture(texture);
        circle.setFillColor(sf::Color::Green);
        circle2.setFillColor(sf::Color::Green);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
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

        window.draw(sprite);
    }



    void move(Brick** bricks, int numBricks) {
       
        switch (ch) {
        case 'A':
            if (canMove('U', bricks, numBricks)) {
                pos.updatePosition(0, -CHANGE);
                animation(); // Update animation if needed

            }
            else {
                ch = 'A' + rand() % 4;
            }
            break;

        case 'C':
            if (canMove('D', bricks, numBricks)) {
                pos.updatePosition(0, CHANGE);
                animation(); // Update animation if needed
            }
            else {
                ch = 'A' + rand() % 4;
            }
            break;

        case 'B':
            if (canMove('L', bricks, numBricks)) {
                pos.updatePosition(-CHANGE, 0);
                animation(); // Update animation if needed
            }
            else {
               ch = 'A' + rand() % 4;

            }
            break;

        case 'D':
            if (canMove('R', bricks, numBricks)) {
                pos.updatePosition(CHANGE, 0);
                animation(); // Update animation if needed
            }
            else {
                ch = 'A' + rand() % 4;

            }
            break;

        default:
            // Handle any other key presses or do nothing
            break;
        }
    }

   static int getnEnemy() { return Enemy::nEnemey; }
   static void decrement_n_Enemy() { Enemy::nEnemey--; }
   static void setnEnemy(int x) { Enemy::nEnemey = x; }

};

int Enemy::nEnemey = 0;