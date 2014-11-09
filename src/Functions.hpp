#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


float aabs(float a) {
	return (a < 0.0 ? -a : a);
}

float frac(float x) {
	return x - floor(x);
}

float size(sf::Vector2f v) {
	return sqrt((v.x * v.x) + (v.y * v.y));
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

float periodValueBetween(float angle, float target, float percent, float period = 360) {
	target = cutToPeriod(target - angle, - period / 2, period / 2);
	angle = cutToPeriod(angle, - period / 2, period / 2);
	return cutToPeriod(target * percent + angle, 0, period);
}



#define RANDOM_COLOR sf::Color(rand()%0xff,rand()%0xff,rand()%0xff)

#define RANDOM ((float)rand()/(float)RAND_MAX)
#define RANDOM2 ((RANDOM * 2.0f) - 1.0f)

#endif