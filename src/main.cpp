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


#include "Boll.hpp"
#include "Clumsy.hpp"
#include "Player.hpp"
#include "Battleground.hpp"



std::vector<Object *> *objects;
std::vector<Object *> *followedObjects;
std::vector<Constraint *> *constraints;

World *world;

std::vector<Player *> *players;
std::vector<Clumsy *> *clumsys;


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
	window->setFramerateLimit(50);









	sf::View game_view;
	sf::View gui_view;
	gui_view.setSize(Vector2f((float)window->getSize().x / (float)window->getSize().y, 1.0f));
	gui_view.setCenter(gui_view.getSize() / 2.0f);












	objects = new std::vector<Object *>();
	followedObjects = new std::vector<Object *>();


	constraints = new std::vector<Constraint *>();

	players = new std::vector<Player *>();
	clumsys = new std::vector<Clumsy *>();







	//world = new RectWorld(Vector2f(100.0f, 100.0f));
	//world = new ElasticCircleWorld(150.0f, 40.0f);
	world = new Battleground(200.0f, 40.0f);


	/*for (int i = 0; i < 2; i++) {
		players->push_back(new Player(Vector2f((i + 1), 0.0f), RANDOM_COLOR, -1));
		players->push_back(new Player(Vector2f(-(i + 1), 0.0f), RANDOM_COLOR, -2));
		players->push_back(new Player(Vector2f(0.0f, (i + 1)), RANDOM_COLOR, -3));
		players->push_back(new Player(Vector2f(0.0f, -(i + 1)), RANDOM_COLOR, -4));
	}*/

	players->push_back(new Player(Vector2f(25.0f, 0.0f), Vector2f(RANDOM2 * 100.0f, RANDOM2 * 100.0f), Color(200, 160, 80), -1));
	players->push_back(new Player(Vector2f(-25.0f, 0.0f), Vector2f(RANDOM2 * 100.0f, RANDOM2 * 100.0f), Color(255, 120, 230), -2));
	players->push_back(new Player(Vector2f(25.0f, 25.0f), Vector2f(RANDOM2 * 100.0f, RANDOM2 * 100.0f), Color(150, 100, 200), -3));
	//players->push_back(new Player(Vector2f(-25.0f, 25.0f), Vector2f(RANDOM2 * 100.0f, RANDOM2 * 100.0f), Color(40, 40, 40), -4));


	for (unsigned int i = 0; i < players->size(); i++) {
		objects->push_back(players->at(i));
		followedObjects->push_back(players->at(i));
	}

	Boll *boll = new Boll(Vector2f(0.0f, -25.0f), Color(200, 80, 160, 50));
	objects->push_back(boll);

	Clumsy *clumsy = new Clumsy(Vector2f(0.0f, 25.0f), Color(160, 200, 80), boll);
	objects->push_back(clumsy);
	followedObjects->push_back(clumsy);


	constraints->push_back(new ElasticDistanceConstraint(clumsy, boll, 120.0f, 4.0f));







	/*sf::Text text;
	text.setFont(font);
	text.setString("");
	text.setCharacterSize(64);
	text.setColor(sf::Color::Blue);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	text.setPosition(Vector2f(0,0));
	text.setScale(Vector2f(0.002,0.002));*/








	sf::Event event;
	sf::Clock clock;

	while (window->isOpen()) {

		float elapsedTime = clock.restart().asSeconds();

		while (window->pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: {
					window->close();
				} break;
				case sf::Event::Resized: {
					gui_view.setSize(Vector2f((float)event.size.width / (float)event.size.height, 1.0f));
					gui_view.setCenter(gui_view.getSize() / 2.0f);
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

												case sf::Keyboard::Return: {

															for (unsigned int i = 0; i < players->size(); i++) {
																players->at(i)->health = 1.0f;
															}
												} break;
						default: break;
					}
				} break;

				/*case sf::Event::TextEntered: {
					if (event.text.unicode < 128) {
						if (event.text.unicode == '\b') {
							sf::String str = text.getString();
							if (str.getSize() > 0) {
								str.erase(str.getSize() - 1);
								text.setString(str);
							}
						} else if (event.text.unicode == '\r') {
							text.move(0,0.1);
						} else {
							sf::String str(static_cast<char>(event.text.unicode));
							std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;

							text.setString(text.getString() + str);
						}
					} else {
						std::cout << "character typed: " << (char)(event.text.unicode) << std::endl;
					}
				} break;*/

				case sf::Event::JoystickConnected: {
					std::cout << "Joystick connected: " << event.joystickConnect.joystickId << std::endl;
				} break;
				case sf::Event::JoystickDisconnected: {
					std::cout << "Joystick disconnected: " << event.joystickConnect.joystickId << std::endl;
				} break;
				default: break;
			}
		}















		if (boll->connected) {
			if (RANDOM < 0.001) {

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



		/*Vector2f center_position(0.0f,0.0f);
		for (unsigned int i = 0; i < players->size(); i++) {
			center_position += players->at(i)->pos;
		}
		center_position /= (float)players->size();*/

		/*float largers_dist = 0.0f;
		for (unsigned int i = 0; i < players->size(); i++) {
			float dist = size(players->at(i)->pos - center_position);
			if (dist > largers_dist) {
				largers_dist = dist;
			}
		}

		float scale_multiply = 2.0f * (largers_dist + 20.0f);
		*/

		/*Vector2f outermost(0.0f, 0.0f);
		for (unsigned int i = 0; i < players->size(); i++) {
			Vector2f v = players->at(i)->pos - center_position;
			if (fabs(v.x) > outermost.x) {
				outermost.x = fabs(v.x);
			}
			if (fabs(v.y) > outermost.y) {
				outermost.y = fabs(v.y);
			}
		}

		Vector2f thing(outermost.x / ((float)window->getSize().x / (float)window->getSize().y), outermost.y);


		//float scale_multiply = 2.0f * ((thing.x > thing.y ? thing.x : thing.y) + 20.0f);
		float scale_multiply = 2.0f * (size(thing) + 20.0f);
		*/


		Vector2f smalest_most = followedObjects->at(0)->pos;
		Vector2f largest_most = followedObjects->at(0)->pos;
		for (unsigned int i = 1; i < followedObjects->size(); i++) {
			Vector2f v = followedObjects->at(i)->pos;
			if (v.x > largest_most.x) {
				largest_most.x = v.x;
			} else if (v.x < smalest_most.x) {
				smalest_most.x = v.x;
			}
			if (v.y > largest_most.y) {
				largest_most.y = v.y;
			} else if (v.y < smalest_most.y) {
				smalest_most.y = v.y;
			}
		}

		Vector2f center_position = (smalest_most + largest_most) / 2.0f;

		smalest_most.x /= ((float)window->getSize().x / (float)window->getSize().y);
		largest_most.x /= ((float)window->getSize().x / (float)window->getSize().y);


		float scale_multiply = size(smalest_most - largest_most) + 40.0f;

		if (scale_multiply < 200.0f) {
			scale_multiply = 200.0f;
		}
		Vector2f newSize = Vector2f((float)window->getSize().x / (float)window->getSize().y, 1.0f) * scale_multiply;
		Vector2f currentSize = game_view.getSize();
		Vector2f currentPosition = game_view.getCenter();
		game_view.setSize((newSize - currentSize) / 4.0f + currentSize);
		game_view.setCenter((center_position - currentPosition) / 4.0f + currentPosition);
		window->setView(game_view);

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
















		window->setView(gui_view);

		{

			Vector2f size(0.3, 0.03);
			float space = 0.02;

			Vector2f offset(0.02, 0.02);

			sf::RectangleShape healthBar(size);
			healthBar.setPosition(offset);

			Vector2f border(0.006f, 0.006f);
			sf::RectangleShape outline(size + border * 2.0f);
			outline.setPosition(offset - border);
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

		//window->draw(text);



		//Update window
		window->display();
	}











	delete world;

	// dont delete the objects i the player vector because they are also a part och the objects vector
	delete players;
	delete clumsys;

	delete followedObjects;

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
