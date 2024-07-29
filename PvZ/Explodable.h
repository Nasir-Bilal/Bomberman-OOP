#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Brick.h"

using namespace sf;
class Explodable :public Brick
{
private:
    const int FRAME_WIDTH = 52;
    const int FRAME_HEIGHT = 52;
public:

    Explodable(int x, int y,char id) :Brick(x * 52, y * 52,id)
    {

        if (!texture.loadFromFile("../SFML/Images/ExplodableBlock.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));

    }

    virtual ~Explodable() {}

    void draw(RenderWindow& window)  override {
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }


};