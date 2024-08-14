#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
//#include "Hero.h"
using namespace sf;
using namespace std;

struct Position
{
	int x;
	int y;
    int centerX;
    int centerY;
	Position(int x = 0, int y = 0): x(x),y(y),centerX(x+26),centerY(y+26)
    {
    }

    void updatePosition(int x, int y)
    {
        this->x += x;
        this->y += y;
        centerX += x+26;
        centerY +=y + 26;

    }
    void assignPosition(int x, int y)
    {
        this->x = x;
        this->y = y;
        

    }

};

class Entity
{
public:
    bool isPresent;
    Texture texture;
    Sprite sprite;
    Position pos;

    int currentFrame = 0;
    float FRAME_WIDTH = 52;
    float FRAME_HEIGHT = 50;
    int TOTAL_FRAMES = 8;
    float ANIMATION_SPEED = 0.1f;
    sf::Clock animationClock;
   // Clock animationClock;
public:
    Entity(int x,int y)
        :pos(x,y),isPresent(1){}

    Entity(int x, int y, int frames, float frameWidth = 52, float frameHeight = 50, float animationSpeed = 0.1f)
        : TOTAL_FRAMES(frames),
        pos(x, y),
        FRAME_WIDTH(frameWidth),
        FRAME_HEIGHT(frameHeight),
        ANIMATION_SPEED(animationSpeed),
        isPresent(1) //initilizer list ended here
    {
        TOTAL_FRAMES = frames;
        FRAME_WIDTH = frameWidth;
        FRAME_HEIGHT = frameHeight;
        ANIMATION_SPEED = animationSpeed;
    }


   virtual void draw(RenderWindow& window)  = 0;

   void animation() {
       if (animationClock.getElapsedTime().asSeconds() >= ANIMATION_SPEED) {
           currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
           sprite.setTextureRect(sf::IntRect(currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
           animationClock.restart(); // Restart the clock for the next frame update
       }
   }
  

};