#pragma once
#include <fstream>
class map_objects
{
public:
	sf::IntRect rect;
	sf::Texture txt;
	sf::Sprite sprite;
	float scale = 1;
	float xp = 0;
	float yp = 0;
	float height = 0;
	float width = 0;
	std::string txt_path = "";
	map_objects(float xp, float yp, float heightt, float widthh, std::string txt_path, float scalee);
};
