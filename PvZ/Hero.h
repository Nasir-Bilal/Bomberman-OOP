#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "gameGrid.h"
#include "Bomb.h"
#include "Brick.h"
#include "entity.h"
#include "Fire.h"
#include "moveable.h"
#include "ExitDoor.h"

using namespace sf;
const int CHANGE = 3;
const int BOMBTIMER = 2;

//note: if the bomb time is increased
//then bombcollisionclock should also be incremented
//hero

class Hero : public Moveable{
private:
  
    Clock bombClock;
    Bomb* bomb;

    SoundBuffer walkBuffer;
    Sound walkSound;

    SoundBuffer bombPlaceBuffer;
    Sound bombPlaceSound; 

    SoundBuffer bombTimerBuffer;
    Sound bombTimerSound;

    SoundBuffer boomBuffer;
    Sound boomSound;


    SoundBuffer brickBuffer;
    Sound brickSound; 
    
    SoundBuffer enemyBuffer;
    Sound enemySound;
   
    Clock walkSound_clock;


public:
    Clock redClock;
    bool isRed;
    int lives;
    Hero(int x = 52, int y = 52)
        : Moveable(x, y,::CHANGE,8) ,lives(3) {
        if (!texture.loadFromFile("../SFML/Images/Bman2.png")) {
            std::cout << "Error loading Bman texture" << std::endl;
        }
        sprite.setTexture(texture);
      
        sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
        bomb = NULL;
        isPresent = true;
        isRed = 0;

        //sound effects

        if (!walkBuffer.loadFromFile("../SFML/Images/walk.wav")) {
            cout << "Error loading sound file!" << std::endl;
        }

        walkSound.setBuffer(walkBuffer);
        walkSound.setVolume(30);

        if (!bombPlaceBuffer.loadFromFile("../SFML/sound/bombPlace.wav")) {
            cout << "Error loading sound file!" << std::endl;
        }
        bombPlaceSound.setBuffer(bombPlaceBuffer);

        if (!bombTimerBuffer.loadFromFile("../SFML/sound/bombTimer.wav")) {
            cout << "Error loading sound file!" << std::endl;
        }

        bombTimerSound.setBuffer(bombTimerBuffer);
        
        if (!boomBuffer.loadFromFile("../SFML/sound/boom.mp3")) {
            cout << "Error loading sound file!" << std::endl;
        }

        boomSound.setBuffer(boomBuffer);

        if (!brickBuffer.loadFromFile("../SFML/sound/brick.wav")) {
            cout << "//Error loading sound file!" << std::endl;
        }

        brickSound.setBuffer(brickBuffer); 
        
        if (!enemyBuffer.loadFromFile("../SFML/sound/enemy.wav")) {
            cout << "Error loading sound file!" << std::endl;
        }

        enemySound.setBuffer(enemyBuffer);
     
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

  
    void effects() {
        animation(); // Update animation if needed

        // Ensure the sound only plays every 100 milliseconds
        if (walkSound_clock.getElapsedTime().asSeconds() >=0.15f ) {
            walkSound.play();
            walkSound_clock.restart();
        }
    }

    void move(Brick** bricks, int numBricks) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (canMove('U', bricks, numBricks)) {
                pos.updatePosition(0, -CHANGE);
                effects();

            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (canMove('D', bricks, numBricks)) {
                pos.updatePosition(0, CHANGE);
                effects();

            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (canMove('L', bricks, numBricks) ) {
                pos.updatePosition(-CHANGE, 0);
                effects();

            }
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (canMove('R', bricks, numBricks) ) {
                pos.updatePosition(CHANGE, 0);
                effects();

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
            

             //   bombPlaceSound.play();
                bombTimerSound.play();
            }
        }
    }

   

    void boom(Brick** brick,Enemy** creep,Fire* fire[2],Exit*& Portal)
    {

        if(bombClock.getElapsedTime().asSeconds()>BOMBTIMER)
        {
            if (bomb != NULL)
            {
                boomSound.play();
                FloatRect bManBounds(this->sprite.getGlobalBounds());
                int num = brick[0]->nBrick;
                int num2 = creep[0]->nEnemey;
                bomb->distroyBricks(brick,creep,Portal,bManBounds,lives, this->isRed, this->redClock); //also enemy
                fire[0] = new Fire(bomb->pos.x / 52, ((bomb->pos.y / 52) - 1), "vFire", 5, 1, 3);
                
                if (num != brick[0]->nBrick)
                {
                    brickSound.play();
                 }
                
                if (num2 != creep[0]->nEnemey)
                {
                    cout << "yesss" << endl;
                    enemySound.play();
                 }

                   
                //fire[0]->pos.assignPosition(bomb->pos.x, bomb->pos.y - 52);
                fire[1] = new Fire((bomb->pos.x / 52) - 1, ((bomb->pos.y / 52)), "hFire", 5, 3, 1);
               
                fire[0]->distroyBricks(brick, creep);
                fire[1]->distroyBricks(brick, creep);
              //  fire[1]->pos.assignPosition(bomb->pos.x-52, bomb->pos.y);
                delete bomb;
                bomb = NULL;
                bombClock.restart();
            }
          
        }

        if (bombClock.getElapsedTime().asSeconds() > 0.8f)
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
