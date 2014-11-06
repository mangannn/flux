#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

class Constraint {

public:

	Object *a;
	Object *b;

	Constraint(Object *aParam, Object *bParam):
		a(aParam),
		b(bParam)
	{
		if (a == b) {
			cout << "Constraint created between object and itself!" << endl;
		}
	}
	virtual ~Constraint() {}

	virtual void solve() = 0;

	virtual void draw(RenderWindow *window) = 0;
};

class MaxDistanceConstraint: public Constraint {

public:

	float distance;

	MaxDistanceConstraint(Object *a, Object *b, float distanceParam = 100.0f):
		Constraint(a, b),
		distance(distanceParam)
	{}
	virtual ~MaxDistanceConstraint() {}

	virtual void solve() {

		Vector2f diff = (a->pos - b->pos);

		float current_distance = size(diff);


		if (current_distance > distance) {

			Vector2f normal = (diff / current_distance);


			float a_u = dot(a->vel, normal);
			float b_u = dot(b->vel, normal);

			if (a_u - b_u > 0.0f) {
				/*float a_v = ((a_u * (a->mass - b->mass)) + (2.0f * b->mass * b_u)) / (a->mass + b->mass);
				float b_v = ((b_u * (b->mass - a->mass)) + (2.0f * a->mass * a_u)) / (a->mass + b->mass);

				a->vel += (normal * (a_v - a_u));
				b->vel += (normal * (b_v - b_u));*/

				float vel = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);

				a->vel += (normal * (vel - a_u));
				b->vel += (normal * (vel - b_u));
			}
		}
	}


	virtual void draw(RenderWindow *window) {
		CircleShape shape(distance / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance / 2.0f, distance / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		window->draw(shape);
	}
};

class DistanceConstraint: public Constraint {

public:

	float distance;

	DistanceConstraint(Object *a, Object *b, float distanceParam = 100.0f):
		Constraint(a, b),
		distance(distanceParam)
	{}
	virtual ~DistanceConstraint() {}

	virtual void solve() {

		Vector2f diff = (a->pos - b->pos);

		float current_distance = size(diff);

		Vector2f normal = (diff / current_distance);

		const float error = 0.01f;

		if (!(current_distance / distance > (1.0f - error) && current_distance / distance < (1.0f + error))) {

			Vector2f center = (a->pos + b->pos) / 2.0f;
			a->pos = center + (normal * (distance / 2.0f));
			b->pos = center - (normal * (distance / 2.0f));
		}



		float a_u = dot(a->vel, normal);
		float b_u = dot(b->vel, normal);

		float vel = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);

		a->vel += (normal * (vel - a_u));
		b->vel += (normal * (vel - b_u));
	}


	virtual void draw(RenderWindow *window) {
		CircleShape shape(distance / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance / 2.0f, distance / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		window->draw(shape);
	}
};

class HaloConstraint: public Constraint {

public:

	float distance_inner, distance_outer;

	HaloConstraint(Object *a, Object *b, float distance_innerParam = 50.0f, float distance_outerParam = 100.0f):
		Constraint(a, b),
		distance_inner(distance_innerParam),
		distance_outer(distance_outerParam)
	{
		if (distance_inner > distance_outer) {
			float temp = distance_outer;
			distance_outer = distance_inner;
			distance_inner = temp;
		}
	}
	virtual ~HaloConstraint() {}

	virtual void solve() {

		Vector2f diff = (a->pos - b->pos);

		float current_distance = size(diff);

		bool need_correction = false;
		float intended_distance;

		const float error = 0.01f;

		if (current_distance < distance_inner) {
			need_correction = true;
			intended_distance = distance_inner * (1.0f + error);
		} else if (current_distance > distance_outer) {
			need_correction = true;
			intended_distance = distance_outer * (1.0f - error);
		}

		if (need_correction) {

			Vector2f normal = (diff / current_distance);

			Vector2f center = (a->pos + b->pos) / 2.0f;
			a->pos = center + (normal * (intended_distance / 2.0f));
			b->pos = center - (normal * (intended_distance / 2.0f));


			float a_u = dot(a->vel, normal);
			float b_u = dot(b->vel, normal);

			/*float a_v = ((a_u * (a->mass - b->mass)) + (2.0f * b->mass * b_u)) / (a->mass + b->mass);
			float b_v = ((b_u * (b->mass - a->mass)) + (2.0f * a->mass * a_u)) / (a->mass + b->mass);


			a->vel += (normal * (a_v - a_u));
			b->vel += (normal * (b_v - b_u));*/

			float vel = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);

			a->vel += (normal * (vel - a_u));
			b->vel += (normal * (vel - b_u));
		}
	}

	virtual void draw(RenderWindow *window) {
		CircleShape shape;

		shape.setRadius(distance_outer / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance_outer / 2.0f, distance_outer / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		window->draw(shape);


		shape.setRadius(distance_inner / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance_inner / 2.0f, distance_inner / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		window->draw(shape);
	}
};

#endif