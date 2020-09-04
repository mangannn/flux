
#include "Clumsy.hpp"

#include "Player.hpp"

#include "ParticleSystem.hpp"

#include <iostream>

using namespace std;


#define MOVEMENT_ACCELRATION (700.0f)
#define HEAL_RATE (0.001f)

// max velocity is about 350.f

#define DASH_LOADING_TIME (1.0f)
#define DASH_START_VELOCITY (1200.0f)
#define DASH_END_VELOCITY (370.0f)

#define IMPULSE_DMG (0.00001f)
#define CLUMSY_DMG (5.0f)



Player::Player(Vector2f position, float direction, Color color, Controls *con):
	Object(position, Vector2f(0,0), 6.0f, 50.0f, 100.0f, color),
	Controlled(con),

	lastPos(pos),

	dashing(false),
	dashTimer(0),

	sprite_pos(0),
	sprite_timer(0.0f),
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
			if (dashTimer >= DASH_LOADING_TIME) {

				Vector2f v = controls->movement();
				float vSize = size(v);
				if (vSize > 0) {
					dashVel = (v / vSize) * DASH_START_VELOCITY;
					dashing = true;
					dashTimer = 0;
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

	dashTimer += elapsedTime;




	if (!dashing) {

		vel += (controls->movement() * MOVEMENT_ACCELRATION) * elapsedTime;

	} else {

		if (sqrSize(dashVel) < DASH_END_VELOCITY * DASH_END_VELOCITY) {
			dashing = false;
		}
		
		vel = dashVel;
		dashVel *= 0.95f;
		
		ParticleSystem::addParticle(pos + 3.f * RANDOM_VECTOR2F,-0.05f * vel + 60.f * RANDOM_VECTOR2F);
	}



	// Sprite update

	float current_velocity = size(vel);

	if (current_velocity < 5.0f) {

		vel = Vector2f(0,0);


		sprite_timer += elapsedTime;

		if (sprite_timer > 0.25f) {

			sprite.setTexture(standingTex);

			sprite_timer -= 0.25f;

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

void Player::draw(RenderTarget *target) {
	sprite.setRotation(spriteDirection + 90);
	sprite.setPosition(pos);
	target->draw(sprite);
}
