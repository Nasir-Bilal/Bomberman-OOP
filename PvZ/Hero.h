#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"
#include "Fire.h"
#include "moveable.h"
#include "ExitDoor.h"

using namespace sf;
const int CHANGE = 3;
const int BOMBTIMER = 1;
//hero

class Hero : public Moveable{
private:
  
    Clock bombClock;
    Bomb* bomb;

public:
    Clock redClock;
    bool isRed;
    int lives;
    Hero(int x = 52, int y = 52)
        : Moveable(x, y,::CHANGE,8) ,lives(4) {
        if (!texture.loadFromFile("../SFML/Images/Bman2.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }
        sprite.setTexture(texture);
      
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
        bomb = NULL;
        isPresent = true;
        isRed = false;
    }

    void draw(RenderWindow& window) override {
        
        sprite.setPosition(pos.x, pos.y);
       sprite.setScale(0.8f, 0.8f);
        

        if (bomb != NULL)
        {
            bomb->draw(window);
        }
        window.draw(sprite );
    }

  
   
    void move(Brick** bricks, int numBricks) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (canMove('U', bricks, numBricks)) {
                pos.updatePosition(0, -CHANGE);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (canMove('D', bricks, numBricks)) {
                pos.updatePosition(0, CHANGE);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (canMove('L', bricks, numBricks) ) {
                pos.updatePosition(-CHANGE, 0);
                animation(); // Update animation if needed
            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (canMove('R', bricks, numBricks) ) {
                pos.updatePosition(CHANGE, 0);
                animation(); // Update animation if needed
            }
        }
    }

    void plantBomb()
    {
        if (bomb == NULL)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {

                bomb = new Bomb(pos.x,pos.y);
                bombClock.restart();
            }
        }
    }

   

    void boom(Brick** brick,Enemy** creep,Fire* fire[2],Exit*& Portal)
    {

        if(bombClock.getElapsedTime().asSeconds()>BOMBTIMER)
        {
            if (bomb != NULL)
            {
                FloatRect bManBounds(this->sprite.getGlobalBounds());

                bomb->distroyBricks(brick,creep,Portal,bManBounds,lives, this->isRed, this->redClock); //also enemy
                fire[0] = new Fire(bomb->pos.x / 52, ((bomb->pos.y / 52) - 1), "vFire", 5, 1, 3);
               
                //fire[0]->pos.assignPosition(bomb->pos.x, bomb->pos.y - 52);
                fire[1] = new Fire((bomb->pos.x / 52) - 1, ((bomb->pos.y / 52)), "hFire", 5, 3, 1);
               
                fire[0]->distroyBricks(brick, creep);
                fire[1]->distroyBricks(brick, creep);
              //  fire[1]->pos.assignPosition(bomb->pos.x-52, bomb->pos.y);
                delete bomb;
                bomb = NULL;
                bombClock.restart();
            }
            else if (bomb == NULL && fire[0] != NULL&&fire[1]!= NULL)
            {
                
            }
        }

        if (bombClock.getElapsedTime().asSeconds() > BOMBTIMER)
        {
            if (fire != NULL)
            {

                delete fire[0];
                delete fire[1];
                fire[0] = NULL;
                fire[1] = NULL;
            }
        }
    }

    int getLives() { return 3; }
   

   
        bool enemyCollision(Enemy **creep, int numEnemies=Enemy::nEnemey)
        {
            sf::FloatRect bounds = sprite.getGlobalBounds();
            for (int i = 0; i < numEnemies; i++) {
                if (bounds.intersects(creep[i]->sprite.getGlobalBounds())) {
                    return 1;
                }
            }
            return 0;
        }

        bool reallyRed() { return isRed; }
        void setRed(bool x) { isRed = x; }
    
};
