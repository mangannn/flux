#ifndef _CLUMSY_H_
#define _CLUMSY_H_

#include "Object.hpp"

class Clumsy: public Object {

public:

	sf::Sprite body, eyes, mouth;

	int body_pos;
	float body_mouth_timer;

	bool yawning;
	int mouth_index;
	int mouth_dir;
	float mouth_timer;
	float eyes_index;

	float direction;

	Clumsy(Vector2f pos, Color color):
		Object(pos, Vector2f(0,0), 20.0f, 50.0f, 1.0f),

		body_pos(0),
		body_mouth_timer(0.0f),

		yawning(false),
		mouth_index(0),
		mouth_dir(0),
		mouth_timer(0.0f),
		eyes_index(-5),

		direction(0.0f)
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
		mouth.setColor(Color(200, 100, 100));
	}

	virtual ~Clumsy() {}

	virtual void update(float elapsedTime) {

		body_mouth_timer += elapsedTime;
		if (body_mouth_timer > 0.2f) {
			body_mouth_timer -= 0.2f;
			body_pos++;
			if (body_pos >= 6) {
				body_pos = 0;
			}
			body.setTextureRect(sf::IntRect(bodySize.x * body_pos, 0, bodySize.x, bodySize.y));
		}



		mouth_timer += elapsedTime;
		if (yawning) {
			if (mouth_timer > 0.1f) {

				mouth_timer -= 0.1f;

				if (mouth_dir == 0) {
					mouth_index++;
					if (mouth_index >= 8) {
						mouth_dir = 1;
						mouth_index -= 2;
					}
				} else {
					mouth_index--;
					if (mouth_index < 0) {
						mouth_dir = 0;
						mouth_index = 0;
						yawning = false;
						mouth_timer = - (3 + RANDOM * 25);
					}
				}
				mouth.setTextureRect(sf::IntRect(mouthSize.x * mouth_index, 0, mouthSize.x, mouthSize.y));
			}
		} else if (mouth_timer > 0) {
			mouth_timer = 0;
			yawning = true;
		}

		eyes_index += elapsedTime;
		if (eyes_index >= 0) eyes_index += .2;
		if (eyes_index >= 9) eyes_index = - (3 + RANDOM * 3);

		if (eyes_index >= 3) {
			eyes.setTextureRect(sf::IntRect(eyesSize.x * (eyes_index >= 6.0f ? (6 + (int)(6 - eyes_index)) : (int)(eyes_index)), 0, eyesSize.x, eyesSize.y));
		} else {
			eyes.setTextureRect(sf::IntRect((int)(frac(aabs(eyes_index)) * 3), 0, eyesSize.x, eyesSize.y));
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
