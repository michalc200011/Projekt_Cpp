#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdlib.h> 
#include <time.h>
#include <fstream>
#include "map_objects.h"
#include "Text.h"
#include <fstream>
//kolizje//
bool collisionP(sf::Sprite& a, sf::Sprite& b,float a_width,float a_height,float b_width,float b_height,float a_scale,float b_scale)
{
	float aw = a_width * a_scale, ah = a_height * a_scale, bw = b_width * b_scale, bh = b_height * b_scale*0.2;
	if (a.getPosition().x + aw  > b.getPosition().x && a.getPosition().y+ah > b.getPosition().y && a.getPosition().y < b.getPosition().y+bh )return true;
	return false;
}
bool collisionL(sf::Sprite& a, sf::Sprite& b, float a_width, float a_height, float b_width, float b_height, float a_scale, float b_scale)
{
	float aw = a_width * a_scale, ah = (a_height+300) * a_scale, bw = b_width * b_scale, bh = b_height * b_scale*0.5;
	if (a.getPosition().x < b.getPosition().x+bw && a.getPosition().y + ah > b.getPosition().y && a.getPosition().y < b.getPosition().y + bh)return true;
	return false;
}
int main()
{
	//wymiary okna
	const int w = 400, h = 670;
	//Dane do animacji
	const int gravd = 4,gravu=4, jumpheight = 50;
	bool Jump = false, noJump = true, Left = false,firstgen=true,start=false,lose=false,entered=true,first=true,scoresave=false;
	float jump_lvl, xVel = 1; 
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
	const float birdscale = 0.08;
	sf::Texture bird_txtl;
	bird_txtl.loadFromFile("Textures/Bird/Bird_left.png");
	sf::Texture bird_txtr;
	bird_txtr.loadFromFile("Textures/Bird/Bird_right.png");
	sf::IntRect bird_rect(0, 0, birdw, birdh);
	sf::Sprite bird(bird_txtr, bird_rect);
	bird.scale(birdscale,birdscale);
	bird.setPosition(w/3, h/3);
	int lefts[2] = { 0,2169 };
	//klolce
	const int spikew = 855, spikeh = 621;
	const float spikescale = 0.05;
	const int nspikes = int(h/(spikeh*spikescale));
	int spikes_places = int(nspikes*0.3);
	std::vector <std::unique_ptr<map_objects>>Lspikes;
	std::vector <std::unique_ptr<map_objects>>Rspikes;
	std::vector <int> place_numbers;
	for (int i = 0; i < nspikes; i++)
	{
		Lspikes.push_back(std::unique_ptr<map_objects>(new map_objects(3, 111, spikeh, spikew, "Textures/Lspike.png", spikescale)));
		(*Lspikes[i]).sprite.setPosition(0,0+i*621*0.05);
		Rspikes.push_back(std::unique_ptr<map_objects>(new map_objects(3, 111, spikeh, spikew, "Textures/Rspike.png", spikescale)));
		(*Rspikes[i]).sprite.setPosition(w-(spikew*spikescale), 0 + i * 621 * 0.05);
		//Rspikes.push_back(std::unique_ptr<map_objects>(new map_objects(16, 21, 22, 31, "Textures/items/Rspike.png", 1.5)));
	}
	//cukierek
	const int candyw=194,candyh=194;
	const float candyscale =0.25 ;
	std::vector <std::unique_ptr<map_objects>>candy;
	//'ala' menu
	const int menuw = 837, menuh = 84;
	const float menuscale = 0.4;
	sf::Texture menutxt;
	menutxt.loadFromFile("Textures/Start.png");
	sf::IntRect menurect(87, 58, menuw, menuh);
	sf::Sprite menu(menutxt, menurect);
	menu.scale(menuscale, 1);
	menu.setPosition(w / 2 - (menuw * menuscale)/2, 0);
	//tekst
	sf::Font font;
	font.loadFromFile("Font/Raleway-SemiBold.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color(0,0,0));
	sf::Text textsc;
	textsc.setFont(font);
	textsc.setCharacterSize(26);
	textsc.setFillColor(sf::Color(0, 0, 0));
	//D¿wiêki
	sf::Music coinsound;
	coinsound.openFromFile("Sounds/coin.wav");
	sf::Music jumpsound;
	jumpsound.openFromFile("Sounds/jump.wav");
	sf::Music death;
	death.openFromFile("Sounds/death.wav");
	sf::Music theme;
	theme.openFromFile("Sounds/theme.wav");
	theme.setLoop(true);
	/////////////////////////////////////////GRA///////////////////////////
	int score = 0;
	//60FPS
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	//zegar do animacji
	sf::Clock clock;
	//textbox
	Textbox textbox1(28, sf::Color(0, 0, 0), true);
	textbox1.setFont(font);
	//scoreboard
	std::fstream plik;
	//gra
	theme.play();
	while (window.isOpen()) 
	{
		window.setFramerateLimit(60);
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space ) && !scoresave)start = true;
		}
		window.draw(mapbackground);
		if (!start && !scoresave) {
			window.draw(menu); 
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))start = true;
			plik.open("Score/score.txt", std::ios::in);
			std::string line;
			std::string srcscore="Last 10 scores:\n\n";
			std::vector<std::string> lines;
			int k = 0;
			while (std::getline(plik,line))
			{
				lines.push_back(line);
				k++;
			}
			for (int i = k-1; i >= 0 && i > k - 11; i--)
			{
				srcscore += lines[i] + "\n";
			}
			textsc.setPosition(w / 2 - textsc.getGlobalBounds().width / 2, 100);
			textsc.setString(srcscore);
			window.draw(textsc);
			plik.close();
		}
		if (start)
		{	
			first = false;
			
			if (lose && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
			{
				lose = false;
				start = false;
				score = 0;
				bird.setPosition(w / 3, h / 3);
			}
			if (!lose)
			{
				
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
						jumpsound.stop();
						jumpsound.play();
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
					bird.setPosition(bird.getPosition().x, bird.getPosition().y - gravu);
				}
				else
				{
					bird.setPosition(bird.getPosition().x, bird.getPosition().y + gravd);
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
				//kolizje z œcianami
				if (bird.getPosition().x >= w - birdw * birdscale)
				{
					xVel = xVel * -1;
					Left = true;
					candy.erase(candy.begin(), candy.begin() + candy.size());
					candy.push_back(std::unique_ptr<map_objects>(new map_objects(31, 31, candyh, candyh, "Textures/seed.png", candyscale)));
					(*candy[0]).sprite.setPosition(rand() % 201 + 100, rand() % 402 + 134);
				}
				if (bird.getPosition().x < 0)
				{
					xVel = xVel * -1;
					Left = false;
					candy.erase(candy.begin(), candy.begin() + candy.size());
					candy.push_back(std::unique_ptr<map_objects>(new map_objects(31, 31, candyh, candyh, "Textures/seed.png", candyscale)));
					(*candy[0]).sprite.setPosition(rand() % 201 + 100, rand() % 402 + 134);
				}
				//
				sf::IntRect result;
				if (candy.size() != 0 && bird.getGlobalBounds().intersects((*candy[0]).sprite.getGlobalBounds()))
				{
					coinsound.stop();
					candy.erase(candy.begin(), candy.begin() + candy.size());
					score++;
					coinsound.play();
				}//////////////////////////////////////////////////////////////////
				text.setString("Your score:" + std::to_string(score)+"\nType your name \nand press enter to save");
				text.setPosition(w/2 - (text.getGlobalBounds().width / 2), 0);
				//generacja kolców
				switch (score)
				{
				case 5:
					spikes_places = (nspikes - 4) * 0.35;
				case 15:
					spikes_places = (nspikes - 4) * 0.40;
				case 20:
					spikes_places = (nspikes - 4) * 0.45;
				case 35:
					spikes_places = (nspikes - 4) * 0.50;
				case 45:
					spikes_places = (nspikes - 4) * 0.55;
				case 55:
					spikes_places = (nspikes - 4) * 0.60;
				default:
					break;
				}
				int number;
				int range[5];
				bool simmilar = false;
				if (firstgen)
				{
					place_numbers.erase(place_numbers.begin(), place_numbers.begin() + place_numbers.size());
					while (place_numbers.size() < spikes_places)
					{
						number = rand() % 21;
						if (place_numbers.size() == 0)
						{
							place_numbers.push_back(number);
							if (number + 6 < 21)
							{
								place_numbers.push_back(number + 6);
								range[0] = number + 1;
								range[1] = number + 2;
								range[2] = number + 3;
								range[3] = number + 4;
								range[4] = number + 5;
							}
							else if (number - 6 >= 0)
							{
								place_numbers.push_back(number - 6);
								range[0] = number - 1;
								range[1] = number - 2;
								range[2] = number - 3;
								range[3] = number - 4;
								range[4] = number - 5;
							}
						}
						else
						{
							for (int i = 0; i < place_numbers.size(); i++)
							{
								if (number == place_numbers[i] || number == range[0] || number == range[1] || number == range[2] || number == range[3] || number == range[4]) { simmilar = true; }
							}
							if (!simmilar) { place_numbers.push_back(number); }
							simmilar = false;
						}
					}
					firstgen = false;
				}
				if (bird.getPosition().x < 0 || bird.getPosition().x >= w - birdw * birdscale)
				{
					place_numbers.erase(place_numbers.begin(), place_numbers.begin() + place_numbers.size());
					while (place_numbers.size() < spikes_places)
					{
						number = rand() % 21;
						if (place_numbers.size() == 0)
						{
							place_numbers.push_back(number);
							if (number + 6 < 21)
							{
								place_numbers.push_back(number + 6);
								range[0] = number + 1;
								range[1] = number + 2;
								range[2] = number + 3;
								range[3] = number + 4;
								range[4] = number + 5;
							}
							else if (number - 6 >= 0)
							{
								place_numbers.push_back(number - 6);
								range[0] = number - 1;
								range[1] = number - 2;
								range[2] = number - 3;
								range[3] = number - 4;
								range[4] = number - 5;
							}
						}
						else
						{
							for (int i = 0; i < place_numbers.size(); i++)
							{
								if (number == place_numbers[i] || number == range[0] || number == range[1] || number == range[2] || number == range[3] || number == range[4]) { simmilar = true; }
							}
							if (!simmilar) { place_numbers.push_back(number); }
							simmilar = false;
						}

					}
				}
				//Przesuniecie x
				bird.setPosition(bird.getPosition().x + xVel, bird.getPosition().y);
				for (int i = 0; i < place_numbers.size(); i++)
				{
					if (!Left && collisionP(bird, (*Rspikes[place_numbers[i]]).sprite, birdw, birdh, spikew, spikeh, birdscale, spikescale)) {lose = true; death.stop(); death.play();
					}
					if (Left && collisionL(bird, (*Lspikes[place_numbers[i]]).sprite, birdw, birdh, spikew, spikeh, birdscale, spikescale)) {lose=true; death.stop(); death.play();
					}
				}
				if (bird.getPosition().y < 0) { lose = true; death.stop();death.play();}
				if (bird.getPosition().y + birdw * birdscale > h) { lose = true; death.stop();death.play();}

				window.draw(bird);
				for (int i = 0; i < place_numbers.size(); i++)
				{
					if (Left)window.draw((*Lspikes[place_numbers[i]]).sprite);
					if (!Left)window.draw((*Rspikes[place_numbers[i]]).sprite);
				}
				if (candy.size() != 0)
				{

					window.draw((*candy[0]).sprite);
				}


			}
			{	
			
			}

			
		}
		if (lose) {
			scoresave = true;
			start = false;
			window.setFramerateLimit(10);
			window.draw(text);
			textbox1.setLimit(true, 10);
			if (event.type == sf::Event::TextEntered) {
				textbox1.typedOn(event);
			}
			textbox1.setPosition({ w / 4, 120 });
			textbox1.drawTo(window);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				plik.open("Score/score.txt", std::ios::out | std::ios::app);
				plik << textbox1.getText() << ":" << score << "\n";
				plik.close();
				scoresave = false;
				lose = false;
				bird.setPosition(w / 3, h / 3);
				firstgen = true;
			}

		}
		
		window.display();
		window.clear();
	}

}