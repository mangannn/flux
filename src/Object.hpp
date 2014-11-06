#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object {

public:

	Vector2f pos, vel;
	float mass, radius;

	CircleShape shape;

	Object(
		Vector2f posParam, Vector2f velParam, 
		float massParam, float radiusParam, 
		Color colorParam = Color(0, 0, 0)
		):

		pos(posParam),
		vel(velParam),
		mass(massParam),
		radius(radiusParam),
		shape(radiusParam)
	{
		shape.setFillColor(colorParam);
	}
	virtual ~Object() {}

	virtual void draw(RenderWindow *window) {
		shape.setPosition(pos - Vector2f(radius, radius));
		window->draw(shape);
	}
};

#endif