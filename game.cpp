#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
int main()
{
	//wymiary okna
	const int w = 400, h = 670;
	//Dane do animacji
	const int grav = 4, jumpheight = 100;
	bool Jump=false;
	bool noJump = true;
	bool Left = false;
	float jump_lvl;
	int xVel = 1;
	//okno gry
	sf::RenderWindow window(sf::VideoMode(w, h), "Ptaszor");
	//t³o
	sf::Texture map;
	map.loadFromFile("Textures/background.png");
	sf::IntRect rect(0, 0, 218, 365);
	sf::Sprite mapbackground(map, rect);
	mapbackground.scale(w / 218.0, h / 365.0);
	//ptaszor
	const int birdw = 712, birdh = 606;
	const float scale = 0.1;
	sf::Texture bird_txtl;
	bird_txtl.loadFromFile("Textures/Bird/Bird_left.png");
	sf::Texture bird_txtr;
	bird_txtr.loadFromFile("Textures/Bird/Bird_right.png");
	sf::IntRect bird_rect(0, 0, birdw, birdh);
	sf::Sprite bird(bird_txtr, bird_rect);
	bird.scale(scale,scale);
	bird.setPosition(0, 0);
	int lefts[2] = { 0,2169 };
	//60FPS
	window.setFramerateLimit(60);
	//zegar do animacji
	sf::Clock clock;
	//gra
	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			jump_lvl = bird.getPosition().y - jumpheight;
			if (noJump && !Jump)//okreslenie stanu skoku
			{
				Jump = true;
				noJump = false;
			}
		}
		//skok
		if (Jump)
		{
			//animacja skrzydel w góre

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (!Left)
				{
					bird_rect.left = lefts[1];
					bird.setTexture(bird_txtr);
					bird.setTextureRect(bird_rect);
				}
				else
				{
					bird_rect.left = lefts[1];
					bird.setTexture(bird_txtl);
					bird.setTextureRect(bird_rect);
				}
			}
			bird.setPosition(bird.getPosition().x, bird.getPosition().y - grav);
		}
		else
		{
			bird.setPosition(bird.getPosition().x, bird.getPosition().y + grav);
		}
		//animacja skrzyde³ w dó³
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!Left) 
			{
				bird_rect.left = lefts[0];
				bird.setTexture(bird_txtr);
				bird.setTextureRect(bird_rect);
			}
			else
			{
				bird_rect.left = lefts[0];
				bird_rect.height += 300;
				bird.setTexture(bird_txtl);
				bird.setTextureRect(bird_rect);
			}
		}
		//wysokoœæ graniczna skoku
		if (bird.getPosition().y <= jump_lvl)
		{
			noJump = true;
			Jump = false;
		}
		//Przesuniecie x
		
		if (bird.getPosition().x >= w - birdw * scale)
		{
			xVel = xVel * -1;
			Left = true;
		}
		if (bird.getPosition().x < 0) 
		{ 
			xVel = xVel * -1;
			Left = false;
		}
		bird.setPosition(bird.getPosition().x + xVel, bird.getPosition().y);
		window.clear();
		window.draw(mapbackground);
		window.draw(bird);
		window.display();

	}

}
