#include "Controls.hpp"

#include <iostream>
using namespace std;

#include <math.h>

KeyboardControls::KeyboardControls(int k_up, int k_down, int k_left, int k_right, int *in_buttons, int num_in_buttons) {
	UP = (sf::Keyboard::Key)k_up;
	DOWN = (sf::Keyboard::Key)k_down;
	LEFT = (sf::Keyboard::Key)k_left;
	RIGHT = (sf::Keyboard::Key)k_right;

	num_action_keys = num_in_buttons;
	for (int i = 0; i < num_action_keys; i++) {
		actionButton[i] = (sf::Keyboard::Key)in_buttons[i];
	}
}

void KeyboardControls::eventHandle(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		for (int i = 0; i < num_action_keys; i++) {
			if (event.key.code == actionButton[i]) {
				controlled->eventCallback(i);
			}
		}
	}
}

bool KeyboardControls::action(int num) {
	return sf::Keyboard::isKeyPressed(actionButton[num]);
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











JoystickControls::JoystickControls(int joystickIdParam, int axisXParam, int axisYParam, int *in_buttons, int num_in_buttons) :
	joystickId(joystickIdParam),
	axisX((sf::Joystick::Axis)axisXParam),
	axisY((sf::Joystick::Axis)axisYParam)
{
	if (joystickId >= 0 && joystickId < 8) {
		if (sf::Joystick::isConnected(joystickId)) {

			if (!sf::Joystick::hasAxis(joystickId, axisX)) {
				cout << "Joystick " << joystickId << " doesn't have axis " << axisX << endl;
			}
			if (!sf::Joystick::hasAxis(joystickId, axisY)) {
				cout << "Joystick " << joystickId << " doesn't have axis " << axisY << endl;
			}

			num_buttons = sf::Joystick::getButtonCount(joystickId);

			if (num_in_buttons > num_buttons) {
				cout << "Joystick " << joystickId << " doesn't have enought buttons" << endl;
			} else {
				for (int i = 0; i < num_in_buttons; i++) {
					if (in_buttons[i] >= 0 && in_buttons[i] < num_buttons) {
						actionButton[i] = in_buttons[i];
					} else {
						cout << "Joystick " << joystickId << " doesn't have button " << in_buttons[i] << endl;
					}
				}
			}
		} else {
			cout << "Joystick not conected:" << joystickId << endl;
		}
	} else {
		cout << "Joystick number ivalid: " << joystickId << endl;
	}
}

void JoystickControls::eventHandle(sf::Event event) {
	if (event.type == sf::Event::JoystickButtonPressed) {
		if ((int)event.joystickButton.joystickId == joystickId) {
			for (int i = 0; i < num_buttons; i++) {
				if (event.joystickButton.button == actionButton[i]) {
					controlled->eventCallback(i);
				}
			}
		}
	}
}

bool JoystickControls::action(int num) {
	return sf::Joystick::isButtonPressed(joystickId, actionButton[num]);
}
Vector2f JoystickControls::movement() {

	float x = sf::Joystick::getAxisPosition(joystickId, axisX) / 100.0f;
	float y = sf::Joystick::getAxisPosition(joystickId, axisY) / 100.0f;

	return Vector2f(x, y);
}

