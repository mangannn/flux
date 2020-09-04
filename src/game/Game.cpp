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
#include "ParticleSystem.hpp"


Game::Game(std::vector<Player *> *playersParam):
	players(playersParam)
{

	world = new Battleground(200.0f, 0.004f);

	ParticleSystem::init();

	followedObjects = new std::vector<Object *>();

	CPUControls *c;

	for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); it++) {
		Player *p = *it;

		objects->push_back(p);
		followedObjects->push_back(p);

		if ((c = dynamic_cast<CPUControls *>(p->controls))) {
			c->game = this;
			c->player = p;
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

	ParticleSystem::exit();

	delete world;

	// dont delete the objects in the player vector because they are also a part och the objects vector
	delete players;

	delete followedObjects;
}






EventPass *Game::eventHandle(sf::Event event) {

	for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); it++) {
		(*it)->controls->eventHandle(event);
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

	for (std::vector<Object *>::iterator it = objects->begin(); it != objects->end(); it++) {
		(*it)->update(elapsedTime);
	}

	ParticleSystem::update(elapsedTime);

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

			for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); it++) {
				Player *p = *it;

				if (p != pl) {
					if (p->dashing) {
						if (lineIntersect(p->lastPos, p->pos, clumsy->pos, boll->pos)) {

							cutOffRope();
						}
					}
				}
			}
		}
	}

	for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); it++) {
		Player *p = *it;

		if (p->health <= 0) {

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


			players->erase(it);

			for (std::vector<Object *>::iterator it2 = objects->begin(); it2 != objects->end(); it2++) {
				if (*it2 == p) {
					objects->erase(it2);
					break;
				}
			}
			for (std::vector<Object *>::iterator it2 = followedObjects->begin(); it2 != followedObjects->end(); it2++) {
				if (*it2 == p) {
					followedObjects->erase(it2);
					break;
				}
			}

			delete p;

			if (players->size() <= 1) {
				if (players->size() == 1) {
					return new GameEnd(players->front()->sprite.getColor(), 4);
				} else {
					return new CharacterSelect();
				}
			}
			break;
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
		for (std::vector<Object *>::iterator it = followedObjects->begin() + 1; it != followedObjects->end(); it++) {
			Vector2f v = (*it)->pos;
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

	ParticleSystem::draw(window);

	for (std::vector<Object *>::iterator it = objects->begin(); it != objects->end(); it++) {
		(*it)->draw(window);
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

		for (std::vector<Player *>::iterator it = players->begin(); it != players->end(); it++) {
			Player *p = *it;

			window->draw(outline);

			Color color(p->sprite.getColor());
			color.a = 200;
			healthBar.setFillColor(color);
			healthBar.setScale(Vector2f(p->health, 1.0f));
			window->draw(healthBar);

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

	for (std::vector<Constraint *>::iterator it = constraints->begin(); it != constraints->end(); it++) {
		if (c == *it) {
			constraints->erase(it);
			break;
		}
	}


	Vector2f diffrence = (clumsy->pos - boll->pos);
	Vector2f normal = (diffrence / size(diffrence));

	boll->vel += normal * 10.0f;
	boll->pos += normal * (o->radius + boll->radius);

	delete c;
	boll->connected = false;
}

