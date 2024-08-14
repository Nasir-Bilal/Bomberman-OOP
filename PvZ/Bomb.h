#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Brick.h"
#include "Enemy.h"
#include "entity.h"
#include "ExitDoor.h"


class Bomb :public Entity
{
private:
    int currentFrame = 0;
    const float FRAME_WIDTH = 40;
    const float FRAME_HEIGHT = 40;
    const int TOTAL_FRAMES = 3;
    const float ANIMATION_SPEED = 0.1f;
	sf::Clock animationClock;
    Clock bombCollisionClock;
    
    Texture textureForPortal;


public:

    Bomb(int x = 52, int y = 52)
        : Entity(x, y)  
    {
        if (!texture.loadFromFile("../SFML/Images/Bomb.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }
        if (!textureForPortal.loadFromFile("../SFML/Images/PortalL.png")) {
            cerr << "Error loading Fire texture" << endl;
        }


        sprite.setTexture(texture);
      
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
        sprite.setPosition(pos.x, pos.y);
        isPresent = true;

    }

    ~Bomb(){}

    void draw(RenderWindow & window)
    {
        animation();
      
        window.draw(sprite);
    }

    void printBounds(const sf::FloatRect& bounds)
    {
      
    }


    void distroyBricks(Brick** bricks,Enemy** creep,Exit*& Portal,
        FloatRect& bManBounds,int&lives, bool& isRed, Clock& redClock)
    {
        const int BOUNDSIZE = 2;
        sf::FloatRect newBounds[BOUNDSIZE];
        for (int i = 0; i < BOUNDSIZE; i++)
        {
            newBounds[i] = sprite.getGlobalBounds();
        }

        newBounds[0].top -= 35;
        newBounds[0].height = 40 * 3;
      
        newBounds[1].left -= 35;
        newBounds[1].width = 40 * 3;
        
        //if the bomb time is increases
        //then bombcollisionclock should also be incremented
        if(bombCollisionClock.getElapsedTime().asSeconds()<3 && !isRed)
        //bman and bomb collision
        {
            for (int i = 0; i < BOUNDSIZE; i++)
            {
                if (newBounds[i].intersects(bManBounds))
                {
                    lives--;
                    isRed = 1;
                    redClock.restart();
                    break;
                }
            }
        }

        //brick and bomb collision
      // Assume bricks[0]->getnBrick() returns the number of bricks currently in the array
        int totalBricks = bricks[0]->getnBrick();

        for (int i = 0; i < totalBricks; i++) {
            if (bricks[i] == NULL) {
                continue;  // Skip already deleted bricks
            }

            for (int j = 0; j < BOUNDSIZE; j++) {
                if (newBounds[j].intersects(bricks[i]->sprite.getGlobalBounds()) && bricks[i]->getID() == 'W') {
                   
                    
                    if (bricks[i]->reallyExit()) {

                        // Create a new Portal object
                        Portal = new Exit(textureForPortal,bricks[i]->pos.x / 52, bricks[i]->pos.y / 52, 10, 54, 54, 0.2f);
                        Portal->i = i;
                        Portal->pos.x -= 1;
                        Portal->pos.y -= 1;
                    }

                    // Delete the brick
                    delete bricks[i];
                    bricks[i] = NULL;

                    // Shift the remaining bricks to fill the gap
                    for (int k = i; k < totalBricks - 1; k++) {
                        bricks[k] = bricks[k + 1];
                    }

                    bricks[totalBricks - 1] = NULL;
                   // bricks[0]->nBrick--;
                    totalBricks--;

                    // Break the inner loop and re-check current index

                    break;
                }
            }
        }


        //enemy and bomb colliion
        for (int i = 0; i < Enemy::getnEnemy(); )
        {
            bool enemyRemoved = false; 
            for (int j = 0; j < BOUNDSIZE; j++)
            {
                if (creep[i] != NULL &&
                    newBounds[j].intersects(creep[i]->sprite.getGlobalBounds()))
                {
                   //deleteing and shifting array
                    delete creep[i];
                    for (int k = i; k < Enemy::getnEnemy() - 1; k++)
                    {
                        creep[k] = creep[k + 1];
                    }
                    creep[Enemy::getnEnemy() - 1] = NULL; //Seting the last element to NULL
                    //Enemy::setnEnemy( n-1);
                    Enemy::decrement_n_Enemy();
                    int n = Enemy::getnEnemy();
                   // cout << n << endl;
                    enemyRemoved = true;
                   

                    break; 
                }
            }
            if (enemyRemoved)
                continue;
            i++;
        }

    }


    void animation() {
        if (animationClock.getElapsedTime().asSeconds() >= ANIMATION_SPEED) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            sprite.setTextureRect(sf::IntRect(currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
            animationClock.restart(); // Restart the clock for the next frame update
        }
    }

};