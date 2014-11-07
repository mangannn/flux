#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player: public Object {

public:

	int input_handle;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2i spriteSize;

	int sprite_pos;
	int sprite_dir;
	float timer;

	float direction;

	Player(Vector2f pos, Color color, int input_handleParam):
		Object(pos, Vector2f(0,0), 50.0f, 5.0f, color),
		input_handle(input_handleParam),
		sprite_pos(0),
		sprite_dir(0),
		timer(0.0f),
		direction(0.0f)
	{

		if ((input_handle >= 0 && input_handle < 8) && sf::Joystick::isConnected(input_handle)) {

			cout << "Joystick: " << input_handle << endl;
			cout << "Button Count: " << sf::Joystick::getButtonCount(input_handle) << endl;
			cout << "X Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::X) ? "yes" : "no") << endl;
			cout << "Y Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Y) ? "yes" : "no") << endl;
			cout << "Z Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Z) ? "yes" : "no") << endl;
		} else {
			input_handle = -1;
		}

		if (!texture.loadFromFile("media/images/player_running.png")) {/*error...*/}
		texture.setSmooth(true);

		spriteSize = Vector2i(200, 300);

		sprite.setTextureRect(sf::IntRect(0, 0, spriteSize.x, spriteSize.y));
		sprite.setTexture(texture);
		sprite.setOrigin(sf::Vector2f((float)spriteSize.x / 2.0f, (float)spriteSize.y / 2.0f));

		//sprite.setColor(sf::Color(0, 255, 0)); // green

		sprite.setScale(sf::Vector2f((radius * 2.0f) / (float)spriteSize.x, (radius * 2.0f) / (float)spriteSize.x));
	}

	virtual ~Player() {}

	void handleInput(float elapsedTime) {

		Vector2f v(0,0);

		if (input_handle >= 0) {

			const float a = 200.0;

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

			v = Vector2f(x * a, y * a);

		} else {

			const float a = 300.0;

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

			v *= a;
		}

		if (size(v) > 0.1) {
			float d = ((180.0f / M_PI) * atan2(v.y, v.x)) - direction;
			if (d > 180.0f) {
				d -= 360.0f;
			} else if (d < -180.0f) {
				d += 360.0f;
			}

			const float a = 600.0f;

			if (d < 0.0f) {
				direction -= a * elapsedTime;
			} else {
				direction += a * elapsedTime;
			}
		}

		vel += (v * elapsedTime);
	}

	void update(float elapsedTime) {
		timer += elapsedTime;
		if (timer > 0.1f) {

			timer -= 0.1f;

			if (sprite_dir == 0) {
				sprite_pos++;
				if (sprite_pos >= 7) {
					sprite_dir = 1;
				}
			} else {
				sprite_pos--;
				if (sprite_pos <= 0) {
					sprite_dir = 0;
				}
			}
			sprite.setTextureRect(sf::IntRect(spriteSize.x * sprite_pos, 0, spriteSize.x, spriteSize.y));
		}
	}

	virtual void draw(RenderWindow *window) {
		sprite.setRotation(angle(vel));//(sprite.getRotation() + angle(vel)) / 2);

		sprite.setPosition(pos);
		window->draw(sprite);
	}
};

#endif
