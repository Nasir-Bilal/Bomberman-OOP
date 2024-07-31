#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.h"
#include "Hero.h"
#include "Brick.h"
#include "strongBrick.h"
#include "Explodable.h"
#include "Enemy.h"
#include "gameGrid.h"
#include "Fire.h"
#include "ExitDoor.h"
#include <ctime>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

const int MAX_BRICKS = 300; // Adjust as needed
const int MAX_ENEMY = 10;

class Game
{
private:
    sf::Image menu_image;
    sf::Texture menu_t;
    sf::Sprite menu_s;

    sf::Image map_image;
    sf::Texture t_map;
    sf::Sprite s_map;
    sf::RenderWindow window;
    int menuState;
    int currentScreen;
    Clock clk;

    GameGrid grid;
    Brick* bricks[MAX_BRICKS]; // Fixed-size array for Brick pointers
    int nBricks;
    Hero bMan;
    Enemy* creep[MAX_ENEMY];
    int nEnemy;
    Fire* fire[2];
    sf::CircleShape* cr;
    sf::SoundBuffer buffer;
    sf::Sound sound;

    sf::Clock delay;

    Exit* Portal;

public:
    Game() : window(VideoMode(780, 676), "Plants Vs Zombies"), bMan(52, 52),
        nBricks(0), nEnemy(0)
    {
        menu_image.loadFromFile("../SFML/Images/menu.jpg");
        menu_t.loadFromImage(menu_image);
        menu_s.setTexture(menu_t);
        menu_s.setPosition(0, 0);

        map_image.loadFromFile("../SFML/Images/tileMap.png");
        t_map.loadFromImage(map_image);
        s_map.setTexture(t_map);
        s_map.setPosition(0, 0);

        cr = new CircleShape[MAX_BRICKS];
        for (int i = 0; i < MAX_BRICKS; i++)
        {
            cr[i].setRadius(10);
            cr[i].setFillColor(sf::Color::Green);
        }

        for (int i = 0; i < MAX_ENEMY; i++)
        {
            creep[i] = NULL; // Initialize the enemy pointers to nullptr
        }

        fire[0] = NULL;
        fire[1] = NULL;


        if (!buffer.loadFromFile("../SFML/Images/pop.wav")) {
            std::cout << "Error loading sound file!" << std::endl;
        }

        sound.setBuffer(buffer);
        Portal = NULL;
        Portal = NULL;
    }

    ~Game()
    {
        // Clean up allocated memory
        for (int i = 0; i < nBricks; i++)
        {
            if(bricks[i]!=NULL)
                delete bricks[i];
        }
        for (int i = 0; i < nEnemy; i++)
        {
            if(creep[i]!=   NULL)
                delete creep[i];
        }
        delete[] cr; // Clean up CircleShape array
    }



    int start()
    {
        cout << endl;
        grid.display();
        window.setPosition(Vector2i(0, 0));
        window.setFramerateLimit(60);

        mainMenue();



        brickFactory(); // Initialize bricks

        int k = 0;
        for (int j = 0; j < nBricks; j++)
        {
            if (bricks[j]->getID() == 'S')
            {
                cout << "K: " << k << endl;
                cout << "position x: " << bricks[j]->pos.x << endl;
                cout << "position y: " << bricks[j]->pos.y << endl << endl;
                k++;
            }
        }

        //Game intro
        for (int i = 0; i < nBricks; i++)
        {
            window.draw(s_map);//background
            for (int j = 0; j < nBricks; j++)
            {
                if (bricks[j]->getID() == 'S')
                {
                    bricks[j]->draw(window);
                }
            }
            for (int j = 0; j < i; j++)
            {
                if(bricks[j]->getID()!='S')
                bricks[j]->draw(window);
                
            }
            window.display();

            if (bricks[i]->getID() != 'S') {
                sound.play();
                while (delay.getElapsedTime().asMilliseconds() < 50);
                delay.restart();
            }
            window.clear();

        }

        this->enemyFactory();


        // Game loop
        while (window.isOpen())
        {


            

           cout << "Enemy!!: " << Enemy::getnEnemy() << endl;


            bricks[0]->setnBrick(nBricks);

            //creep[0]->setnEnemy(nEnemy);

            bMan.move(bricks, nBricks);
            for (int i = 0; i < nEnemy; i++)
            {
                if (creep[i] != nullptr)
                {
                    creep[i]->move(bricks, nBricks);
                }
            }

            bMan.plantBomb();

            bMan.boom(bricks, creep,fire,Portal);

            sf::Event event;
            while (window.pollEvent(event))
            {
                // Window closing conditions
                if (event.type == Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
            }

            window.clear();
            display(window);
            window.display();
        }
        return 0;
    }

    void display(RenderWindow& window)
    {
        window.draw(s_map);
        if (fire[0] != NULL && fire[1] != NULL)
        {
            fire[0]->draw(window);
            fire[1]->draw(window);

        }
        window.draw(s_map);

        // Draw all bricks
        for (int i = 0; i < nBricks; i++)
        {
            bricks[i]->draw(window);
        }

        for (int i = 0; i < nEnemy; i++)
        {
            if (creep[i] != nullptr)
            {
                creep[i]->draw(window);
            }
        }

        if(Portal!=NULL)
         Portal->draw(window);

        bMan.draw(window);

        int k = 0;
        for (int i = 0; i < grid.ROWS; i++)
        {
            for (int j = 0; j < grid.COLS; j++)
            {
                if (grid.grid[i][j] == 'S' || grid.grid[i][j] == 'W')
                {
                    cr[k].setPosition(j * 52, i * 52);
                    // window.draw(cr[k]); // Uncomment to draw the CircleShape
                    k++;
                }
            }
        }
        
    }

    void brickFactory()
    {

        
        // Create bricks 
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (grid.getValue(i, j) == 'S' && nBricks < MAX_BRICKS)
                {
                    bricks[nBricks] = new solidBrick(j, i, 'S'); // Add SolidBrick 
                    nBricks++;
                    cout << "solid: " << nBricks << endl;
                }

                if (grid.getValue(i, j) == 'W' && nBricks < MAX_BRICKS)
                {
                    bricks[nBricks] = new Explodable(j, i, 'W'); // Add Explodable brick 
                    nBricks++;
                    cout << "Explodable: " << nBricks << endl;

                }
            }
        }
       
        
        //random exit generator
        int randomNumber = rand() % 10;
        int i = nBricks - 17- randomNumber;
        while (i > 0)
        {
            if (bricks[i]->getID() != 'W')
                i--;
            else
            {
                cout << "Exit: " << i << endl;
                bricks[i]->yesExit(1);
               // bricks[i]->pos.x -= 20;
                break;
            }

        }
       
    }

    void enemyFactory()
    {
        //create enemy
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                if (grid.getValue(j, i) == 'E' && nEnemy < MAX_ENEMY)
                {
                    creep[nEnemy] = new Enemy(i * 52, j * 52);
                    nEnemy++;
                }
            }
        }
        cout << "Enemies created" << endl;
        cout << "total enemies: " << nEnemy << endl;
        Enemy::setnEnemy(nEnemy);
    }


    void mainMenue()
    {

        while(window.isOpen())
        {
            window.draw(menu_s);//background
           
          
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                break;
          }
               

                sf::Event event;
                while (window.pollEvent(event))
                {
                    // Window closing conditions
                    if (event.type == Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    {
                        window.close();
                    }
                }
           
            window.clear();

        }
            
        

    }


};
