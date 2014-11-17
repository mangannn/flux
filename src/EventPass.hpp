#ifndef _EVENT_PASS_H_
#define _EVENT_PASS_H_

#include <SFML/Graphics.hpp>

class EventPass {

public:

	EventPass() {}
	virtual ~EventPass() {}

	virtual EventPass *event_handle(sf::Event event) = 0;

	virtual EventPass *update(float elapsedTime) = 0;

	virtual void draw(sf::RenderWindow *window) = 0;
};

#endif