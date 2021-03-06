#include "Physics.hpp"

#include "Object.hpp"
#include "Constraint.hpp"
#include "World.hpp"

#include <SFML/System.hpp>

#include <math.h>

using namespace sf;


Physics::Physics() {

	objects = new std::vector<Object *>();
	constraints = new std::vector<Constraint *>();

}
Physics::~Physics() {

	{
		Constraint *temp;
		while (!constraints->empty()) {
			temp = constraints->back();
			delete temp;
			constraints->pop_back();
		}
		delete constraints;
	}

	{
		Object *temp;
		while (!objects->empty()) {
			temp = objects->back();
			delete temp;
			objects->pop_back();
		}
		delete objects;
	}
}

bool Physics::can_collide(Object *a, Object *b) {

	/*for (unsigned int i = 0; i < constraints->size(); i++) {
		Constraint *c = constraints->at(i);
		if ((c->a == a && c->b == b) || (c->a == b && c->b == a)) {
			return false;
		}
	}*/
	return true;
}

void Physics::handle_collisions(float elapsedTime) {

	std::vector<Object *>::iterator it_a, it_b;

	for (it_a = objects->begin(); it_a != objects->end(); it_a++) {

		Object *a = *it_a;

		// check collisions with all after object a

		for (it_b = it_a + 1; it_b != objects->end(); it_b++) {

			Object *b = *it_b;

			//if (can_collide(a, b)) {

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
						a->collision_callback(b, fabs(a->mass * (a_v - a_u)));
						b->collision_callback(a, fabs(b->mass * (b_v - b_u)));


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
			//}
		}

		world->collision(a, elapsedTime);
	}
}

void Physics::solve_constraints() {
	for (std::vector<Constraint *>::iterator it = constraints->begin(); it != constraints->end(); it++) {
		(*it)->solve();
	}
}

void Physics::update_positions(float elapsedTime) {

	for (std::vector<Object *>::iterator it = objects->begin(); it < objects->end(); it++) {
		Object *a = *it;

		// update positions
		a->pos += (a->vel * elapsedTime);

		// apply friction
		a->vel *= 1.0f - ((a->friction / a->mass) * elapsedTime);
	}
}

void Physics::step(float elapsedTime) {


	handle_collisions(elapsedTime);

	solve_constraints();

	update_positions(elapsedTime);
}