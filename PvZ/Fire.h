#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Brick.h"
#include "Enemy.h"
#include "entity.h"

using namespace std;
using namespace sf;

class Fire : public Entity
{
private:
    const int WIDTH = 52;
    const int HEIGHT = 52;

public:
    Fire(int x, int y, std::string name, int frames, int xx, int yy)
        : Entity(x * 52, y * 52, frames, 52* xx, 52* yy)
    {
        if (!texture.loadFromFile("../SFML/Images/" + name + ".png")) {
            cerr << "Error loading Fire texture" << endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
    }

    virtual ~Fire() {}

    void draw(RenderWindow& window) override {
      
        animation();
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }


    void distroyBricks(Brick** bricks, Enemy** creep)
    {
        const int BOUNDSIZE = 2;
        sf::FloatRect newBounds=this->sprite.getGlobalBounds();
       
        

        

     //   newBounds[0].top -= 35;
       // newBounds[0].height = 40 * 3;
      

        //newBounds[1].left -= 35;
        //newBounds[1].width = 40 * 3;
    
        for (int i = 0; i < bricks[0]->getnBrick() && bricks[i] != NULL; i++) {
          
                if (newBounds.intersects(bricks[i]->sprite.getGlobalBounds())
                    && bricks[i]->getID() == 'W')
                {
                    delete bricks[i];
                    for (int j = i; j < bricks[0]->getnBrick() - 1; j++)
                    {
                        bricks[j] = bricks[j + 1];
                    }
                    bricks[bricks[0]->getnBrick()] = NULL;
                }
            
        }
        for (int i = 0; i < Enemy::getnEnemy(); )
        {
            bool enemyRemoved = false;
           
                if (creep[i] != NULL &&
                    newBounds.intersects(creep[i]->sprite.getGlobalBounds()))
                {
                    //deleteing and shifting array
                    delete creep[i];
                    for (int k = i; k < Enemy::getnEnemy() - 1; k++)
                    {
                        creep[k] = creep[k + 1];
                    }
                    creep[Enemy::getnEnemy() - 1] = NULL; //Seting the last element to NULL
                    int n = Enemy::getnEnemy();
                    Enemy::setnEnemy(n - 1);
                    enemyRemoved = true;
                    break;
                }
            
            if (enemyRemoved)
                continue;
            i++;
        }

    }
};
