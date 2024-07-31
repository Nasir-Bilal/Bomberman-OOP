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

   

public:

    Bomb(int x = 52, int y = 52)
        : Entity(x, y)  
    {
        if (!texture.loadFromFile("../SFML/Images/Bomb.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
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
        cout << "Get Position: "<<endl << pos.x << endl;
        cout << pos.y << endl;
        
        window.draw(sprite);
    }

    void printBounds(const sf::FloatRect& bounds) {
        std::cout << "Left: " << bounds.left << std::endl;
        std::cout << "Top: " << bounds.top << std::endl;
        std::cout << "Width: " << bounds.width << std::endl;
        std::cout << "Height: " << bounds.height << std::endl;
    }


    void distroyBricks(Brick** bricks,Enemy** creep,Exit*& Portal)
    {
        const int BOUNDSIZE = 2;
        sf::FloatRect newBounds[BOUNDSIZE];
        for (int i = 0; i < BOUNDSIZE; i++)
        {
            newBounds[i] = sprite.getGlobalBounds();
        }

        printBounds(newBounds[0]);
        printBounds(newBounds[1]);
        cout << endl;

        newBounds[0].top -= 35;
        newBounds[0].height = 40 * 3;
        printBounds(newBounds[0]);

        newBounds[1].left -= 35;
        newBounds[1].width = 40 * 3;
        printBounds(newBounds[1]);

        for (int i = 0; i < bricks[0]->getnBrick() && bricks[i]!=NULL; i++) {
            for(int j=0; j<BOUNDSIZE; j++)
            {
                if (newBounds[j].intersects(bricks[i]->sprite.getGlobalBounds())
                    && bricks[i]->getID() == 'W')
                {
                    if (bricks[i]->reallyExit())
                    {
                        Portal = new Exit(bricks[i]->pos.x/52, bricks[i]->pos.y/52,10,54,54,0.2f);
                        Portal->pos.x -= 1;
                        Portal->pos.y -= 1;
                        cout << "Portal Created!!!" << endl;
                        
                     }
                    
                    delete bricks[i];
                    for (int j = i; j < bricks[0]->getnBrick() - 1; j++)
                    {
                        bricks[j] = bricks[j + 1];
                    }
                    bricks[bricks[0]->getnBrick()] = NULL;
                    
                }
            }
        }


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