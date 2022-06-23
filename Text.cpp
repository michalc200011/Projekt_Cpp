#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Text.h"
Textbox::Textbox(int size, sf::Color color, bool sel) {
	textbox.setCharacterSize(size);
	textbox.setFillColor(color);
	if (sel) {
		textbox.setString("_");
	}
	else {
		textbox.setString("");
	}
}
void Textbox::setFont(sf::Font &font) {
	textbox.setFont(font);
}
void Textbox::setPosition(sf::Vector2f pos) {
	textbox.setPosition(pos);
}
void Textbox::setLimit(bool ToF) { 
	hasLimit=ToF; }
void Textbox::setLimit(bool ToF, int lim) {
	hasLimit = ToF;
	limit = lim;
}
void Textbox::setSelected(bool sel) {
	is_Selected = sel;
	if (!sel) {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		textbox.setString(newT);
	}
}
std::string Textbox::getText() {
	return text.str();
}
void Textbox::drawTo(sf::RenderWindow &window) {
	window.draw(textbox);
}
void Textbox::typedOn(sf::Event input) {
	if (is_Selected) {
		int charTyped = input.text.unicode;
		if (charTyped < 128) {
			if (hasLimit) {
				if (text.str().length() <= limit) {
					inputLogic(charTyped);
				}
				else if (text.str().length() > limit && charTyped == Del_key) {
					deleteLastChar();
				}
			}else inputLogic(charTyped);
		}
	}
}