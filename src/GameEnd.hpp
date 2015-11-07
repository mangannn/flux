#include "EventPass.hpp"

#include <math.h>

class GameEnd : public EventPass {

public:
	
	float timer;
	float duration;

	sf::Color color;

	GameEnd(float durationParam = 1.0f) :
		timer(0.0f),
		duration(durationParam),

		color(0xff, 0, 0, 0)
	{}
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

		window->draw(box);


		//window->clear(color);
	}
};