#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Brick.h"
#include "Enemy.h"
#include "entity.h"

using namespace std;
using namespace sf;

class Exit : public Entity
{
private:
    const int WIDTH = 52;
    const int HEIGHT = 52;

public:
    int i;
    Exit(int x, int y, int frames, int xx, int yy,float speed)
        : Entity(x * 52, y * 52, frames, xx, yy,speed)
    {
        i = 0;
        if (!texture.loadFromFile("../SFML/Images/PortalL.png")) {
            cerr << "Error loading Fire texture" << endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
    }

    virtual ~Exit() {}

    void draw(RenderWindow& window) override {

        animation();
        cout << "Drawing Portal!!!:   ";
        cout << pos.x << "  " << pos.y << endl;

        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);

    }


  
};
