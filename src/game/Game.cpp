#include <math.h>

#include "../CharacterSelect.hpp"

#include "../Functions.hpp"
#include "../Resources.hpp"

#include "Boll.hpp"
#include "Clumsy.hpp"
#include "Player.hpp"
#include "Battleground.hpp"

#include "Game.hpp"


Game::Game(std::vector<Player *> *playersParam):
	players(playersParam)

{

	objects = new std::vector<Object *>();
	constraints = new std::vector<Constraint *>();

	//world = new RectWorld(Vector2f(100.0f, 100.0f));
	world = new Battleground(200.0f, 40.0f);


	followedObjects = new std::vector<Object *>();


	for (unsigned int i = 0; i < players->size(); i++) {
		objects->push_back(players->at(i));
		followedObjects->push_back(players->at(i));
	}

	boll = new Boll(Vector2f(0.0f, 0.0f), Color(200, 80, 160, 50));
	objects->push_back(boll);

	clumsy = new Clumsy(Vector2f(0.0f, 80.0f), Color(160, 200, 80), boll);
	objects->push_back(clumsy);
	followedObjects->push_back(clumsy);


	constraints->push_back(new ElasticDistanceConstraint(clumsy, boll, 120.0f, 4.0f));


}
Game::~Game() {

	delete world;

	// dont delete the objects i the player vector because they are also a part och the objects vector
	delete players;

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
}






EventPass *Game::eventHandle(sf::Event event) {

	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->controls->eventHandle(event);
	}

	switch (event.type) {
		case sf::Event::KeyPressed: {

			switch (event.key.code) {
				case sf::Keyboard::Return: {
					return new CharacterSelect();
				} break;
				default: break;
			}
		} break;
		default: break;
	}

	return NULL;
}







EventPass *Game::update(float elapsedTime) {
	for (unsigned int i = 0; i < players->size(); i++) {
		players->at(i)->handleInput(elapsedTime);
	}

	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->update(elapsedTime);
	}

	step(elapsedTime);



	if (boll->connected) {
		Constraint *c = boll->constraint;
		Player *pl;

		if (c->a == boll) {
			pl = dynamic_cast<Player *>(c->b);
		} else {
			pl = dynamic_cast<Player *>(c->a);
		}

		for (unsigned int i = 0; i < players->size(); i++) {
			if (players->at(i) != pl) {
			//if (sqrSize(players->at(i)->dashVel) > 0.0f) {
				if (lineIntersect(players->at(i)->dashPos, players->at(i)->pos, clumsy->pos, boll->pos)) {

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

					boll->vel += normal * 10.0f;
					boll->pos += normal * (o->radius + boll->radius);

					delete c;
					boll->connected = false;
					
				}
			//}
			}
		}
	}

	return NULL;
}





bool Game::collision_callback(Object *a, Object *b) {

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







void Game::draw(RenderWindow *window) {

	window->clear(sf::Color(100, 200, 100));
	//window->clear(sf::Color(0xff, 0xff, 0xff));


	float aspect = ((float)window->getSize().x / (float)window->getSize().y);


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

	smalest_most.x /= aspect;
	largest_most.x /= aspect;


	float scale_multiply = size(smalest_most - largest_most) + 40.0f;

	if (scale_multiply < 200.0f) {
		scale_multiply = 200.0f;
	}

	Vector2f newPosition = (smalest_most + largest_most) / 2.0f;
	Vector2f newSize = Vector2f(aspect, 1.0f) * scale_multiply;

	Vector2f currentSize = game_view.getSize();
	Vector2f currentPosition = game_view.getCenter();

	game_view.setSize((newSize - currentSize) / 4.0f + currentSize);
	game_view.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);
	
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















	gui_view.setSize(Vector2f(aspect, 1.0f));
	gui_view.setCenter(gui_view.getSize() / 2.0f);
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
}

