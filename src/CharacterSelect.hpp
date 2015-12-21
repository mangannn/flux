#ifndef _CHARACTERSELECT_H_
#define _CHARACTERSELECT_H_

#include "EventPass.hpp"

class Game;
class Player;
class PlayerDummy;


class CharacterSelect : public EventPass {

public:

	CharacterSelect();
	virtual ~CharacterSelect();

	virtual EventPass *eventHandle(sf::Event event);

	virtual EventPass *update(float elapsedTime);

	virtual void draw(sf::RenderWindow *window);

private:

	sf::View view;

	sf::Sprite characterSprite;
	sf::Text text;
	sf::Color backgroundColor;

	float timer;

	std::vector<PlayerDummy *> *playerDummys;

	sf::Event::EventType catchEvent;
	int catchIndex;

	float markPosition, markDirection;
	int markedIndex;

	void drawButton(sf::RectangleShape box, sf::Text text, sf::Vector2f pos, sf::Vector2f size, sf::Color color, sf::String str, bool marked, sf::RenderWindow *window);

	void loadPlayerList(const char* path);
	void savePlayerList(const char* path);

	EventPass *createGame();

};

#endif
