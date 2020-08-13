#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <vector>

class Object;
class Constraint;
class World;

class Physics {

protected:

	bool can_collide(Object *a, Object *b);
	void handle_collisions(float elapsedTime);
	void solve_constraints();
	void update_positions(float elapsedTime);

	void step(float elapsedTime);

public:

	std::vector<Object *> *objects;
	std::vector<Constraint *> *constraints;

	World *world;

	Physics();
	virtual ~Physics();

	virtual bool collision_callback(Object *a, Object *b) {
		return true;
	}
};

#endif
