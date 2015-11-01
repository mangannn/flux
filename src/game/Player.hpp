#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Clumsy.hpp"
#include "../physics/Object.hpp"
#include "../Controls.hpp"

#define DASH_LOADING_TIME 1.0f
#define DASH_STARTVELOCITY 15000.0f

#include <iostream>

using namespace std;


class Player: public Object, public Controlled {

public:

	sf::Sprite sprite;

	Vector2f dashVel;

	Vector2f lastPos;

	bool running;
	float dashBegin;

	float sprite_pos;
	float timer;

	float spriteDirection;

	float health;

	bool releaseRope;

	Player(Vector2f pos, Color color, int input_handle):
		Object(pos, Vector2f(0,0), 6.0f, 50.0f, 100.0f, color),
		Controlled(input_handle),

		dashVel(Vector2f(0.0f, 0.0f)),

		lastPos(pos),

		running(false),
		dashBegin(0),
		sprite_pos(0),
		timer(0.0f),
		spriteDirection(0.0f),

		health(1.0f),
		releaseRope(false)
	{

		sprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		sprite.setTexture(standingTex);
		sprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f) + Vector2f(0, -30));

		sprite.setColor(color);

		Vector2f scale = sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x);

		sprite.setScale(scale * 1.1f);
	}

	virtual ~Player() {}

	virtual void eventCallback(int id) {
		switch (id) {
			case 0: {
				if (dashBegin >= 0) {

					Vector2f v = controls->movement();
					float vSize = size(v);
					if (vSize > 0) {
						dashVel = (v / vSize) * DASH_STARTVELOCITY;
						dashBegin = -DASH_LOADING_TIME;
					}
				}
			} break;
			case 1: {
				releaseRope = true;
			} break;
			default: cout << "Action button pressed: " << id << endl;
		}
	}

	virtual void collision_callback(Object *a, float impulse) {
		if (dynamic_cast<Clumsy *>(a)) {
			health -= impulse * 0.00005f;
		} else {
			health -= impulse * 0.00001f;
		}
	}

	virtual void update(float elapsedTime) {

		if (health < 1.0) {
			health += 0.001;
		}


		lastPos = pos;



		const float a = 700.0;

		Vector2f v = controls->movement();

		vel += (v * a) * elapsedTime;






		// Dash update

		if (dashBegin < 0){
			dashBegin += elapsedTime;
		}

		dashVel *= 0.8f;

		const float dashEndVel = 5000;

		if (sqrSize(dashVel) < dashEndVel * dashEndVel) {
			dashVel = Vector2f(0.0f, 0.0f);
		} else {
			vel += dashVel * elapsedTime;
		}





		// Sprite update

		float current_velocity = size(vel);

		if (current_velocity < 5.0f) {

			vel = Vector2f(0,0);


			timer += elapsedTime;

			if (timer > 0.25f) {

				sprite.setTexture(standingTex);

				timer -= 0.25f;

				sprite_pos += 1.0f;
				if (sprite_pos >= 4.0f) {
					sprite_pos = 0.0f;
				}
				sprite.setTextureRect(sf::IntRect(playerSpriteSize.x * (int)sprite_pos, 0, playerSpriteSize.x, playerSpriteSize.y));
			}
		} else {

			spriteDirection = periodValueBetween(spriteDirection, angle(vel), 10.0f * elapsedTime);

			int pre = (int)sprite_pos;
			sprite_pos += current_velocity * elapsedTime * 0.3f;
			int post = (int)sprite_pos;

			if (post != pre) {

				sprite.setTexture(runningTex);

				if (post >= 8.0f) {
					post = 0;
					sprite_pos = 0.0f;
				}
				sprite.setTextureRect(sf::IntRect(playerSpriteSize.x * post, 0, playerSpriteSize.x, playerSpriteSize.y));
			}
		}
	}

	virtual void draw(RenderWindow *window) {
		sprite.setRotation(spriteDirection + 90);
		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
