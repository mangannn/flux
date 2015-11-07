#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Resources.hpp"
#include <math.h>

class Button {

public:

	sf::RectangleShape box;
	sf::Text text;

	Button(Vector2f pos, Vector2f size, Color color, String str) {
		box.setPosition(pos - 0.5f * size);
		box.setSize(size);
		box.setFillColor(color);

		text.setFont(font);
		text.setCharacterSize(64);
		float s = fmin(size.x, size.y);
		text.setScale(0.014f * s, 0.014f * s);

		text.setString(str);
		text.setPosition(pos - 0.5f * Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height * 1.3));
	}

	bool inside(Vector2f v) {
		return box.getGlobalBounds().contains(v);
	}

	void draw(sf::RenderWindow *window) {
		window->draw(box);
		window->draw(text);
	}
};

#endif