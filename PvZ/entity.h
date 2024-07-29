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
   // Clock animationClock;
public:
    Entity(int x,int y)
        :pos(x,y),isPresent(0){}

   virtual void draw(RenderWindow& window)  = 0;


  

};