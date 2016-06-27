#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include "Object.hpp"
#include "../Functions.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

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

	virtual void draw(RenderTarget *target) {};
};

class PointConstraint: public Constraint {

public:

	PointConstraint(Object *a, Object *b):
		Constraint(a, b)
	{}
	virtual ~PointConstraint() {}

	virtual void solve() {

		Vector2f diffrence = (a->pos - b->pos);
		float current_distance = size(diffrence);

		if (current_distance > 0.0) {
			Vector2f normal = (diffrence / current_distance);

			float a_u = dot(a->vel, normal);
			float b_u = dot(b->vel, normal);

			// inelastic collision
			float a_v = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);
			float b_v = a_v;

			a->vel += (normal * (a_v - a_u));
			b->vel += (normal * (b_v - b_u));

			Vector2f position = (a->pos + b->pos) / 2.0f;

			a->pos = position;
			b->pos = position;
		}
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

		Vector2f diffrence = (a->pos - b->pos);

		float current_distance = size(diffrence);

		Vector2f normal = (diffrence / current_distance);

		const float error = 0.01f;

		if (!(current_distance / distance > (1.0f - error) && current_distance / distance < (1.0f + error))) {

			Vector2f center = (a->pos + b->pos) / 2.0f;
			a->pos = center + (normal * (distance / 2.0f));
			b->pos = center - (normal * (distance / 2.0f));
		}



		float a_u = dot(a->vel, normal);
		float b_u = dot(b->vel, normal);

		// inelastic collision
		float a_v = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);
		float b_v = a_v;

		a->vel += (normal * (a_v - a_u));
		b->vel += (normal * (b_v - b_u));
	}


	virtual void draw(RenderTarget *target) {
		CircleShape shape(distance / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance / 2.0f, distance / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		target->draw(shape);
	}
};

class ElasticDistanceConstraint: public Constraint {

public:

	float distance, elasticity;

	ElasticDistanceConstraint(Object *a, Object *b, float distanceParam = 100.0f, float elasticityParam = 5.0f):
		Constraint(a, b),
		distance(distanceParam),
		elasticity(elasticityParam)
	{}
	virtual ~ElasticDistanceConstraint() {}

	virtual void solve() {

		Vector2f diffrence = (a->pos - b->pos);

		float current_distance = size(diffrence);


		if (current_distance > distance) {

			Vector2f normal = (diffrence / current_distance);

			a->vel -= (normal * ((current_distance - distance) * elasticity) * (b->mass / (a->mass + b->mass)));
			b->vel += (normal * ((current_distance - distance) * elasticity) * (a->mass / (a->mass + b->mass)));
		}
	}


	virtual void draw(RenderTarget *target) {
		CircleShape shape(distance / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance / 2.0f, distance / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		target->draw(shape);
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

		Vector2f diffrence = (a->pos - b->pos);

		float current_distance = size(diffrence);

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

			Vector2f normal = (diffrence / current_distance);

			Vector2f center = (a->pos + b->pos) / 2.0f;
			a->pos = center + (normal * (intended_distance / 2.0f));
			b->pos = center - (normal * (intended_distance / 2.0f));


			float a_u = dot(a->vel, normal);
			float b_u = dot(b->vel, normal);

			/*// elastic collision
			float a_v = ((a_u * (a->mass - b->mass)) + (2.0f * b->mass * b_u)) / (a->mass + b->mass);
			float b_v = ((b_u * (b->mass - a->mass)) + (2.0f * a->mass * a_u)) / (a->mass + b->mass);*/

			// inelastic collision
			float a_v = ((a_u * a->mass) + (b_u * b->mass)) / (a->mass + b->mass);
			float b_v = a_v;

			a->vel += (normal * (a_v - a_u));
			b->vel += (normal * (b_v - b_u));
		}
	}

	virtual void draw(RenderTarget *target) {
		CircleShape shape;

		shape.setRadius(distance_outer / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance_outer / 2.0f, distance_outer / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		target->draw(shape);


		shape.setRadius(distance_inner / 2.0f);
		shape.setPosition(((a->pos + b->pos) / 2.0f) - Vector2f(distance_inner / 2.0f, distance_inner / 2.0f));
		shape.setFillColor(Color(0,0,0,0));

		shape.setOutlineThickness(1);
		shape.setOutlineColor(Color(0xff, 0, 0));

		target->draw(shape);
	}
};

#endif