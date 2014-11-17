#include <math.h>

#include <SFML/System.hpp>

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

bool lineCollision(Vector2f a, Vector2f b, Vector2f c, Vector2f d) {
	return true;
}

bool dashedIntersection(Vector2f a, Vector2f b) {
	return true;
}