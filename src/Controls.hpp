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

	Controlled(Controls *controlsParam):
		controls(controlsParam)
	{
		controls->controlled = this;
	}
	virtual ~Controlled() {
		delete controls;
	}

	virtual void eventCallback(int id) = 0;
};












class KeyboardControls : public Controls {

public:

	int num_action_keys;

	sf::Keyboard::Key UP, DOWN, LEFT, RIGHT;
	sf::Keyboard::Key actionButton[10];

	KeyboardControls(int k_up, int k_down, int k_left, int k_right, int *in_buttons, int num_in_buttons);
	virtual ~KeyboardControls() {}

	virtual void eventHandle(sf::Event event);

	virtual bool action(int num);

	virtual Vector2f movement();
};




class JoystickControls : public Controls {

public:

	int joystickId;

	int num_buttons;

	sf::Joystick::Axis axisX, axisY;
	unsigned int actionButton[32]; // 32 i max number of buttons sfml can handle

	JoystickControls(int joystickIdParam, int axisXParam, int axisYParam, int *in_buttons, int num_in_buttons);
	virtual ~JoystickControls() {}

	virtual void eventHandle(sf::Event event);

	virtual bool action(int num);
	virtual Vector2f movement();
};

#endif
