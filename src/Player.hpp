#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player: public Object {

public:

	int input_handle;

	sf::Texture standingTex, runningTex;
	sf::Sprite sprite;
	sf::Vector2i spriteSize;

	bool running;

	float sprite_pos;
	int sprite_dir;
	float timer;

	float spriteDirection;

	Player(Vector2f pos, Color color, int input_handleParam):
		Object(pos, Vector2f(0,0), 5.0f, 50.0f, 100.0f, color),

		input_handle(input_handleParam),

		running(false),
		sprite_pos(0),
		sprite_dir(0),
		timer(0.0f),
		spriteDirection(0.0f)
	{

		if ((input_handle >= 0 && input_handle < 8) && sf::Joystick::isConnected(input_handle)) {

			cout << "Joystick: " << input_handle << endl;
			cout << "Button Count: " << sf::Joystick::getButtonCount(input_handle) << endl;
			cout << "X Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::X) ? "yes" : "no") << endl;
			cout << "Y Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Y) ? "yes" : "no") << endl;
			cout << "Z Axis: " << (sf::Joystick::hasAxis(input_handle, sf::Joystick::Z) ? "yes" : "no") << endl;
		}

		if (!standingTex.loadFromFile("media/images/player/standing.png")) {/*error...*/}
		standingTex.setSmooth(true);

		if (!runningTex.loadFromFile("media/images/player/running.png")) {/*error...*/}
		runningTex.setSmooth(true);

		spriteSize = Vector2i(200, 300);

		sprite.setTextureRect(sf::IntRect(0, 0, spriteSize.x, spriteSize.y));
		sprite.setTexture(standingTex);
		sprite.setOrigin(sf::Vector2f((float)spriteSize.x / 2.0f, (float)spriteSize.y / 2.0f));

		sprite.setColor(color);

		sprite.setScale(sf::Vector2f((radius * 2.0f) / (float)spriteSize.x, (radius * 2.0f) / (float)spriteSize.x));
	}

	virtual ~Player() {}

	void handleInput(float elapsedTime) {

		Vector2f v(0,0);

		const float a = 800.0;

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

			v = Vector2f(x * a, y * a);

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

			v *= a;
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

			v *= a;
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

			v *= a;
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

			v *= a;
		}

		vel += (v * elapsedTime);
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
				sprite.setTextureRect(sf::IntRect(spriteSize.x * post, 0, spriteSize.x, spriteSize.y));
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
				sprite.setTextureRect(sf::IntRect(spriteSize.x * (int)sprite_pos, 0, spriteSize.x, spriteSize.y));
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
