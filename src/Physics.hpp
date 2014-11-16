#ifndef _PHYSICS_H_
#define _PHYSICS_H_

using namespace sf;
using namespace std;

#include "Object.hpp"
#include "Constraint.hpp"
#include "World.hpp"


extern std::vector<Object *> *objects;
extern std::vector<Constraint *> *constraints;

extern World *world;


bool collision_callback(Object *a, Object *b);

bool can_collide(Object *a, Object *b) {

	/*for (unsigned int i = 0; i < constraints->size(); i++) {
		Constraint *c = constraints->at(i);
		if ((c->a == a && c->b == b) || (c->a == b && c->b == a)) {
			return false;
		}
	}*/
	return true;
}

void handle_collisions(float elapsedTime) {
	for (unsigned int i = 0; i < objects->size(); i++) {

		Object *a = objects->at(i);

		// check collisions with all after object a
		for (unsigned int j = i + 1; j < objects->size(); j++) {

			Object *b = objects->at(j);

			if (can_collide(a, b)) {

				Vector2f next_a = a->pos + (a->vel * elapsedTime);
				Vector2f next_b = b->pos + (b->vel * elapsedTime);

				Vector2f difference = (next_a - next_b);
				float distance = size(difference);

				if (distance < a->radius + b->radius) {
					if (collision_callback(a, b)) {


						// needed when creating object with the same position
						if (distance == 0.0f) {
							difference.x = 1.0f;
							distance = 1.0f;
						}

						//cout << "Rörelsemängd: " << ((size(a->vel) * a->mass) + (size(b->vel) * b->mass));

						Vector2f normal = (difference / distance);

						float a_u = dot(a->vel, normal);
						float b_u = dot(b->vel, normal);

						// elastic collision
						float a_v = ((a_u * (a->mass - b->mass)) + (2.0f * b->mass * b_u)) / (a->mass + b->mass);
						float b_v = ((b_u * (b->mass - a->mass)) + (2.0f * a->mass * a_u)) / (a->mass + b->mass);


						// impulse
						a->collision_callback(aabs(a->mass * (a_v - a_u)));
						b->collision_callback(aabs(b->mass * (b_v - b_u)));


						a->vel += (normal * (a_v - a_u));
						b->vel += (normal * (b_v - b_u));

						// m1u1+m2u2=m1v1+m2v2 -> v1 = u1+(m2/m1)(u2-v2)
						// (m1u1^2)/2+(m2u2^2)/2=(m1v1^2)/2+(m2v2^2)/2 -> v1 = sqrt(u1^2+(m2/m1)(u2^2-v2^2))

						// u1+(m2/m1)(u2-v2) = sqrt(u1^2+(m2/m1)(u2^2-v2^2))

						// v1 = (u1(m1-m2) + 2m2u2) / (m1 + m2)

						float intended_distance = a->radius + b->radius;

						Vector2f center = (a->pos + b->pos) / 2.0f;
						a->pos = center + (normal * (intended_distance / 2.0f));
						b->pos = center - (normal * (intended_distance / 2.0f));

						//cout << " -> " << ((size(a->vel) * a->mass) + (size(b->vel) * b->mass)) << endl;
					}
				}
			}
		}

		world->collision(a, elapsedTime);
	}
}

void solve_constraints() {
	for (unsigned int i = 0; i < constraints->size(); i++) {
		constraints->at(i)->solve();
	}
}

void update_positions(float elapsedTime) {

	for (unsigned int i = 0; i < objects->size(); i++) {
		Object *a = objects->at(i);

		// update positions
		a->pos += (a->vel * elapsedTime);

		// apply friction
		a->vel *= 1.0f - ((a->friction / a->mass) * elapsedTime);
	}
}

void step(float elapsedTime) {


	handle_collisions(elapsedTime);

	solve_constraints();

	update_positions(elapsedTime);
}

#endif
