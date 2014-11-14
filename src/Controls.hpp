#ifndef _CONTROLS_H_
#define _CONTROLS_H_

class Controls {

public:

	Controls() {}
	virtual ~Controls() {}

	virtual bool action(int num) = 0;
	virtual Vector2f movement() = 0;
};

class KeyboardControls : public Controls {

public:

	sf::Keyboard::Key UP, DOWN, LEFT, RIGHT;
	sf::Keyboard::Key action_button[10];

	KeyboardControls(int keyboard_type) {
		if (keyboard_type == 0) {
			UP = sf::Keyboard::Up;
			DOWN = sf::Keyboard::Down;
			LEFT = sf::Keyboard::Left;
			RIGHT = sf::Keyboard::Right;
			action_button[0] = sf::Keyboard::M;
		} else if (keyboard_type == 1) {
			UP = sf::Keyboard::W;
			DOWN = sf::Keyboard::S;
			LEFT = sf::Keyboard::A;
			RIGHT = sf::Keyboard::D;
			action_button[0] = sf::Keyboard::Q;
		} else if (keyboard_type == 2) {
			UP = sf::Keyboard::T;
			DOWN = sf::Keyboard::G;
			LEFT = sf::Keyboard::F;
			RIGHT = sf::Keyboard::H;
			action_button[0] = sf::Keyboard::R;
		} else {
			UP = sf::Keyboard::I;
			DOWN = sf::Keyboard::K;
			LEFT = sf::Keyboard::J;
			RIGHT = sf::Keyboard::L;
			action_button[0] = sf::Keyboard::U;
		}


		action_button[1] = sf::Keyboard::X;
		action_button[2] = sf::Keyboard::C;
	}
	virtual ~KeyboardControls() {}

	virtual bool action(int num) {
		return sf::Keyboard::isKeyPressed(action_button[num]);
	}

	virtual Vector2f movement() {

		Vector2f v(0.0f, 0.0f);

		if (sf::Keyboard::isKeyPressed(UP)) {
			v.y -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(DOWN)) {
			v.y += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(LEFT)) {
			v.x -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(RIGHT)) {
			v.x += 1.0f;
		}

		if (fabs(v.x) > 0.0f && fabs(v.y) > 0.0f) {
			v *= (float)M_SQRT1_2;
		}

		return v;
	};
};

class JoystickControls : public Controls {

public:

	int handle_ID;

	int num_buttons;

	sf::Joystick::Axis HORIZONTAL, VERTICAL;
	int action_button[10];

	JoystickControls(int handle_IDParam) :
		handle_ID(handle_IDParam)
	{
		if (handle_ID >= 0 && handle_ID < 8) {
			if (sf::Joystick::isConnected(handle_ID)) {

				num_buttons = sf::Joystick::getButtonCount(handle_ID);

				cout << "Joystick: " << handle_ID << endl;
				cout << "Button Count: " << sf::Joystick::getButtonCount(handle_ID) << endl;
				cout << "X Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::X) ? "yes" : "no") << endl;
				cout << "Y Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::Y) ? "yes" : "no") << endl;
				cout << "Z Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::Z) ? "yes" : "no") << endl;
			} else {
				cout << "Joystick not conected:" << handle_ID << endl;
			}
		} else {
			cout << "Joystick number ivalid: " << handle_ID << endl;
		}

		action_button[0] = 0;
		action_button[1] = 1;
		action_button[2] = 2;
	}
	virtual ~JoystickControls() {}

	virtual bool action(int num) {
		return sf::Joystick::isButtonPressed(handle_ID, action_button[num]);
	}
	virtual Vector2f movement() {

		float x = sf::Joystick::getAxisPosition(handle_ID, sf::Joystick::X);
		float y = sf::Joystick::getAxisPosition(handle_ID, sf::Joystick::Y);

		return Vector2f(x, y);
	};
};

#endif
