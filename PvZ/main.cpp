	#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "gameGrid.h"
//#include "coordinates.h"

#include <ctime>
using namespace sf;
using namespace std;


//const coordinats topLeftPlayingArea(280, 130);

int main()
{
	bool flag;
	do 
	{
		srand(time(0));
		Game myGame;
		flag = myGame.start();
	} while (flag);

	return 0;
}