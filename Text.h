#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#define Del_key 8
#define Ent_key 13
#define Esc_key 27

class Textbox {
public:
	Textbox(){}
	Textbox(int size, sf::Color color, bool sel);
	void setFont(sf::Font &font);
	void setPosition(sf::Vector2f pos);
	void setLimit(bool ToF);
	void setLimit(bool ToF, int lim);
	void setSelected(bool sel);
	std::string getText();
	void drawTo(sf::RenderWindow& window);
	void typedOn(sf::Event input);

private:
	sf::Text textbox;
	std::ostringstream text;
	bool is_Selected = true;
	bool hasLimit = false;
	int limit;
	void inputLogic(int charTyped) {
		if (charTyped != Del_key && charTyped != Ent_key && charTyped != Esc_key) {
			text << static_cast<char>(charTyped);
		}else if (charTyped == Del_key) {
			if (text.str().length() > 0) {
				deleteLastChar();
			}
		}
		textbox.setString(text.str());
	}
	void deleteLastChar() {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		text.str("");
		text << newT;

		textbox.setString(text.str());
	}
};	

