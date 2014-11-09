#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include "World.hpp"

class Battleground : public ElasticCircleWorld {

public:

	Battleground(float radius, float elasticity):
		ElasticCircleWorld(radius, elasticity)
	{
	}
	virtual ~Battleground() {}

	virtual void draw(RenderWindow *window) {
		ElasticCircleWorld::draw(window);
	}
};

#endif