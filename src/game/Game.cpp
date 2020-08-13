#include <math.h>

#include "../CharacterSelect.hpp"

#include "../Functions.hpp"

#include "Boll.hpp"
#include "Clumsy.hpp"
#include "Player.hpp"
#include "Battleground.hpp"

#include "../GameEnd.hpp"

#include "../physics/Constraint.hpp"

#include "Game.hpp"


Game::Game(std::vector<Player *> *playersParam):
	players(playersParam)
{

	world = new Battleground(200.0f, 0.004f);


	followedObjects = new std::vector<Object *>();

	CPUControls *c;

	for (unsigned int i = 0; i < players->size(); i++) {
		objects->push_back(players->at(i));
		followedObjects->push_back(players->at(i));

		if ((c = dynamic_cast<CPUControls *>(players->at(i)->controls))) {
			c->game = this;
			c->player = players->at(i);
		}
	}

	clumsy = new Clumsy(Vector2f(0.0f, -80.0f), Color(160, 200, 80));
	boll = new Boll(Vector2f(0.0f, 0.0f), Color(140, 40, 200), clumsy);


	objects->push_back(boll);

	objects->push_back(clumsy);
	followedObjects->push_back(clumsy);


	constraints->push_back(new ElasticDistanceConstraint(clumsy, boll, 120.0f, 2.0f));


	gameView.setSize(Vector2f(1000, 1000));
	gameView.setCenter(Vector2f(0,0));;
}
Game::~Game() {

	delete world;

	// dont delete the objects in the player vector because they are also a part och the objects vector
	delete players;

	delete followedObjects;
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

		if (pl->releaseRope) {
			cutOffRope();
		} else {

			for (unsigned int i = 0; i < players->size(); i++) {
				if (players->at(i) != pl) {
					if (sqrSize(players->at(i)->vel) > 100.0f * 100.0f) {
						if (lineIntersect(players->at(i)->lastPos, players->at(i)->pos, clumsy->pos, boll->pos)) {

							cutOffRope();
						}
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < players->size(); i++) {
		if (players->at(i)->health <= 0) {
			Player *p = players->at(i);


			if (boll->connected) {
				Constraint *c = boll->constraint;
				Player *pl;

				if (c->a == boll) {
					pl = dynamic_cast<Player *>(c->b);
				} else {
					pl = dynamic_cast<Player *>(c->a);
				}

				if (pl == p) {
					cutOffRope();
				}
			}


			players->erase(players->begin()+i);
			for (unsigned int j = 0; j < objects->size(); j++) {
				if (objects->at(j) == p) {
					objects->erase(objects->begin()+j);
					break;
				}
			}

			for (unsigned int j = 0; j < followedObjects->size(); j++) {
				if (followedObjects->at(j) == p) {
					followedObjects->erase(followedObjects->begin()+j);
					break;
				}
			}

			delete p;

			if (players->size() < 2) {
				return new GameEnd(players->at(0)->sprite.getColor(), 6);//4);
			}
		}
	}

	return NULL;
}





bool Game::collision_callback(Object *a, Object *b) {

	Boll *bo;
	Player *pl;

	Constraint *c;

	if (((bo = dynamic_cast<Boll *>(a)) && (pl = dynamic_cast<Player *>(b)))
	|| ((bo = dynamic_cast<Boll *>(b)) && (pl = dynamic_cast<Player *>(a)))) {
		if (!bo->connected) {
			constraints->push_back(c = new PointConstraint(pl, bo));
			bo->constraint = c;
			bo->connected = true;

			pl->releaseRope = false;
		}
		return false;
	}
	return true;
}







void Game::draw(RenderWindow *window) {

	window->clear(sf::Color(100, 200, 100));
	//window->clear(sf::Color(0xff, 0xff, 0xff));

	float aspect = ((float)window->getSize().x / (float)window->getSize().y);

	// set game view
	{

		Vector2f smallest_most = followedObjects->at(0)->pos;
		Vector2f largest_most = followedObjects->at(0)->pos;
		for (unsigned int i = 1; i < followedObjects->size(); i++) {
			Vector2f v = followedObjects->at(i)->pos;
			if (v.x > largest_most.x) {
				largest_most.x = v.x;
			} else if (v.x < smallest_most.x) {
				smallest_most.x = v.x;
			}
			if (v.y > largest_most.y) {
				largest_most.y = v.y;
			} else if (v.y < smallest_most.y) {
				smallest_most.y = v.y;
			}
		}

		smallest_most.x /= aspect;
		largest_most.x /= aspect;


		float scale_multiply = size(smallest_most - largest_most) + 40.0f;

		if (scale_multiply < 200.0f) {
			scale_multiply = 200.0f;
		}

		Vector2f newPosition = (smallest_most + largest_most) / 2.0f;
		Vector2f newSize = Vector2f(aspect, 1.0f) * scale_multiply;

		Vector2f currentSize = gameView.getSize();
		Vector2f currentPosition = gameView.getCenter();

		gameView.setSize((newSize - currentSize) / 4.0f + currentSize);
		gameView.setCenter((newPosition - currentPosition) / 4.0f + currentPosition);
		
		window->setView(gameView);

	}


	world->draw(window);



	{
		Vector2f diffrence = (boll->pos - clumsy->pos);

		sf::RectangleShape line(sf::Vector2f(size(diffrence), 1));
		line.setOrigin(sf::Vector2f(0, 0.5f));
		line.rotate(angle(diffrence));
		line.setPosition(clumsy->pos);
		line.setFillColor(sf::Color(0, 0, 0));

		window->draw(line);
	}

	for (unsigned int i = 0; i < objects->size(); i++) {
		objects->at(i)->draw(window);
	}















	guiView.setSize(Vector2f(aspect, 1.0f));
	guiView.setCenter(guiView.getSize() / 2.0f);
	window->setView(guiView);

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




void Game::cutOffRope() {

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

	boll->vel += normal * 10.0f;
	boll->pos += normal * (o->radius + boll->radius);

	delete c;
	boll->connected = false;
}

