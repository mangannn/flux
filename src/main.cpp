#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace sf;
using namespace std;



#include "Functions.hpp"

#include "Resources.hpp"

#include "Physics.hpp"


#include "Battleground.hpp"
#include "Player.hpp"
#include "Clumsy.hpp"
#include "Boll.hpp"





#define WORLD_SCALE 210.0f

std::vector<Object *> *objects;
std::vector<Constraint *> *constraints;

World *world;

std::vector<Player *> *players;



bool collision_callback(Object *a, Object *b) {

	Boll *bo;
	Player *pl;

	Constraint *c;

	if ((bo = dynamic_cast<Boll *>(a))) {
		if ((pl = dynamic_cast<Player *>(b))) {
			if (!bo->connected) {
				constraints->push_back(c = new PointConstraint(pl, bo));
				bo->constraint = c;
				bo->connected = true;
			}
			return false;
		}
	} else if ((bo = dynamic_cast<Boll *>(b))) {
		if ((pl = dynamic_cast<Player *>(a))) {
			if (!bo->connected) {
				constraints->push_back(c = new PointConstraint(pl, bo));
				bo->constraint = c;
				bo->connected = true;
			}
			return false;
		}
	}
	return true;
}


int main() {

	if (!load_resources()) {
		return 1;
	}


	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 5;

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

	objects = new std::vector<Object *>();
	constraints = new std::vector<Constraint *>();

	players = new std::vector<Player *>();


	//world = new RectWorld(Vector2f(100.0f, 100.0f));
	//world = new ElasticCircleWorld(150.0f, 40.0f);
	world = new Battleground(100.0f, 40.0f);


	for (int i = 0; i < 2; i++) {
		players->push_back(new Player(Vector2f((i + 1), 0.0f), RANDOM_COLOR, -1));
		players->push_back(new Player(Vector2f(-(i + 1), 0.0f), RANDOM_COLOR, -2));
		players->push_back(new Player(Vector2f(0.0f, (i + 1)), RANDOM_COLOR, -3));
		players->push_back(new Player(Vector2f(0.0f, -(i + 1)), RANDOM_COLOR, -4));
	}
	//players->push_back(new Player(Vector2f(25.0f, 0.0f), Color(200, 160, 80), -1));
	//players->push_back(new Player(Vector2f(-25.0f, 0.0f), Color(0, 100, 200), -2));
	//players->push_back(new Player(Vector2f(25.0f, 25.0f), Color(150, 100, 200), -3));
	//players->push_back(new Player(Vector2f(-25.0f, 25.0f), Color(40, 40, 40), -4));

	for (unsigned int i = 0; i < players->size(); i++) {
		objects->push_back(players->at(i));
	}

	Clumsy *clumsy = new Clumsy(Vector2f(0.0f, 25.0f), Color(160, 200, 80));
	Boll *boll = new Boll(Vector2f(0.0f, -25.0f), Color(200, 80, 160, 50), clumsy);
	objects->push_back(clumsy);
	objects->push_back(boll);

	constraints->push_back(new ElasticDistanceConstraint(clumsy, boll, 60.0f, 4.0f));


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
						case sf::Keyboard::F11:
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
							if (boll->connected) {
								Constraint *c = boll->constraint;

								Object *o;

								if (c->a == boll) {
									o = c->b;
								} else {
									o = c->a;
								}

								for (unsigned int i = 0; i < constraints->size(); i++) {
									if (c == constraints->at(i)) {
										constraints->erase(constraints->begin() + i);
									}
								}


								Vector2f diffrence = (clumsy->pos - boll->pos);
								Vector2f normal = (diffrence / size(diffrence));

								boll->vel += normal * 200.0f;
								boll->pos += normal * (o->radius + boll->radius);

								delete c;
								boll->connected = false;
							}
						} break;
						default: break;
					}

				} break;
				default: break;
			}
		}



		for (unsigned int i = 0; i < players->size(); i++) {
			players->at(i)->handleInput(elapsedTime);
		}

		for (unsigned int i = 0; i < objects->size(); i++) {
			objects->at(i)->update(elapsedTime);
		}


		step(elapsedTime);


		window->clear(sf::Color(100, 200, 100));
		//window->clear(sf::Color(0xff, 0xff, 0xff));

		world->draw(window);

		{
			Vector2f diffrence = (boll->pos - clumsy->pos);

			sf::RectangleShape line(sf::Vector2f(size(diffrence), 1));
			line.rotate(angle(diffrence));
			line.setPosition(clumsy->pos);
			line.setFillColor(sf::Color(0, 0, 0));

			window->draw(line);
		}

		/*for (unsigned int i = 0; i < constraints->size(); i++) {
			constraints->at(i)->draw(window);
		}*/

		for (unsigned int i = 0; i < objects->size(); i++) {
			objects->at(i)->draw(window);
		}

		{

			Vector2f size(50, 5);
			float space = 5;

			Vector2f offset(10, 10);

			sf::RectangleShape healthBar(size);
			healthBar.setPosition(Vector2f(-WORLD_SCALE / 2.0f, -WORLD_SCALE / 2.0f) + offset);

			Vector2f border(1.0f, 1.0f);
			sf::RectangleShape outline(size + border * 2.0f);
			outline.setPosition(Vector2f(-WORLD_SCALE / 2.0f, -WORLD_SCALE / 2.0f) + offset - border);
			outline.setFillColor(Color(0, 0, 0, 100));

			for (unsigned int i = 0; i < players->size(); i++) {

				window->draw(outline);

				if (players->at(i)->health > 0.0f) {

					Color color(players->at(i)->sprite.getColor());
					color.a = 200;
					healthBar.setFillColor(color);
					healthBar.setScale(Vector2f(players->at(i)->health, 1.0f));
					window->draw(healthBar);
				}

				outline.move(Vector2f(0, size.y + space));
				healthBar.move(Vector2f(0, size.y + space));
			}
		}

		//Update window
		window->display();
	}



	delete world;

	// dont delete the objects i the player vector because they are also a part och the objects vector
	delete players;

	{
		Constraint *temp;
		while (!constraints->empty()) {
			temp = constraints->back();
			delete temp;
			constraints->pop_back();
		}
		delete constraints;
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

	delete window;

	return 0;
}
