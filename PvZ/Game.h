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
#include <fstream>
#include "Timer.h"

using namespace sf;
using namespace std;

const int MAX_BRICKS = 300; // Adjust as needed
const int MAX_ENEMY = 10;

#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

sf::Font font;


class Game
{
private:
    bool isLoad;
    bool isTimeTrial;

    sf::Image menu_image;
    sf::Texture menu_t;
    sf::Sprite menu_s;
    sf::Image gameover_image;
    sf::Image victory_image;
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

    Text livesText;
   
    
    

public:
    Game() : window(VideoMode(780, 676), "Plants Vs Zombies"), bMan(52, 52),
        nBricks(0), nEnemy(0)
    {
        isTimeTrial = false;
        isLoad = false;
        menu_image.loadFromFile("../SFML/Images/menu.jpg");
        gameover_image.loadFromFile("../SFML/Images/gameover.png");
        victory_image.loadFromFile("../SFML/Images/victory1.png");
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
            cout << "Error loading sound file!" << std::endl;
        }

        sound.setBuffer(buffer);
        Portal = NULL;
        Portal = NULL;

        if (!font.loadFromFile("../SFML/Images/bmFont.ttf")) {
            //cout << "Font not loaded" << std::endl;
        }

        livesText.setFont(font);
        livesText.setString("LIVES: ");
        livesText.setCharacterSize(20);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(600, 10);
    }

    ~Game()
    {
        // Clean up allocated memory
        for (int i = 0; i < nBricks; i++)
        {
            if (Portal != NULL && Portal->i == i)
                continue;
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



    bool start()
    {
       
      


        window.setPosition(Vector2i(0, 0));
        window.setFramerateLimit(60);

        mainMenue();

        if(!isLoad)
        {
            grid.display();
            this->enemyFactory();

            brickFactory(); // Initialize bricks

            int k = 0;
            for (int j = 0; j < nBricks; j++)
            {
                if (bricks[j]->getID() == 'S')
                {
                    //cout << "K: " << k << endl;
                    //cout << "position x: " << bricks[j]->pos.x << endl;
                    //cout << "position y: " << bricks[j]->pos.y << endl << endl;
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
                    if (bricks[j]->getID() != 'S')
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

        }

        TimeClock ttime;
        bool i = 0;
        // Game loop
        while (window.isOpen())
        {
            

            if (bMan.redClock.getElapsedTime().asSeconds() > 2)
                bMan.setRed(0);

            if (bMan.reallyRed())
            {
                //apply red tint
                bMan.sprite.setColor(sf::Color(255, 0, 0, 128));
            }
            else
            {
                //reset
                bMan.sprite.setColor(sf::Color::White);
            }

            //File handling

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                if(delay.getElapsedTime().asSeconds()>2)
                {
                    this->saveGame();
                    delay.restart ();
                }
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            {
               
            }

       

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

            
                if (bMan.enemyCollision(creep) && !bMan.reallyRed() && i)
                {
                   
                    bMan.lives--;
                    bMan.setRed(1);
                    bMan.redClock.restart();
                }
                i = true;
            
            

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
            if ((isTimeTrial && ttime.action(window)) || bMan.lives<= 0) 
            {

               return gameOverScreen();
                
            }


            //PORTAL COLLISION
            if (Portal != NULL)
            {
                FloatRect portalBounds(Portal->sprite.getGlobalBounds());
                if (portalBounds.intersects(bMan.sprite.getGlobalBounds()))
                {
                    return victoryScreen();
                }
            }
            window.clear();
            display(window);
          
            string livetxt = "LIVES " + to_string(bMan.lives);
            livesText.setString(livetxt);
            window.draw(livesText);
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
                }

                if (grid.getValue(i, j) == 'W' && nBricks < MAX_BRICKS)
                {
                    bricks[nBricks] = new Explodable(j, i, 'W'); // Add Explodable brick 
                    nBricks++;

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
        
        Enemy::setnEnemy(nEnemy);
    }


    void mainMenue()
    {
        sf::Font font;
        if (!font.loadFromFile("../SFML/Images/bmFont.ttf")) {
            //cout << "Font not loaded" << std::endl;
        }

        sf::Text text;
        text.setFont(font);
        text.setString("Start Game");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Yellow);
        text.setPosition(320 - text.findCharacterPos(3).x, 400);

        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString("Time Trial");
        timeText.setCharacterSize(40);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(320 - timeText.findCharacterPos(5).x, 440); // Adjusted position

        sf::Text loadText;
        loadText.setFont(font);
        loadText.setString("Load Game");
        loadText.setCharacterSize(40);
        loadText.setFillColor(sf::Color::White);
        loadText.setPosition(320 - loadText.findCharacterPos(3).x, 480);

        sf::Text* currentSelection = &text; // Pointer to the currently selected text

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
            }


            if (delay.getElapsedTime().asMilliseconds() > 100)  
            {
                delay.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    if (currentSelection == &text)
                    {
                        currentSelection = &timeText;
                        text.setFillColor(sf::Color::White);
                        timeText.setFillColor(sf::Color::Yellow);
                    }
                    else if (currentSelection == &timeText)
                    {
                        currentSelection = &loadText;
                        timeText.setFillColor(sf::Color::White);
                        loadText.setFillColor(sf::Color::Yellow);
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    if (currentSelection == &loadText)
                    {
                        currentSelection = &timeText;
                        loadText.setFillColor(sf::Color::White);
                        timeText.setFillColor(sf::Color::Yellow);
                    }
                    else if (currentSelection == &timeText)
                    {
                        currentSelection = &text;
                        timeText.setFillColor(sf::Color::White);
                        text.setFillColor(sf::Color::Yellow);
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (currentSelection == &text)
                {
                    isTimeTrial = 0;
                    break;
                }
                else if (currentSelection == &timeText)
                {
                    isTimeTrial = 1;
                    break;
                }
                else if (currentSelection == &loadText)
                {
                    
                    this->loadGame();
                    break;
                }
            }

            window.clear(); // Clear the previous frame
            window.draw(menu_s); // background
            window.draw(text);
            window.draw(timeText);
            window.draw(loadText);
            window.display();
        }
    }


    void saveGame()
    {
        //cout << "game saved" << endl;

        ofstream file("gameData.txt");

        file  <<"herox " << bMan.pos.x<<endl;
        file << "heroy " << bMan.pos.y<<endl;


        file << "totalBricks: " << Brick::nBrick << endl<<endl;
        for (int i = 0; i < Brick::nBrick; i++)
        {
            file << "brickid: " << bricks[i]->id << endl;
            file << "brickx: " << bricks[i]->pos.x << endl;
            file << "bricky: " << bricks[i]->pos.y << endl;
            file << "isExit: " << bricks[i]->reallyExit() <<endl<< endl;
        }


        file << "totalEnemy: " << Enemy::nEnemey << endl << endl;
        for (int i = 0; i < Enemy::nEnemey; i++)
        {
            file << "enemyx: " << creep[i]->pos.x << endl;
            file << "enemyy: " << creep[i]->pos.y <<endl<< endl;
        }

        file.close();

    }

   
    void loadGame()
    {
        isLoad = 1;
        int lineNumber = 1;
        ifstream file("gameData.txt");

        if (!file.is_open())
        {
            cout << "Failed to open gameData.txt for loading." << endl;
            return;
        }

        string line;
        int totalBricks, totalEnemies;

        // Read hero position
        getline(file, line);
        lineNumber++;
        int x=bMan.pos.x = stoi(line.substr(line.find('x') + 2));
        getline(file, line);
        lineNumber++;
        int y = bMan.pos.y = stoi(line.substr(line.find('y') + 2));

        // Read total bricks
        getline(file, line);
        lineNumber++;
        totalBricks = stoi(line.substr(line.find(':') + 2));

       
      
        
        
        for (int i = 0; i < totalBricks; i++)
        {
            getline(file, line);
            lineNumber++;
            getline(file, line); // brickid
            lineNumber++;
            string idd = (line.substr(line.find(':') + 2));
            char id = idd[0];
            getline(file, line); // brickx
            lineNumber++;
            int x = stoi(line.substr(line.find('x') + 2));
            getline(file, line); // bricky
            lineNumber++;
            int y = stoi(line.substr(line.find('y') + 2));
            getline(file, line); // isExit
            lineNumber++;
            bool isExit = (line.substr(line.find(':') + 2) == "1");

           

            if (id == 'S' && nBricks < MAX_BRICKS)
            {
                bricks[nBricks] = new solidBrick(x/52, y/52, 'S'); // Add SolidBrick 
                nBricks++;
              
            }

            if (id == 'W' && nBricks < MAX_BRICKS)
            {
                bricks[nBricks] = new Explodable(x/52, y/52, 'W'); // Add Explodable brick 
                nBricks++;
              

            }
        }

      
        // Read total enemies
        getline(file, line); // Skip "totalEnemy:"
        lineNumber++;
        getline(file, line);
        lineNumber++;
        totalEnemies = stoi(line.substr(line.find(':') + 2));

        //cout << "Total Enemies: " << totalEnemies << endl;
        //cout << nEnemy << endl;
        getline(file, line);
        lineNumber++;
       
        //cout << "line: " << lineNumber << " " << line << endl;

        while(nEnemy<totalEnemies)
        {
            getline(file, line); // enemyx
            lineNumber++;

            //cout << "line: " << lineNumber << " " << line << endl;
            int x = stoi(line.substr(line.find("x:") + 2));

            getline(file, line); // enemyy
            lineNumber++;
            int y = stoi(line.substr(line.find("y:") + 2));

            if (nEnemy < MAX_ENEMY) 
            {
                creep[nEnemy] = new Enemy(x , y ); 
                nEnemy++;
            }

            //cout << "Enemy X: " << x << endl;
            //cout << "Enemy Y: " << y << endl<<endl;
            getline(file, line); // enemyx
            lineNumber++;
        }
        file.close();
    }


    bool gameOverScreen()
    {
        menu_t.loadFromImage(gameover_image);
        menu_s.setTexture(menu_t);
        sf::Font font;
        if (!font.loadFromFile("../SFML/Images/bmFont.ttf")) {
            cout << "Font not loaded" << std::endl;
        }

        sf::Text text;
        text.setFont(font);
        text.setString("Main Menu");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Yellow);
        text.setPosition(320 - text.getLocalBounds().width / 2, 400);

        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString("Exit");
        timeText.setCharacterSize(40);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(320 - timeText.getLocalBounds().width / 2, 440);

        
        sf::Text* currentSelection = &text; // Pointer to the currently selected text

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
            }

            if (delay.getElapsedTime().asMilliseconds() > 100)
            {
                delay.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    if (currentSelection == &text)
                    {
                        currentSelection = &timeText;
                        text.setFillColor(sf::Color::White);
                        timeText.setFillColor(sf::Color::Yellow);
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    if (currentSelection == &timeText)
                    {
                        currentSelection = &text;
                        timeText.setFillColor(sf::Color::White);
                        text.setFillColor(sf::Color::Yellow);
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (currentSelection == &text)
                {
                    return 1;
                }
                else if (currentSelection == &timeText)
                {
                    return 0;
                }
            }


            window.clear(); // Clear the previous frame
            window.draw(menu_s); // Draw background
            window.draw(text);
            window.draw(timeText);
            window.display();
        }
    }

    bool victoryScreen()
    {
        menu_t.loadFromImage(victory_image);
        menu_s.setTexture(menu_t);
        sf::Font font;
        if (!font.loadFromFile("../SFML/Images/bmFont.ttf")) {
            cout << "Font not loaded" << std::endl;
        }

        sf::Text text;
        text.setFont(font);
        text.setString("Main Menu");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::Yellow);
        text.setPosition(320 - text.getLocalBounds().width / 2, 400);

        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString("Exit");
        timeText.setCharacterSize(40);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(320 - timeText.getLocalBounds().width / 2, 440);


        sf::Text* currentSelection = &text; // Pointer to the currently selected text

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
            }

            if (delay.getElapsedTime().asMilliseconds() > 100)
            {
                delay.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    if (currentSelection == &text)
                    {
                        currentSelection = &timeText;
                        text.setFillColor(sf::Color::White);
                        timeText.setFillColor(sf::Color::Yellow);
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    if (currentSelection == &timeText)
                    {
                        currentSelection = &text;
                        timeText.setFillColor(sf::Color::White);
                        text.setFillColor(sf::Color::Yellow);
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (currentSelection == &text)
                {
                    return 1;
                }
                else if (currentSelection == &timeText)
                {
                    return 0;
                }
            }


            window.clear(); // Clear the previous frame
            window.draw(menu_s); // Draw background
            window.draw(text);
            window.draw(timeText);
            window.display();
        }
    }

};
