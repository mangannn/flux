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
float semiCross(sf::Vector2f a, sf::Vector2f b) {
	return (a.x * b.y) - (a.y * b.x);
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
	angle = cutToPeriod(angle, - period / 2, period / 2);
	float diffrence = cutToPeriod(target - angle, - period / 2, period / 2);
	return cutToPeriod(diffrence * percent + angle, 0, period);
}

bool lineIntersect(Vector2f a1, Vector2f a2, Vector2f b1, Vector2f b2) {
	
	Vector2f p = a1, r = a2 - a1, q = b1, s = b2 - b1;

	float n = semiCross(r, s);

	if (n != 0) {

		float l = semiCross(q - p, r) / n;
		float u = semiCross(q - p, s) / n;

		if (fmin(l, u) >= 0 && fmax(l, u) <= 1) {
			return true;
		}
	} else if (semiCross(q - p, r) == 0) {
		// rare case when lines are parallel and might be intersecting

		Vector2f rd = r / sqrSize(r);

		float t0 = dot((q - p), rd);
		float t1 = t0 + dot(s, rd);

		if (fmax(t0, t1) >= 0 && fmin(t0, t1) <= 1) {
			return true;
		}
	}

	return false;
}


void lineIntersectDebug() {

	std::cout << "true" << std::endl;

	float t[] = {
		1,0, -1,0, 0,1, 0,-1,
		-1,0, 1,0, 0,1, 0,-1,
		0,1, 0,-1, 1,0, -1,0,
		0,1, 0,-1, -1,0, 1,0,
		1,0, -1,0, 0,1, 1,-1,
		-1,0, 1,0, 0,1, 1,-1,
		0,1, 1,-1, 1,0, -1,0,
		0,1, 1,-1, -1,0, 1,0,
		1,0, -1,1, 0,1, 0,-1,
		-1,0, 1,1, 0,1, 0,-1,
		0,1, 0,-1, 1,0, -1,1,
		0,1, 0,-1, -1,0, 1,1,
		1,0, 0,1, 0,0, 1,1,
		0,1, 1,0, 0,0, 1,1,
		0,0, 1,1, 1,0, 0,1,
		0,0, 1,1, 0,1, 1,0
	};

	for (int i = 0; i < 16; i++) {

		std::cout << "intersection: " << lineIntersect(
			Vector2f(t[8 * i + 0],t[8 * i + 1]), Vector2f(t[8 * i + 2],t[8 * i + 3]),
			Vector2f(t[8 * i + 4],t[8 * i + 5]), Vector2f(t[8 * i + 6],t[8 * i + 7]) 
			) << std::endl;
	}

	std::cout << "false" << std::endl;

	float f[] = {
		2, 3, 3, 2, 0, 0, 1, 1,
		3, 2, 2, 3, 0, 0, 1, 1,
		0, 0, 1, 1, 2, 3, 3, 2, 
		0, 0, 1, 1, 3, 2, 2, 3, 
		0.967674, -29.9809, -3.08349, -30.4089, 0, 80, 0, 0,
		-146.176, -97.4457, -146.176, -97.4457, -147.019, -2.70507, 10.7836, 33.6048,
		0, 25, 0, 25, 80, 0, 0, 0,
		-71.7272, -68.7826, -71.7272, -68.7826, -74.2051, -3.98992, -66.5042, 93.0426,
		-71.7272, -68.7826, -71.7272, -68.7826, -70.0826, 123.088, -115.216, 3.1257,
		4.59559, -62.3794, 4.59559, -62.3794, 41.6498, -133.101, -4.59137, -98.1135,

		-29.9809, 0.967674, -30.4089, -3.08349, 80, 0, 0, 0, 
		-97.4457, -146.176, -97.4457, -146.176, -2.70507, -147.019, 33.6048, 10.7836, 
		25, 0, 25, 0, 0, 80, 0, 0, 
		-68.7826, -71.7272, -68.7826, -71.7272, -3.98992, -74.2051, 93.0426, -66.5042, 
		-68.7826, -71.7272, -68.7826, -71.7272, 123.088, -70.0826, 3.1257, -115.216, 
		-62.3794, 4.59559, -62.3794, 4.59559, -133.101, 41.6498, -98.1135, -4.59137,

		0, 25, 0, 24.7172, 80, 0, 0, 0,
		25, 0, 24.7172, 0, 0, 80, 0, 0
	};

	for (int i = 0; i < 18; i++) {

		std::cout << "intersection: " << lineIntersect(
			Vector2f(f[8 * i + 0],f[8 * i + 1]), Vector2f(f[8 * i + 2],f[8 * i + 3]),
			Vector2f(f[8 * i + 4],f[8 * i + 5]), Vector2f(f[8 * i + 6],f[8 * i + 7]) 
			) << std::endl;
		
	}

}
