#ifndef _BOll_H_
#define _BOll_H_

#include "../physics/Object.hpp"
#include "../physics/Constraint.hpp"

class Boll: public Object {

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

	bool connected;

	Constraint *constraint;

	Boll(Vector2f pos, Color color):
		Object(pos, Vector2f(0,0), 5.0f, 10.0f, 4.0f),

		body_pos(0),
		body_mouth_timer(0.0f),

		yawning(false),
		mouth_index(0),
		mouth_dir(0),
		mouth_timer(0.0f),
		eyes_index(-5),

		direction(0.0f),

		connected(false),

		constraint(NULL)
	{

		sf::Vector2f scale((radius * 2.0f) / (float)bollSize.x, (radius * 2.0f) / (float)bollSize.y);
		sf::Vector2f origin((float)bollSize.x / 2.0f, (float)bollSize.y / 2.0f);


		body.setTextureRect(sf::IntRect(0, 0, bollSize.x, bollSize.y));
		body.setTexture(bollTex);
		body.setScale(scale);
		body.setOrigin(origin);

		eyes.setTextureRect(sf::IntRect(0, 0, bollSize.x, bollSize.y));
		eyes.setTexture(bollTex);
		eyes.setScale(scale);
		eyes.setOrigin(origin);

		mouth.setTextureRect(sf::IntRect(0, 0, bollSize.x, bollSize.y));
		mouth.setTexture(bollTex);
		mouth.setScale(scale);
		mouth.setOrigin(origin);


		body.setColor(color);
		eyes.setColor(Color(0xff, 0, 0, 50));
		mouth.setColor(Color(0, 0xff, 0, 50));
	}

	virtual ~Boll() {}

	virtual void update(float elapsedTime) {

		body_mouth_timer += elapsedTime;
		if (body_mouth_timer > 0.2f) {
			body_mouth_timer -= 0.2f;
			body_pos++;
			if (body_pos >= 6) {
				body_pos = 0;
			}
			body.setTextureRect(sf::IntRect(bollSize.x * body_pos, 0, bollSize.x, bollSize.y));
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
						mouth_timer = -3;
					}
				}
				mouth.setTextureRect(sf::IntRect(bollSize.x * mouth_index, 0, bollSize.x, bollSize.y));
			}
		} else if (mouth_timer > 0) {
			mouth_timer = 0;
			yawning = true;
		}

		eyes_index += elapsedTime;
		if (eyes_index >= 0) eyes_index += .2;
		if (eyes_index >= 9) eyes_index = -3;

		if (eyes_index >= 3) {
			eyes.setTextureRect(sf::IntRect(bollSize.x * (eyes_index >= 6.0f ? (6 + (int)(6 - eyes_index)) : (int)(eyes_index)), 0, bollSize.x, bollSize.y));
		} else {
			eyes.setTextureRect(sf::IntRect((int)(frac(fabs(eyes_index)) * 3), 0, bollSize.x, bollSize.y));
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
