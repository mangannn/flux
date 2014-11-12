#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Object.hpp"

class Player: public Object {

public:

	int input_handle;

	sf::Sprite sprite;

	bool running;

	float sprite_pos;
	int sprite_dir;
	float timer;

	float spriteDirection;

	float health;

	Player(Vector2f pos, Vector2f vel, Color color, int input_handleParam):
		Object(pos, vel, 5.0f, 50.0f, 100.0f, color),

		input_handle(input_handleParam),

		running(false),
		sprite_pos(0),
		sprite_dir(0),
		timer(0.0f),
		spriteDirection(0.0f),

		health(1.0f)
	{

		if ((input_handle >= 0 && input_handle < 8) && sf::Joystick::isConnected(input_handle)) {

			cout << "Joystick: " << input_handle << endl;
			cout << "Button Count: " << sf::Joystick::getButtonCount(input_handle) << endl;
			cout << "X Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::X) ? "yes" : "no") << endl;
			cout << "Y Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Y) ? "yes" : "no") << endl;
			cout << "Z Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Z) ? "yes" : "no") << endl;
		}

		playerSpriteSize = Vector2i(200, 300);

		sprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		sprite.setTexture(standingTex);
		sprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f));

		sprite.setColor(color);

		sprite.setScale(sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x));
	}

	virtual ~Player() {}

	void handleInput(float elapsedTime) {

		Vector2f v(0,0);

		const float a = 700.0;

		if (input_handle >= 0) {

			if (sf::Joystick::isButtonPressed(input_handle, 0)) {
				cout << "BAM!" << endl;
			}
			if (sf::Joystick::isButtonPressed(input_handle, 1)) {
				cout << "PUH!" << endl;
			}
			if (sf::Joystick::isButtonPressed(input_handle, 3)) {
				cout << "KLONK!" << endl;
			}

			float x = sf::Joystick::getAxisPosition(input_handle, sf::Joystick::X);
			float y = sf::Joystick::getAxisPosition(input_handle, sf::Joystick::Y);

			v = Vector2f(x, y);

		} else if (input_handle == -1) {

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				cout << "BAM!" << endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
				cout << "PUH!" << endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
				cout << "KLONK!" << endl;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				v.x -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				v.x += 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				v.y -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				v.y += 1.0f;
			}
		} else if (input_handle == -2) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				v.x -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				v.x += 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				v.y -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				v.y += 1.0f;
			}
		} else if (input_handle == -3) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				v.x -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
				v.x += 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
				v.y -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
				v.y += 1.0f;
			}
		} else if (input_handle == -4) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
				v.x -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
				v.x += 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
				v.y -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
				v.y += 1.0f;
			}
		}

		float s = size(v);

		if (s > 0) {
			vel += ((v / size(v)) * a * elapsedTime);
		}
	}

	virtual void collision_callback(float impulse) {
		health -= impulse * 0.00001f;
		if (health < 0.0f) {
			cout << "DEAD: " << endl;
			health = 1.0f;
		}
	}

	virtual void update(float elapsedTime) {


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

		//sprite.setPosition(pos);
		//sprite.setRotation(spriteDirection + 90);

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
	}

	virtual void draw(RenderWindow *window) {
		sprite.setRotation(spriteDirection + 90);
		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
