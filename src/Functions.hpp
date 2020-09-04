#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <SFML/System.hpp>

using namespace sf;

float frac(float x);

float size(sf::Vector2f v);
float sqrSize(sf::Vector2f v);

float dot(sf::Vector2f a, sf::Vector2f b);

float angle(sf::Vector2f v);

float cutToPeriod(float v, float periodStart, float periodEnd);
float periodValueBetween(float angle, float target, float percent, float period = 360.f);

bool lineIntersect(Vector2f a, Vector2f b, Vector2f c, Vector2f d);
bool lineIntersectDebug();

#define RANDOM_COLOR sf::Color(rand()%0x100,rand()%0x100,rand()%0x100)

#define RANDOM ((float)rand()/(float)RAND_MAX)
#define RANDOM2 ((RANDOM * 2.0f) - 1.0f)
#define RANDOM_VECTOR2F (Vector2f(RANDOM2, RANDOM2))

#endif
