#include <SFML/Graphics.hpp>
using namespace sf;

#include "../Controls.hpp"
#include "Game.hpp"
#include "Boll.hpp"
#include "Clumsy.hpp"
#include "Player.hpp"
#include "../physics/Constraint.hpp"

typedef struct {
	Vector2f p;
	float g;
} attraction_point;

CPUControls::CPUControls() {
	game = NULL;
}

bool CPUControls::action(int num) {return false;}

Vector2f CPUControls::movement() {

	std::vector<attraction_point> v;

	attraction_point ap;

	if (!game->boll->connected) {
		v.push_back(ap = {game->boll->pos, 60});
		v.push_back(ap = {game->clumsy->pos, -50});

		for (unsigned int i = 0; i < game->players->size(); i++) {
			if (game->players->at(i) != player) {
				v.push_back(ap = {game->players->at(i)->pos, (game->players->at(i)->health < 0.9f*player->health ? 30.f : -20.f)});
			}
		}

	} else {
		// bad assumption
		Player *player_connected = dynamic_cast<Player *>(game->boll->constraint->a);

		if (player_connected == player) {

			v.push_back(ap = {game->clumsy->pos, -70});
			for (unsigned int i = 0; i < game->players->size(); i++) {
				if (game->players->at(i) != player) {
					v.push_back(ap = {game->players->at(i)->pos, (game->players->at(i)->health < 0.9f*player->health ? 60.f : 10.f)});
				}
			}
		} else {
			v.push_back(ap = {game->clumsy->pos, -70});
			v.push_back(ap = {player_connected->pos, (player_connected->health < 0.9f*player->health ? 80.f : -30.f)});
		}
	}

	Vector2f gravity(0,0);
	for (unsigned int i = 0; i < v.size(); i++) {
		gravity += (v.at(i).g / sqrSize(v.at(i).p - player->pos))* (v.at(i).p - player->pos);
	}

	float s = size(gravity);
	if (s > 1) {
		gravity *= (1/s);
	}

	return gravity;
}
