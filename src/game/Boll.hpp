#ifndef _BOll_H_
#define _BOll_H_

#include "../physics/Object.hpp"

class Constraint;
class Clumsy;

class Boll: public Object {

public:

	sf::Sprite body, eyes, mouth;

	int body_index;
	float body_timer;

	bool blinking;
	int eyes_index;
	float eyes_timer;

	int mouth_index;
	float mouth_timer;

	float direction;

	bool connected;

	Constraint *constraint;

	Clumsy *clumsy;

	Boll(Vector2f pos, Color color, Clumsy *clumsyParam);

	virtual ~Boll() {}

	virtual void update(float elapsedTime);

	virtual void draw(RenderWindow *window);
};

#endif
