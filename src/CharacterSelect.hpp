#include "EventPass.hpp"

class CharacterSelect : public EventPass {

public:

	sf::View view;

	Color color;

	float timer;

	CharacterSelect() :
		color(0,0,0),
		timer(0.0f)
	{

	}
	virtual ~CharacterSelect() {}


	virtual EventPass *event_handle(sf::Event event) {

		switch (event.type) {
			case sf::Event::KeyPressed: {
				switch (event.key.code) {
					case sf::Keyboard::Return: {
						return new Game();
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

		float a = 0.5f;

		color.r = 128 - (int)(120.0f * cos(timer * a * 3.0f));
		color.g = 128 - (int)(120.0f * cos(timer * a * 5.0f));
		color.b = 128 - (int)(120.0f * cos(timer * a * 7.0f));

		return NULL;
	}

	virtual void draw(RenderWindow *window) {

		//view.setSize(Vector2f(aspect, 1.0f));
		//view.setCenter(view.getSize() / 2.0f);

		window->clear(color);
	}
};