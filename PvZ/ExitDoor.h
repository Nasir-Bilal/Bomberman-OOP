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
    SoundBuffer portalBuffer;
    Sound portalSound;


public:
    int i;
    Exit(Texture& tt , int x, int y, int frames, int xx, int yy,float speed)
        : Entity(x * 52, y * 52, frames, xx, yy,speed)
    {
        i = 0;
        texture = tt;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
        if (!portalBuffer.loadFromFile("../SFML/sound/portal.mp3")) {
            cout << "//Error loading sound file!" << std::endl;
        }
        portalSound.setBuffer(portalBuffer);
        portalSound.play();
    }
    virtual ~Exit() {}
    void draw(RenderWindow& window) override 
    {
        animation();
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }
};
