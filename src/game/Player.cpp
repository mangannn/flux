
#include "Clumsy.hpp"

#include "Player.hpp"

#include <iostream>

using namespace std;


#define MOVEMENT_SPEED (700.0f)
#define HEAL_RATE (0.001f)

#define DASH_LOADING_TIME (1.0f)
#define DASH_STARTVELOCITY (15000.0f)

#define IMPULSE_DMG (0.00001f)
#define CLUMSY_DMG (5.0f)



Player::Player(Vector2f position, float direction, Color color, Controls *con):
	Object(position, Vector2f(0,0), 6.0f, 50.0f, 100.0f, color),
	Controlled(con),

	dashVel(Vector2f(0.0f, 0.0f)),

	lastPos(pos),

	running(false),
	dashBegin(0),
	sprite_pos(0),
	timer(0.0f),
	spriteDirection(direction),

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

void Player::eventCallback(int id) {
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

void Player::collision_callback(Object *a, float impulse) {
	if (dynamic_cast<Clumsy *>(a)) {
		health -= impulse * IMPULSE_DMG * CLUMSY_DMG;
	} else {
		health -= impulse * IMPULSE_DMG;
	}
}

void Player::update(float elapsedTime) {

	if (health < 1.0) {
		health += HEAL_RATE;
	}


	lastPos = pos;




	Vector2f v = controls->movement();

	vel += (v * MOVEMENT_SPEED) * elapsedTime;






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

void Player::draw(RenderWindow *window) {
	sprite.setRotation(spriteDirection + 90);
	sprite.setPosition(pos);
	window->draw(sprite);
}
