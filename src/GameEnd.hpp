#include "EventPass.hpp"
#include "Resources.hpp"

#include <math.h>

class GameEnd : public EventPass {

public:
	
	float timer;
	float duration;

	sf::Text text;
	sf::Color boxColor;

	sf::Sprite characterSprite;

	GameEnd(sf::Color colorParam, float durationParam = 1.0f) :
		timer(0.0f),
		duration(durationParam),

		boxColor(0xff, 0, 0, 0)
	{

		text.setFont(font);
		text.setCharacterSize(64);
		text.setScale(0.002f, 0.002f);


		float radius = 0.2f;

		characterSprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		characterSprite.setTexture(standingTex);
		characterSprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f));

		characterSprite.setScale(sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x));

		characterSprite.setColor(colorParam);

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

		boxColor.a = 0xff * y;
		boxColor.r = 200 * (1 - y);

		if (timer > duration) {
			return new CharacterSelect();
		} else {
			return NULL;
		}
	}

	virtual void draw(RenderWindow *window) {

		sf::RectangleShape box(window->getView().getSize());
		box.setFillColor(boxColor);

		text.setString("Player");
		text.setPosition((window->getView().getSize() - Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height)) * 0.5f + Vector2f(0,-0.4f));
		window->draw(text);

		text.setString("wins!");
		text.setPosition((window->getView().getSize() - Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height)) * 0.5f + Vector2f(0,0.2f));
		window->draw(text);

		characterSprite.setPosition((window->getView().getSize()) * 0.5f);
		window->draw(characterSprite);

		window->draw(box);
	}
};