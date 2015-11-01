#ifndef _CLUMSY_H_
#define _CLUMSY_H_

#include "Boll.hpp"
#include "../physics/Object.hpp"

class Clumsy: public Object {

public:

	sf::Sprite body, eyes, mouth;

	int body_index;
	float body_timer;

	bool yawning;
	int mouth_index;
	float mouth_timer;

	bool blinking;
	int eyes_index;
	float eyes_timer;

	float direction;

	Boll *boll;

	Clumsy(Vector2f pos, Color color, Boll *bollParam):
		Object(pos, Vector2f(0,0), 40.0f, 50.0f, 1.0f),

		body_index(0),
		body_timer(0.0f),

		yawning(true),
		mouth_index(0),
		mouth_timer(0.0f),

		blinking(true),
		eyes_index(0),
		eyes_timer(0.0f),

		direction(0.0f),

		boll(bollParam)
	{

		sf::Vector2f scale((radius * 2.0f) / (float)bodySize.x, (radius * 2.0f) / (float)bodySize.y);


		body.setTextureRect(sf::IntRect(0, 0, bodySize.x, bodySize.y));
		body.setTexture(bodyTex);
		body.setScale(scale);
		body.setOrigin(sf::Vector2f((float)bodySize.x / 2.0f, (float)bodySize.y / 2.0f));

		eyes.setTextureRect(sf::IntRect(0, 0, eyesSize.x, eyesSize.y));
		eyes.setTexture(eyesTex);
		eyes.setScale(scale);
		eyes.setOrigin(sf::Vector2f((float)eyesSize.x / 2.0f, (float)eyesSize.y / 2.0f) + Vector2f(0,50));

		mouth.setTextureRect(sf::IntRect(0, 0, mouthSize.x, mouthSize.y));
		mouth.setTexture(mouthTex);
		mouth.setScale(scale);
		mouth.setOrigin(sf::Vector2f((float)mouthSize.x / 2.0f, (float)mouthSize.y / 2.0f) + Vector2f(0,-190));


		body.setColor(color);
		mouth.setColor(Color(220, 80, 80));
	}

	virtual ~Clumsy() {}

	virtual void update(float elapsedTime) {

		body_timer += elapsedTime;

		if (body_timer > 0.2f) {
			body_timer -= 0.2f;
			body_index++;
			if (body_index >= 6) {
				body_index = 0;
			}
			body.setTextureRect(sf::IntRect(bodySize.x * body_index, 0, bodySize.x, bodySize.y));
		}


		mouth_timer += elapsedTime;
		if (mouth_timer > 0.15) {
			mouth_timer -= 0.15f;
			mouth_index++;
			if (!yawning) {
				if (mouth_index > 3) {
					if (rand() % 10 == 0) {
						yawning = true;
					} else {
						mouth_index = 0;
					}
				}

				mouth.setTextureRect(sf::IntRect(mouthSize.x * mouth_index, 0, mouthSize.x, mouthSize.y));
			} else {
				if (mouth_index > 11) {
					mouth_index = 0;

					yawning = false;
				}

				mouth.setTextureRect(sf::IntRect(mouthSize.x * (mouth_index > 7 ? 15 - mouth_index : mouth_index), 0, mouthSize.x, mouthSize.y));
			}
		}

		eyes_timer += elapsedTime;
		if (eyes_timer > 0.1) {
			eyes_timer -= 0.1f;
			eyes_index++;
			if (!blinking) {
				if (eyes_index > 2) {
					if (rand() % 20 == 0) {
						blinking = true;
					} else {
						eyes_index = 0;
					}
				}
				eyes.setTextureRect(sf::IntRect(eyesSize.x * eyes_index, 0, eyesSize.x, eyesSize.y));
			} else {
				if (eyes_index > 8) {
					eyes_index = 0;

					blinking = false;
				}
				eyes.setTextureRect(sf::IntRect(eyesSize.x * (eyes_index > 5 ? 11 - eyes_index : eyes_index), 0, eyesSize.x, eyesSize.y));
			}
		}


		if (boll->connected) {
			direction = periodValueBetween(direction, angle(pos - boll->pos) + 90.0f, size(vel) / 1500.0f);
		} else {
			direction = periodValueBetween(direction, angle(pos - boll->pos) + 90.0f, 0.008);
		}
	}

	virtual void draw(RenderWindow *window) {
		body.setRotation(direction);
		body.setPosition(pos);
		window->draw(body);
		eyes.setRotation(direction);
		eyes.setPosition(pos);
		window->draw(eyes);
		mouth.setRotation(direction);
		mouth.setPosition(pos);
		window->draw(mouth);
	}
};

#endif
