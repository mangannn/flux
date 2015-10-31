#include <math.h>

#include <SFML/System.hpp>
#include <iostream>

using namespace sf;

float frac(float x) {
	return x - floor(x);
}

float size(sf::Vector2f v) {
	return sqrt((v.x * v.x) + (v.y * v.y));
}
float sqrSize(sf::Vector2f v) {
	return (v.x * v.x) + (v.y * v.y);
}

float dot(sf::Vector2f a, sf::Vector2f b) {
	return (a.x * b.x) + (a.y * b.y);
}

float angle(sf::Vector2f v) {
	return (atan2(v.y, v.x) * 180.0f) / M_PI;
}

float cutToPeriod(float v, float periodStart, float periodEnd) {
	while (v >= periodEnd) {v -= periodEnd - periodStart;}
	while (v < periodStart) {v += periodEnd - periodStart;}
	return v;
}

float periodValueBetween(float angle, float target, float percent, float period) {
	target = cutToPeriod(target - angle, - period / 2, period / 2);
	angle = cutToPeriod(angle, - period / 2, period / 2);
	return cutToPeriod(target * percent + angle, 0, period);
}

bool lineIntersection(Vector2f a1, Vector2f a2, Vector2f b1, Vector2f b2) {

	Vector2f av = a1 - a2;
	Vector2f bv = b1 - b2;

	if (av.x == 0.0f || bv.x == 0.0f) {
		a1 = Vector2f(a1.y, a1.x);
		a2 = Vector2f(a2.y, a2.x);
		b1 = Vector2f(b1.y, b1.x);
		b2 = Vector2f(b2.y, b2.x);

		av = a1 - a2;
		bv = b1 - b2;
	}

	if (av.x != 0.0f && bv.x != 0.0f) {

		float ak = av.y / av.x;
		float bk = bv.y / bv.x;

		if (ak != bk) {

			float am = a1.y - (ak * a1.x);
			float bm = b1.y - (bk * b1.x);

			float intersection = (am - bm) / (bk - ak);

			float maxI, minI;

			if (a1.x < a2.x) {
				maxI = a2.x;
				minI = a1.x;
			} else {
				maxI = a1.x;
				minI = a2.x;
			}

			if (b1.x < b2.x) {
				maxI = fmin(maxI, b2.x);
				minI = fmax(minI, b1.x);
			} else {
				maxI = fmin(maxI, b1.x);
				minI = fmax(minI, b2.x);
			}

			if (intersection >= minI && intersection <= maxI) {
				return true;
			}
		}
	}

	return false;
}


void lineIntersectionDebug() {

	std::cout << "true" << std::endl;

	std::cout << "intersection: " << lineIntersection(
		Vector2f(1,0), Vector2f(-1,0),
		Vector2f(0,1), Vector2f(0,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(-1,0), Vector2f(1,0),
		Vector2f(0,1), Vector2f(0,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(0,-1),
		Vector2f(1,0), Vector2f(-1,0)
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(0,-1), 
		Vector2f(-1,0), Vector2f(1,0)
		) << std::endl;

	std::cout << "intersection: " << lineIntersection(
		Vector2f(1,0), Vector2f(-1,0),
		Vector2f(0,1), Vector2f(1,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(-1,0), Vector2f(1,0),
		Vector2f(0,1), Vector2f(1,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(1,-1),
		Vector2f(1,0), Vector2f(-1,0)
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(1,-1), 
		Vector2f(-1,0), Vector2f(1,0)
		) << std::endl;

	std::cout << "intersection: " << lineIntersection(
		Vector2f(1,0), Vector2f(-1,1),
		Vector2f(0,1), Vector2f(0,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(-1,0), Vector2f(1,1),
		Vector2f(0,1), Vector2f(0,-1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(0,-1),
		Vector2f(1,0), Vector2f(-1,1)
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(0,-1), 
		Vector2f(-1,0), Vector2f(1,1)
		) << std::endl;

	std::cout << "intersection: " << lineIntersection(
		Vector2f(1,0), Vector2f(0,1),
		Vector2f(0,0), Vector2f(1,1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,1), Vector2f(1,0),
		Vector2f(0,0), Vector2f(1,1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,0), Vector2f(1,1), 
		Vector2f(1,0), Vector2f(0,1)
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,0), Vector2f(1,1), 
		Vector2f(0,1), Vector2f(1,0)
		) << std::endl;


	std::cout << "false" << std::endl;

	std::cout << "intersection: " << lineIntersection(
		Vector2f(2,3), Vector2f(3,2),
		Vector2f(0,0), Vector2f(1,1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(3,2), Vector2f(2,3),
		Vector2f(0,0), Vector2f(1,1) 
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,0), Vector2f(1,1),
		Vector2f(2,3), Vector2f(3,2)
		) << std::endl;
	std::cout << "intersection: " << lineIntersection(
		Vector2f(0,0), Vector2f(1,1),
		Vector2f(3,2), Vector2f(2,3)
		) << std::endl;		
}
