#include "EventPass.hpp"

#include <math.h>

class GameEnd : public EventPass {

public:
	
	float timer;
	float duration;

	sf::Text text;
	sf::Color color;

	GameEnd(sf::String message, float durationParam = 1.0f) :
		timer(0.0f),
		duration(durationParam),

		color(0xff, 0, 0, 0)
	{

		text.setFont(font);
		text.setCharacterSize(40);
		text.setScale(Vector2f(0.003,0.003));

		text.setString(message);
	}
	virtual ~GameEnd() {}


	virtual EventPass *eventHandle(sf::Event event) {

		switch (event.type) {
			case sf::Event::KeyPressed: {
				switch (event.key.code) {
					case sf::Keyboard::Space:
					case sf::Keyboard::Return: {
						return new CharacterSelect();
					} break;
					default: break;
				}
			} break;
			default: break;
		}

		return NULL;
	}

	virtual EventPass *update(float elapsedTime) {

		timer += elapsedTime;

		float x = (timer / duration);
		float y = (-2.0 * (x * x * x)) + (3.0 * (x * x));

		color.a = 0xff * y;
		color.r = 200 * (1 - y);

		if (timer > duration) {
			return new CharacterSelect();
		} else {
			return NULL;
		}
	}

	virtual void draw(RenderWindow *window) {

		sf::RectangleShape box(window->getView().getSize());
		box.setFillColor(color);

		text.setPosition((window->getView().getSize() - Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height)) * 0.5f);

		window->draw(text);
		window->draw(box);
	}
};