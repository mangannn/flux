#include "../EventPass.hpp"
#include "../physics/Physics.hpp"

#include "../Functions.hpp"
#include "../Resources.hpp"


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Boll.hpp"
#include "Clumsy.hpp"
#include "Player.hpp"
#include "Battleground.hpp"




class Game : public EventPass, public Physics {

private:

	void load_player_list(const char* path, std::vector<Player *> *list);

public:

	sf::View game_view;
	sf::View gui_view;

	std::vector<Object *> *followedObjects;
	std::vector<Player *> *players;
	std::vector<Clumsy *> *clumsys;

	Boll *boll;
	Clumsy *clumsy;

	Game();
	virtual ~Game();


	virtual EventPass *event_handle(sf::Event event);
	virtual EventPass *update(float elapsedTime);

	virtual bool collision_callback(Object *a, Object *b);

	virtual void draw(RenderWindow *window);
};