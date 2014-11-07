#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object {

public:

	Vector2f pos, vel;
	float radius, mass, friction;

	CircleShape shape;

	Object(
		Vector2f posParam, Vector2f velParam, 
		float radiusParam, 
		float massParam, 
		float frictionParam = 50.0f,
		Color colorParam = Color(0, 0, 0)
		):

		pos(posParam),
		vel(velParam),

		radius(radiusParam),
		mass(massParam),
		friction(frictionParam),

		shape(radiusParam)
	{
		shape.setFillColor(colorParam);
	}
	virtual ~Object() {}

	virtual void update(float elapsedTime) {}

	virtual void draw(RenderWindow *window) {
		shape.setPosition(pos - Vector2f(radius, radius));
		window->draw(shape);
	}
};

#endif