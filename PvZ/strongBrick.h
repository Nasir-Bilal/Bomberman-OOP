#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Brick.h"

using namespace sf;
class solidBrick :public Brick
{
 private:
     const int FRAME_WIDTH = 52;
     const int FRAME_HEIGHT = 52;
public:
  
    solidBrick(int x,int y,char id):Brick(x*52,y*52,id)
    {

        if (!texture.loadFromFile("../SFML/Images/SolidBlock.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));

    }

    virtual ~solidBrick(){}

    void draw(RenderWindow& window)  override {
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }


};