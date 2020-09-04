#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../physics/Object.hpp"
#include "../Controls.hpp"


class Player: public Object, public Controlled {

public:

	sf::Sprite sprite;

	Vector2f lastPos;

	bool dashing;
	float dashTimer;
	Vector2f dashVel;

	float sprite_pos;
	float sprite_timer;

	float spriteDirection;

	float health;

	bool releaseRope;

	Player(Vector2f position, float direction, Color color, Controls *controls);

	virtual ~Player() {}

	virtual void eventCallback(int id);

	virtual void collision_callback(Object *a, float impulse);

	virtual void update(float elapsedTime);

	virtual void draw(RenderTarget *target);
};

#endif
