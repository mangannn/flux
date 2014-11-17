#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

float frac(float x);

float size(sf::Vector2f v);
float sqrSize(sf::Vector2f v);

float dot(sf::Vector2f a, sf::Vector2f b);

float angle(sf::Vector2f v);

float cutToPeriod(float v, float periodStart, float periodEnd);
float periodValueBetween(float angle, float target, float percent, float period = 3600.f);

bool lineCollision(Vector2f a, Vector2f b, Vector2f c, Vector2f d);
bool dashedIntersection(Vector2f a, Vector2f b);

#define RANDOM_COLOR sf::Color(rand()%0x100,rand()%0x100,rand()%0x100)

#define RANDOM ((float)rand()/(float)RAND_MAX)
#define RANDOM2 ((RANDOM * 2.0f) - 1.0f)

#endif
