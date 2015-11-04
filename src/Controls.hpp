#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <SFML/Graphics.hpp>
using namespace sf;

class Controlled;

class Controls {

public:

	Controlled *controlled;

	Controls() :
		controlled(NULL)
	{}
	virtual ~Controls() {}

	virtual void eventHandle(sf::Event event) = 0;

	virtual bool action(int num) = 0;
	virtual Vector2f movement() = 0;
};


class Controlled {

public:

	Controls *controls;

	Controlled(Controls *controlsParam);
	Controlled(int input_handle);

	virtual ~Controlled();

	virtual void eventCallback(int id) = 0;
};












class KeyboardControls : public Controls {

public:

	int num_action_keys;

	sf::Keyboard::Key UP, DOWN, LEFT, RIGHT;
	sf::Keyboard::Key action_button[10];

	KeyboardControls(int keyboard_type);
	KeyboardControls(int k_up, int k_down, int k_left, int k_right, int *in_buttons, int num_in_buttons);
	virtual ~KeyboardControls() {}

	virtual void eventHandle(sf::Event event);

	virtual bool action(int num);

	virtual Vector2f movement();
};




class JoystickControls : public Controls {

public:

	int handle_ID;

	int num_buttons;

	sf::Joystick::Axis axisX, axisY;
	unsigned int action_button[32]; // 32 i max number of buttons sfml can handle

	JoystickControls(int handle_IDParam);
	JoystickControls(int handle_IDParam, int axisXParam, int axisYParam, int *in_buttons, int num_in_buttons);
	virtual ~JoystickControls() {}

	virtual void eventHandle(sf::Event event);

	virtual bool action(int num);
	virtual Vector2f movement();
};

#endif
