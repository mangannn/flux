#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../physics/Object.hpp"
#include "../Controls.hpp"


class Player: public Object, public Controlled {

public:

	sf::Sprite sprite;

	Vector2f dashVel;

	Vector2f lastPos;

	bool running;
	float dashBegin;

	float sprite_pos;
	float timer;

	float spriteDirection;

	float health;

	bool releaseRope;

	Player(Vector2f pos, Color color, Controls *con);

	virtual ~Player() {}

	virtual void eventCallback(int id);

	virtual void collision_callback(Object *a, float impulse);

	virtual void update(float elapsedTime);

	virtual void draw(RenderWindow *window);
};

#endif
