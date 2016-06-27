#ifndef _WORLD_H_
#define _WORLD_H_

#include "Object.hpp"

class World {

public:

	World() {}
	virtual ~World() {}

	virtual void collision(Object *a, float elapsedTime) = 0;

	virtual void draw(RenderTarget *target) = 0;
};

class RectWorld : public World {

public:

	Vector2f size;
	RectangleShape shape;

	RectWorld(Vector2f sizeParam):
		size(sizeParam),
		shape(sizeParam * 2.0f)
	{
		shape.setPosition(-size);
		shape.setFillColor(Color(0xff,0xff,0xff));

		shape.setOutlineThickness(0.6f);
		shape.setOutlineColor(Color(0, 0, 0));
	}
	virtual ~RectWorld() {}

	virtual void collision(Object *a, float elapsedTime) {

		Vector2f next_a = a->pos + (a->vel * elapsedTime);

		const float margin = 0.1f;

		if (next_a.x - a->radius < -size.x) {
			a->vel.x = -a->vel.x;
			a->pos.x = -(size.x - (a->radius + margin));
		} else if (next_a.x + a->radius > size.x) {
			a->vel.x = -a->vel.x;
			a->pos.x = (size.x - (a->radius + margin));
		}

		if (next_a.y - a->radius < -size.y) {
			a->vel.y = -a->vel.y;
			a->pos.y = -(size.y - (a->radius + margin));
		} else if (next_a.y + a->radius > size.y) {
			a->vel.y = -a->vel.y;
			a->pos.y = (size.y - (a->radius + margin));
		}
	}

	virtual void draw(RenderTarget *target) {
		target->draw(shape);
	}
};


class CircleWorld : public World {

public:

	float radius;
	CircleShape shape;

	CircleWorld(float radiusParam):
		radius(radiusParam),
		shape(radiusParam)
	{
		shape.setPosition(-Vector2f(radius, radius));
		shape.setFillColor(Color(0xff,0xff,0xff));

		shape.setOutlineThickness(0.6f);
		shape.setOutlineColor(Color(0, 0, 0));
	}
	virtual ~CircleWorld() {}

	virtual void collision(Object *a, float elapsedTime) {

		Vector2f next_a = a->pos + (a->vel * elapsedTime);
		Vector2f difference = next_a;
		float current_distance = size(difference);

		const float margin = 0.1f;

		if (current_distance > radius - a->radius) {

			Vector2f normal = (difference / current_distance);

			a->vel -= normal * (2.0f * dot(a->vel, normal));
			a->pos = normal * (radius - (a->radius + margin));
		}
	}

	virtual void draw(RenderTarget *target) {
		target->draw(shape);
	}
};

class ElasticCircleWorld : public World {

public:

	float radius, elasticity;
	CircleShape shape;

	ElasticCircleWorld(float radiusParam, float elasticityParam = 0.0005f):
		radius(radiusParam),
		elasticity(elasticityParam),
		shape(radiusParam)
	{
		shape.setPosition(-Vector2f(radius, radius));
		shape.setFillColor(Color(0xff,0xff,0xff));

		shape.setOutlineThickness(0.6f);
		shape.setOutlineColor(Color(0, 0, 0));

		shape.setPointCount(100);
	}
	virtual ~ElasticCircleWorld() {}

	virtual void collision(Object *a, float elapsedTime) {

		Vector2f next_a = a->pos + (a->vel * elapsedTime);
		Vector2f difference = next_a;
		float current_distance = size(difference);

		if (current_distance > radius) {
			Vector2f normal = (difference / current_distance);
			a->vel -= normal * (((current_distance - radius) * elasticity) * a->mass);
		}
	}

	virtual void draw(RenderTarget *target) {
		target->draw(shape);
	}
};

#endif
