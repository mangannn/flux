#ifndef _BOll_H_
#define _BOll_H_

#include "../physics/Object.hpp"
#include "../physics/Constraint.hpp"

class Boll: public Object {

public:

	sf::Sprite body, eyes, mouth;

	int body_index;
	float body_timer;

	bool blinking;
	int eyes_index;
	float eyes_timer;

	float direction;

	bool connected;

	Constraint *constraint;

	Clumsy *clumsy;

	Boll(Vector2f pos, Color color, Clumsy *clumsyParam):
		Object(pos, Vector2f(0,0), 5.0f, 10.0f, 4.0f),

		body_index(0),
		body_timer(0.0f),

		blinking(true),
		eyes_index(0),
		eyes_timer(0.0f),

		direction(0.0f),

		connected(false),

		constraint(NULL),

		clumsy(clumsyParam)
	{

		clumsy->boll = this;


		sf::Vector2f scale((radius * 2.0f) / (float)bollSize.x, (radius * 2.0f) / (float)bollSize.y);
		sf::Vector2f origin((float)bollSize.x / 2.0f, (float)bollSize.y / 2.0f);


		body.setTextureRect(sf::IntRect(0, bollSize.y * 4, bollSize.x, bollSize.y));
		body.setTexture(bollTex);
		body.setScale(scale);
		body.setOrigin(origin);

		eyes.setTextureRect(sf::IntRect(0, bollSize.y * 5, bollSize.x, bollSize.y));
		eyes.setTexture(bollTex);
		eyes.setScale(scale * 0.9f);
		eyes.setOrigin(origin);

		mouth.setTextureRect(sf::IntRect(bollSize.x * 5, 0, bollSize.x, bollSize.y));
		mouth.setTexture(bollTex);
		mouth.setScale(scale * 0.9f);
		mouth.setOrigin(origin);


		body.setColor(color);
	}

	virtual ~Boll() {}

	virtual void update(float elapsedTime) {

		body_timer += elapsedTime;

		if (body_timer > 0.2f) {
			body_timer -= 0.2f;
			body_index++;
			if (body_index >= 6) {
				body_index = 0;
			}
			body.setTextureRect(sf::IntRect(bollSize.x * body_index, bollSize.y * 4, bollSize.x, bollSize.y));
		}

		eyes_timer += elapsedTime;
		if (eyes_timer > 0.1) {
			eyes_timer -= 0.1f;
			eyes_index++;
			if (!blinking) {
				if (eyes_index > 2) {
					if (rand() % 5 == 0) {
						blinking = true;
					} else {
						eyes_index = 0;
					}
				}
				eyes.setTextureRect(sf::IntRect(bollSize.x * eyes_index, bollSize.y * 5, bollSize.x, bollSize.y));
			} else {
				if (eyes_index > 8) {
					eyes_index = 0;

					blinking = false;
				}
				eyes.setTextureRect(sf::IntRect(bollSize.x * (eyes_index > 5 ? 11 - eyes_index : eyes_index), bollSize.y * 5, bollSize.x, bollSize.y));
			}
		}
	}

	virtual void draw(RenderWindow *window) {

		body.setPosition(pos);
		body.setRotation(direction);
		eyes.setPosition(pos);
		eyes.setRotation(direction);
		mouth.setPosition(pos);
		mouth.setRotation(direction);

		window->draw(body);
		window->draw(eyes);

		if (connected) {
			window->draw(mouth);
		}
	}
};

#endif
