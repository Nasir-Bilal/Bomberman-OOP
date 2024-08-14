#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class TimeClock {
public:
	Clock MainClock;
	Texture frameTexture;
	Sprite frame;
	RectangleShape rec;
	float start_x = 10, start_y = 5;

public:
	TimeClock() {
		if (!frameTexture.loadFromFile("../SFML/Images/timeStone.png"))
			cout << "not loaded" << endl;
		frame.setTexture(frameTexture);
		frame.setPosition(start_x, start_y);
		rec.setFillColor(Color::Black);
	}
	int action(RenderWindow& window) {
		
		if (MainClock.getElapsedTime().asMilliseconds() >= 60000) return 1;

		if (rec.getSize().x < 180) {
			float rect_width = (180 * MainClock.getElapsedTime().asMilliseconds()) / 10000;
			rec.setSize(Vector2f(rect_width, 40));
			rec.setPosition(start_x + 180 - rect_width, start_y);
		}
		else {
			return 1;
		}
		
		
		window.draw(rec); 
		window.draw(frame);
		return 0;
	}
};