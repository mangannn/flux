#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace sf;


float aabs(float a) {
	return (a < 0.0 ? -a : a);
}
float size(Vector2f v) {
	return sqrt((v.x * v.x) + (v.y * v.y));
}

float dot(Vector2f a, Vector2f b) {
	return (a.x * b.x) + (a.y * b.y);
}

float angle(Vector2f v) {
  if (v.x >= 0 && v.y < 0)
    return 90 - atan(aabs(v.y) / aabs(v.x)) / 2 / M_PI * 360;
  if (v.x >= 0 && v.y >= 0)
    return 90 + atan(aabs(v.y) / aabs(v.x)) / 2 / M_PI * 360;
  if (v.x < 0 && v.y >= 0)
    return 270 - atan(aabs(v.y) / aabs(v.x)) / 2 / M_PI * 360;
  if (v.x < 0 && v.y < 0)
    return 270 + atan(aabs(v.y) / aabs(v.x)) / 2 / M_PI * 360;
  return -1;
}

float cutToPeriod(float v, float periodStart, float periodEnd) {
	while (v >= periodEnd) v -= periodEnd - periodStart;
	while (v < periodStart) v += periodEnd - periodStart;
	return v;
}

float periodValueBetween(float angle, float target, float percent, float period = 360, bool defaultDirection = true) {
	target = cutToPeriod(target - angle, - period / 2, period / 2);
	angle = cutToPeriod(angle, - period / 2, period / 2);
	return cutToPeriod(target * percent + angle, 0, period);
}



#define RANDOM_COLOR sf::Color(rand()%0xff,rand()%0xff,rand()%0xff)

#define RANDOM ((float)rand()/(float)RAND_MAX)
#define RANDOM2 ((RANDOM * 2.0f) - 1.0f)






#include "Object.hpp"
#include "Player.hpp"
#include "Big.hpp"

#include "Constraint.hpp"



#define WORLD_SCALE 208.0f
#define WORLD_SIZE 100.0f




vector<Object *> *objects;
vector<Constraint *> *constraints;

#include "Physics.hpp"


int main() {

	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 3;

	sf::RenderWindow *window;
	bool fullscreen = false;

	window = new RenderWindow(VideoMode(800, 620), "FLUX", sf::Style::Resize, settings);
	window->setMouseCursorVisible(true);


	sf::View view;
	view.setCenter(Vector2f(0.0f, 0.0f));
	view.setSize(Vector2f((float)window->getSize().x / (float)window->getSize().y, 1.0f) * WORLD_SCALE);
	window->setView(view);


	window->setFramerateLimit(50);

	sf::Clock clock;


	objects = new vector<Object *>();
	constraints = new vector<Constraint *>();



	Vector2f center_of_world = Vector2f(0.0f, 0.0f);
	CircleShape world_shape(WORLD_SIZE);
	world_shape.setPosition(center_of_world - Vector2f(WORLD_SIZE, WORLD_SIZE));
	world_shape.setFillColor(Color(0xff,0xff,0xff));

	world_shape.setOutlineThickness(0.6f);
	world_shape.setOutlineColor(Color(0, 0, 0));


	Player *p1 = new Player(Vector2f(0.0f, 0.0f), Color(200, 160, 80), -1);
	Player *p2 = new Player(Vector2f(1.0f, 0.0f), Color(0, 100, 200), -2);

	Big *big = new Big(Vector2f(0.0f, 1.0f), Color(160, 200, 80));

	objects->push_back(p1);
	objects->push_back(p2);
	objects->push_back(big);

	constraints->push_back(new MaxDistanceConstraint(p1, big, 60));
	constraints->push_back(new MaxDistanceConstraint(p2, big, 60));

	//objects->push_back(new Object(Vector2f(200, 200), Vector2f(0,0), 10, 25, Color(0,200,100)));
	//constraints->push_back(new DistanceConstraint(objects->at(0), objects->at(1), 100));


	/*for (int i = 0; i < 20; i++) {
		objects->push_back(new Object(
			Vector2f(RANDOM2 * WORLD_SIZE, RANDOM2 * WORLD_SIZE), Vector2f(0,0),
			4, 2,
			RANDOM_COLOR));
		//constraints->push_back(new HaloConstraint(objects->at(0), objects->back(), 50, 100));
		constraints->push_back(new DistanceConstraint(objects->at(0), objects->back(), 20));
	}*/


	sf::Event event;

	while (window->isOpen()) {

		float elapsedTime = clock.restart().asSeconds();

		while (window->pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: {
					window->close();
				} break;
				case sf::Event::Resized: {
					view.setSize(Vector2f((float)event.size.width / (float)event.size.height, 1.0f) * WORLD_SCALE);
					window->setView(view);
				} break;
				case sf::Event::KeyPressed: {

					switch (event.key.code) {
						case sf::Keyboard::Escape: {
							window->close();
						} break;
						case sf::Keyboard::F2:
						case sf::Keyboard::F3:
						case sf::Keyboard::F4:
						case sf::Keyboard::F5:
						case sf::Keyboard::F6:
						case sf::Keyboard::F7:
						case sf::Keyboard::F8:
						case sf::Keyboard::F9:
						case sf::Keyboard::F10:
						case sf::Keyboard::F11:
						case sf::Keyboard::F12:
						{

							window->close();
							delete window;

							if (fullscreen) {
								window = new RenderWindow(VideoMode(800, 620), "FLUX", sf::Style::Resize, settings);
							} else {
								window = new RenderWindow(VideoMode(), "FLUX", sf::Style::Fullscreen, settings);
							}

							window->setMouseCursorVisible(!fullscreen);
							view.setSize(Vector2f((float)window->getSize().x / (float)window->getSize().y, 1.0f) * WORLD_SCALE);
							window->setView(view);


							fullscreen = !fullscreen;

						} break;

						case sf::Keyboard::Space: {
						} break;
						default: break;
					}

				} break;
				default: break;
			}
		}

		p1->handleInput(elapsedTime);
		p2->handleInput(elapsedTime);

		p1->update(elapsedTime);
		p2->update(elapsedTime);

		big->update(elapsedTime);


		step(elapsedTime);


		window->clear(sf::Color(100, 200, 100));

		window->draw(world_shape);

		/*for (unsigned int i = 0; i < constraints->size(); i++) {
			constraints->at(i)->draw(window);
		}*/

		for (unsigned int i = 0; i < objects->size(); i++) {
			objects->at(i)->draw(window);
		}

		//Update window
		window->display();
	}




	{
		Object *temp;
		while (!objects->empty()) {
			temp = objects->back();
			delete temp;
			objects->pop_back();
		}
		delete objects;
	}

	{
		Constraint *temp;
		while (!constraints->empty()) {
			temp = constraints->back();
			delete temp;
			constraints->pop_back();
		}
		delete constraints;
	}

	delete window;

	return 0;
}
