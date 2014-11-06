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


int main(int argc, char* argv[]) {

	srand(time(NULL));

	RenderWindow *window;

	if (argc > 1 && argv[1][0] == '1') {
		window = new RenderWindow(VideoMode(), "FLUX", sf::Style::Fullscreen);
		window->setMouseCursorVisible(false);
	} else {
		window = new RenderWindow(VideoMode(800, 620), "FLUX", sf::Style::Resize);
	}

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


	Player *p1 = new Player(Vector2f(0.0f, 0.0f), Color(200, 100, 0), -1);
	Big *big = new Big(Vector2f(0.0f, 1.0f));

	objects->push_back(p1);
	objects->push_back(big);

	constraints->push_back(new MaxDistanceConstraint(p1, big, 60));

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
						case sf::Keyboard::F1: {
							cout << "suppose to toggle fullscreen" << endl;
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

		p1->update(elapsedTime);
		big->update(elapsedTime);


		step(elapsedTime);


		window->clear(Color(100,200,100));

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
