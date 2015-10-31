#include "EventPass.hpp"

#include "Resources.hpp"

#include <math.h>

class Startup : public EventPass {

public:

	sf::View view;

	sf::Sprite sprite;

	float timer;
	float duration;

	Startup(float durationParam = 1.0f) :
		timer(0.0f),
		duration(durationParam)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, startScreenTexSize.x, startScreenTexSize.y));
		sprite.setTexture(startScreenTex);
		sprite.setOrigin(sf::Vector2f((float)startScreenTexSize.x / 2.0f, (float)startScreenTexSize.y / 2.0f));

		sprite.setScale(sf::Vector2f(1.0f / (float)playerSpriteSize.x, 1.0f / (float)playerSpriteSize.x));
		//sprite.setScale(sf::Vector2f(1, 1));

		view.setCenter(Vector2f(0.0f, 0.0f));
	}
	virtual ~Startup() {}


	virtual EventPass *eventHandle(sf::Event event) {

		switch (event.type) {
			case sf::Event::KeyPressed: {
				switch (event.key.code) {
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

		Color color = sprite.getColor();
		color.a = 0xff * fabs(sin((timer / duration) * M_PI));
		sprite.setColor(color);

		if (timer > duration) {
			timer = 0.0f;
			return new CharacterSelect();
		} else {
			return NULL;
		}
	}

	virtual void draw(RenderWindow *window) {
		window->clear(sf::Color(0,0,0));

		view.setSize(Vector2f(((float)window->getSize().x / (float)window->getSize().y), 1.0f) * 5.0f);
		window->setView(view);
		window->draw(sprite);
	}
};