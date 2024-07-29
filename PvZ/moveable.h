#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"

class Moveable : public Entity
{
protected:

    int currentFrame = 0;
    float FRAME_WIDTH = 52;
    const float FRAME_HEIGHT = 50;
    const int TOTAL_FRAMES = 8;
    const float ANIMATION_SPEED = 0.1f;
    sf::Clock animationClock;
   const int CHANGE;

public:
    Moveable(int x, int y,int change,int frames)
        :TOTAL_FRAMES(frames), Entity(x,y), CHANGE(change) {}

    virtual void draw(RenderWindow& window) = 0;

    virtual ~Moveable() {}

    bool canMove(char direction, Brick** bricks, int numBricks)
    {
        
        sf::FloatRect newBounds = sprite.getGlobalBounds();
        cout << CHANGE << endl;
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

        // resolution
        const float windowX = 780.0f-52;
        const float windowY = 676.0f;

        //collisions with walls
        if (newBounds.left < 52 || newBounds.top < 52 
            || newBounds.left + newBounds.width > windowX
            || newBounds.top + newBounds.height > windowY )
        {
            return false; 
        }

        // Check for collisions
        for (int i = 0; i < numBricks; i++) {
            if (newBounds.intersects(bricks[i]->sprite.getGlobalBounds())) {
                return false; // Collision 
            }
        }
        return true; // No collision 
    }



    void animation() {
        if (animationClock.getElapsedTime().asSeconds() >= ANIMATION_SPEED) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            sprite.setTextureRect(sf::IntRect(currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
            animationClock.restart(); // Restart the clock for the next frame update
        }
    }
};
