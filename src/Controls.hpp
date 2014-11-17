#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <SFML/Graphics.hpp>
using namespace sf;

class Controls;

class Controlled {

public:

	Controls *controls;

	Controlled(int input_handle);
	virtual ~Controlled();

	virtual void event_callback(int id) = 0;

	virtual void handleInput(float elapsedTime) = 0;
};


class Controls {

public:

	Controlled *controlled;

	Controls(Controlled *controlledParam) :
		controlled(controlledParam)
	{}
	virtual ~Controls() {}

	virtual void event_handle(sf::Event event) = 0;

	virtual bool action(int num) = 0;
	virtual Vector2f movement() = 0;
};













class KeyboardControls : public Controls {

public:

	int num_action_keys;

	sf::Keyboard::Key UP, DOWN, LEFT, RIGHT;
	sf::Keyboard::Key action_button[10];

	KeyboardControls(Controlled *controlled, int keyboard_type);
	virtual ~KeyboardControls();

	virtual void event_handle(sf::Event event);

	virtual bool action(int num);

	virtual Vector2f movement();
};




class JoystickControls : public Controls {

public:

	int handle_ID;

	int num_buttons;

	sf::Joystick::Axis axis_x, axis_y;
	unsigned int action_button[32]; // 32 i max number of buttons sfml can handle

	JoystickControls(Controlled *controlled, int handle_IDParam);
	virtual ~JoystickControls();

	virtual void event_handle(sf::Event event);

	virtual bool action(int num);
	virtual Vector2f movement();
};

#endif
