#ifndef _GAME_H_
#define _GAME_H_

#include "../EventPass.hpp"
#include "../physics/Physics.hpp"


class Boll;
class Clumsy;
class Player;


class Game : public EventPass, public Physics {

public:

	sf::View gameView;
	sf::View guiView;

	std::vector<Object *> *followedObjects;
	std::vector<Player *> *players;

	Boll *boll;
	Clumsy *clumsy;

	Game(std::vector<Player *> *playersParam);
	virtual ~Game();


	virtual EventPass *eventHandle(sf::Event event);
	virtual EventPass *update(float elapsedTime);

	virtual bool collision_callback(Object *a, Object *b);

	virtual void draw(sf::RenderWindow *window);

	void cutOffRope();
};

#endif