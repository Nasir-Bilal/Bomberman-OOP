#pragma once
#include <SFML/Graphics.hpp>
#include "entity.h"
#include <iostream>

using namespace sf;

class Brick: public Entity
{
protected:
    static int nBrick;
    char id;
    bool isExit;
public:
    Brick(int x, int y,char id) :Entity(x, y),id(id)  
    {
        nBrick = 0; 
        isExit = false;
    }
    bool reallyExit() { return isExit; } //getter
    void yesExit(bool bl) { isExit = bl; }//setter
    
    // Virtual destructor to ensure derived class destructors are called
    virtual ~Brick() {}

    // Pure virtual function to draw the brick
    virtual void draw(RenderWindow& window)  = 0;

    // Set position of the brick
    void setPosition(float x, float y) {
       // shape.setPosition(x, y);
    }
    int getnBrick() { return nBrick; }
    void setnBrick(int n) { nBrick = n; }
    char getID() { return id; }
};

 int Brick::nBrick = 0;