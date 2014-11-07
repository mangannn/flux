#ifndef _PHYSICS_H_
#define _PHYSICS_H_


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

					//cout << "Rörelsemängd: " << ((size(a->vel) * a->mass) + (size(b->vel) * b->mass));

					Vector2f normal = (difference / distance);

					float a_u = dot(a->vel, normal);
					float b_u = dot(b->vel, normal);
					
					// elastic collision
					float a_v = ((a_u * (a->mass - b->mass)) + (2.0f * b->mass * b_u)) / (a->mass + b->mass);
					float b_v = ((b_u * (b->mass - a->mass)) + (2.0f * a->mass * a_u)) / (a->mass + b->mass);

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

		// check room collisions
		{

			/*if (a->pos.x + a->radius < 0.0) {
				a->pos.x += (screen_width + a->radius * 2.0f);
			} else if (a->pos.x - a->radius >= screen_width) {
				a->pos.x -= (screen_width + a->radius * 2.0f);
			}
			if (a->pos.y + a->radius < 0.0) {
				a->pos.y += (screen_height + a->radius * 2.0f);
			} else if (a->pos.y - a->radius >= screen_height) {
				a->pos.y -= (screen_height + a->radius * 2.0f);
			}*/

			Vector2f next_a = a->pos + (a->vel * elapsedTime);
			const float margin = 0.1f;

			/*if ((next_a.x - a->radius < 0.0) || (next_a.x + a->radius >= screen_width)) {
				a->vel.x = -a->vel.x;
			}
			if ((next_a.y - a->radius < 0.0) || (next_a.y + a->radius >= screen_height)) {
				a->vel.y = -a->vel.y;
			}*/

			/*
			if (a->pos.x - a->radius < 0.0) {
				a->vel.x = -a->vel.x;
				a->pos.x = (a->radius + margin);
			} else if (a->pos.x + a->radius >= screen_width) {
				a->vel.x = -a->vel.x;
				a->pos.x = (screen_width - (a->radius + margin));
			}
			if (a->pos.y - a->radius < 0.0) {
				a->vel.y = -a->vel.y;
				a->pos.y = (a->radius + margin);
			} else if (a->pos.y + a->radius >= screen_height) {
				a->vel.y = -a->vel.y;
				a->pos.y = (screen_height - (a->radius + margin));
			}*/

			const float world_radius = WORLD_SIZE;
			Vector2f center_of_world = Vector2f(0.0f, 0.0f);

			Vector2f difference = (next_a - center_of_world);
			float distance = size(difference);

			if (distance > world_radius - a->radius) {

				Vector2f normal = (difference / distance);

				a->vel -= normal * (2.0f * dot(a->vel, normal)); 
				a->pos = center_of_world + (normal * (world_radius - (a->radius + margin)));
			}
		}
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

		//a->vel += Vector2f(0,1) * (elapsedTime * 300.0f);

		a->pos += (a->vel * elapsedTime);

		const float friction = 10.0f;
		a->vel *= 1.0f - ((friction / a->mass) * elapsedTime);
	}
}

void step(float elapsedTime) {

	handle_collisions(elapsedTime);

	solve_constraints();

	update_positions(elapsedTime);
}

#endif