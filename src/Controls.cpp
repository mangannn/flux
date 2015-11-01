#include "Controls.hpp"

#include <iostream>
using namespace std;

#include <math.h>

KeyboardControls::KeyboardControls(Controlled *controlled, int keyboard_type) :
	Controls(controlled)
{


	// this are temporary settings, they should not be hardcoded

	if (keyboard_type == 0) {
		UP = sf::Keyboard::Up;
		DOWN = sf::Keyboard::Down;
		LEFT = sf::Keyboard::Left;
		RIGHT = sf::Keyboard::Right;

		num_action_keys = 2;
		action_button[0] = sf::Keyboard::N;
		action_button[1] = sf::Keyboard::M;

	} else if (keyboard_type == 1) {
		UP = sf::Keyboard::W;
		DOWN = sf::Keyboard::S;
		LEFT = sf::Keyboard::A;
		RIGHT = sf::Keyboard::D;

		num_action_keys = 2;
		action_button[0] = sf::Keyboard::Z;
		action_button[1] = sf::Keyboard::X;

	} else if (keyboard_type == 2) {
		UP = sf::Keyboard::T;
		DOWN = sf::Keyboard::G;
		LEFT = sf::Keyboard::F;
		RIGHT = sf::Keyboard::H;

		num_action_keys = 2;
		action_button[0] = sf::Keyboard::V;
		action_button[1] = sf::Keyboard::B;

	} else {
		UP = sf::Keyboard::I;
		DOWN = sf::Keyboard::K;
		LEFT = sf::Keyboard::J;
		RIGHT = sf::Keyboard::L;

		num_action_keys = 2;
		action_button[0] = sf::Keyboard::O;
		action_button[1] = sf::Keyboard::P;
	}
}
KeyboardControls::~KeyboardControls() {}

void KeyboardControls::eventHandle(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		for (int i = 0; i < num_action_keys; i++) {
			if (event.key.code == action_button[i]) {
				controlled->eventCallback(i);
			}
		}
	}
}

bool KeyboardControls::action(int num) {
	return sf::Keyboard::isKeyPressed(action_button[num]);
}

Vector2f KeyboardControls::movement() {

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
}













JoystickControls::JoystickControls(Controlled *controlled, int handle_IDParam) :
	Controls(controlled),
	handle_ID(handle_IDParam)
{
	if (handle_ID >= 0 && handle_ID < 8) {
		if (sf::Joystick::isConnected(handle_ID)) {

			num_buttons = sf::Joystick::getButtonCount(handle_ID);

			/*cout << "Joystick: " << handle_ID << endl;
			cout << "Button Count: " << sf::Joystick::getButtonCount(handle_ID) << endl;
			cout << "X Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::X) ? "yes" : "no") << endl;
			cout << "Y Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::Y) ? "yes" : "no") << endl;
			cout << "Z Axis: " << (sf::Joystick::hasAxis(handle_ID, sf::Joystick::Z) ? "yes" : "no") << endl;*/
		} else {
			cout << "Joystick not conected:" << handle_ID << endl;
		}
	} else {
		cout << "Joystick number ivalid: " << handle_ID << endl;
	}



	// this are temporary settings, they should not be hardcoded
	{
		axis_x = sf::Joystick::X;
		axis_y = sf::Joystick::Y;

		for (int i = 0; i < num_buttons; i++) {
			action_button[i] = i;
		}
	}
}
JoystickControls::~JoystickControls() {}

void JoystickControls::eventHandle(sf::Event event) {
	if (event.type == sf::Event::JoystickButtonPressed) {
		for (int i = 0; i < num_buttons; i++) {
			if (event.joystickButton.button == action_button[i]) {
				controlled->eventCallback(i);
			}
		}
	}
}

bool JoystickControls::action(int num) {
	return sf::Joystick::isButtonPressed(handle_ID, action_button[num]);
}
Vector2f JoystickControls::movement() {

	float x = sf::Joystick::getAxisPosition(handle_ID, axis_x);
	float y = sf::Joystick::getAxisPosition(handle_ID, axis_y);

	return Vector2f(x, y);
}











Controlled::Controlled(int input_handle) {
	if (input_handle >= 0) {
		controls = new JoystickControls(this, input_handle);
	} else {
		controls = new KeyboardControls(this, (-input_handle) - 1);
	}
}
Controlled::~Controlled() {
	delete controls;
}
