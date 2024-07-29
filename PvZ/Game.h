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
#include <ctime>

using namespace sf;
using namespace std;

const int MAX_BRICKS = 200; // Adjust as needed
const int MAX_ENEMY = 10;
class Game
{
private:
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
    sf::CircleShape* cr;
   

public:
    Game() : window(VideoMode(780, 676), "Plants Vs Zombies"), bMan(52, 52),
        nBricks(0)
    {
        nEnemy = 10;
        //creep = new Enemy[nEnemy];

        map_image.loadFromFile("../SFML/Images/tileMap.png");
        t_map.loadFromImage(map_image);
        s_map.setTexture(t_map);
        s_map.setPosition(0, 0);

        cr = new CircleShape[MAX_BRICKS];


       for (int i = 0; i < MAX_BRICKS; i++)
       {
           cr[i].setRadius(10);
           cr[i].setFillColor((sf::Color::Green));
       }
    }

    ~Game()
    {
        // Clean up allocated memory
        for (int i = 0; i < nBricks; i++)
        {
            delete bricks[i];
        }
    }
    void enemyFactory();

    int start()
    {
        cout << endl;
        grid.display();
        window.setPosition(Vector2i(0, 0));
        window.setFramerateLimit(60);
        brickFactory(); // Initialize bricks
        this->enemyFactory();
      

        // Game icon (optional)
        /*Image icon;
        if (!icon.loadFromFile("../SFML/Images/icon.png"))
        {
            return 1;
        }
        window.setIcon(32, 32, icon.getPixelsPtr());*/

        // Game loop
      
        while (window.isOpen())
        {
            bricks[0]->setnBrick(nBricks);
            bMan.move(bricks,nBricks);
            for(int i=0; i<nEnemy; i++)
                creep[i]->move(bricks, nBricks);

            bMan.plantBomb();
            bMan.boom(bricks,creep);

            sf::Event event;
            while (window.pollEvent(event))
            {
                // Window closing conditions
                if (event.type == Event::
                    Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
            }
            window.clear();
            display(window);
            window.display();
        }
        return 0;
    }
    int j = 0;
    void display(RenderWindow& window)
    {
        window.draw(s_map);

        // Draw all bricks

        for (int i = 0; i < nBricks; i++)
        {
            bricks[i]->draw(window);
            j++;
            
        }

        for (int i = 0; i < nEnemy; i++)
        {
           creep[i]->draw(window);
            j++;

        }
        
        bMan.draw(window);
      
        int k = 0;
        for (int i = 0; i < grid.ROWS; i++)
        {
            for (int j = 0; j < grid.COLS; j++)
            {
                if (grid.grid[i][j] == 'S' || grid.grid[i][j] == 'W')
                {
                    cr[k].setPosition(j*52, i*52);
                  //  window.draw(cr[k]);
                }

            }
        }
    }

    void brickFactory()
    {
        // Create bricks 
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                if (grid.getValue(i, j) == 'S' && nBricks < MAX_BRICKS)
                {
                    bricks[nBricks] = new solidBrick(j, i,'S'); // Add SolidBrick 
                    nBricks++;
                }
                if (grid.getValue(i, j) == 'W' && nBricks < MAX_BRICKS)
                {
                    bricks[nBricks] = new Explodable(j,i,'W'); // Add SolidBrick 
                    nBricks++;
                }
                
            }
        }
        cout << "Bricks created" << endl;
        cout << "total bricks: " << nBricks << endl;
    }


};
    void Game::enemyFactory()
    {
        // Create bricks 
        int k = 0;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                if (grid.getValue(i, j) == 'E' && k < nEnemy)
                {
                    creep[k] = new Enemy(i * 52, j * 52);
                  
                    k++;
                }

            }
        }
        creep[0]->setnEnemy(nEnemy);
        cout << "Bricks created" << endl;
        cout << "total bricks: " << nEnemy << endl;

    }
