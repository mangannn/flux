#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"
#include "Controls.hpp"

#define DASH_LOADING_TIME 2.0f
#define DASH_STARTVELOCITY 700.0f

class Player: public Object, public Controlled {

public:

	sf::Sprite sprite;

	Vector2f dashVel;
	Vector2f dashPos;

	bool running;
	float dashBegin;

	float sprite_pos;
	int sprite_dir;
	float timer;

	float spriteDirection;

	float health;

	Player(Vector2f pos, Vector2f vel, Color color, int input_handle):
		Object(pos, vel, 5.0f, 50.0f, 100.0f, color),
		Controlled(input_handle),

		dashVel(Vector2f(0.0f, 0.0f)),
		dashPos(pos),

		running(false),
		dashBegin(0),
		sprite_pos(0),
		sprite_dir(0),
		timer(0.0f),
		spriteDirection(0.0f),

		health(1.0f)
	{

		playerSpriteSize = Vector2i(200, 300);

		sprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		sprite.setTexture(standingTex);
		sprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f));

		sprite.setColor(color);

		sprite.setScale(sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x));
	}

	virtual ~Player() {}

	virtual void event_callback(int id) {
		switch (id) {
			case 0: {
				cout << "IVAANNN, FIXA SÅ DASHEN GÖRS HÄR!!! MÖÖÖÖPPPP!!!! KOLLA PÅ MIG!!!!! JAG ÄR UNIVERSUMS CENTRUM!!!!" << endl;
			} break;
			default: cout << "Action button pressed: " << id << endl;
		}
	}

	virtual void handleInput(float elapsedTime) {
		vel -= dashVel;

		const float a = 700.0;

		Vector2f v = controls->movement();

		vel += (v * a) * elapsedTime;

		dashVel *= 0.8f;
		if (sqrSize(dashVel) < 40 * 40) dashVel = Vector2f(0, 0);
		if (controls->action(0)) {
			if (dashBegin >= 0) {
				dashPos = pos;
				float vSize = size(v);
				if (vSize > 0) dashVel = v / vSize * DASH_STARTVELOCITY;
				dashBegin = - DASH_LOADING_TIME / 2;
			}
		}
		if (sqrSize(dashVel) < .2f) dashVel = Vector2f(0.0f, 0.0f);
		//if (input_handle == -1) cout << "Dash: " << dashVel.x << ", " << dashVel.y << endl;
	}

	virtual void collision_callback(float impulse) {
		health -= impulse * 0.00001f;
		if (health < 0.0f) {
			cout << "DEAD" << endl;
			health = 1.0f;
		}
	}

	virtual void update(float elapsedTime) {

		if (dashBegin < 0){
			dashBegin += elapsedTime;
		}

		if (sqrSize(dashVel) > 0) {

			if (dashedIntersection(dashPos, pos))
				cout << "CUT OFF ROPE!!!" << endl;
		}

		float current_velocity = size(vel);

		if (current_velocity < 5.0f) {
			vel = Vector2f(0,0);
			running = false;
		} else {
			running = true;
		}

		if (running) {

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
		} else {

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
		}
		vel += dashVel;
		/*if (input_handle == -1 && size(dashVel) > 0.0f) {
			cout << "Added " << size(dashVel) << "from velocity" << endl;
			cout << "Velocity is now " << size(vel) << endl << endl;
		}*/

	}

	virtual void draw(RenderWindow *window) {
		sprite.setRotation(spriteDirection + 90);
		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
