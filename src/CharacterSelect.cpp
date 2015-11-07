#include <math.h>

#include "EventPass.hpp"
#include "game/Game.hpp"

#include "Functions.hpp"
#include "Resources.hpp"

#include "game/Player.hpp"

#include "CharacterSelect.hpp"
#include "Button.hpp"

#include <iostream>

class PlayerDummy {

public:

	Color color;
	int input_handle;

	int UP, DOWN, LEFT, RIGHT;
	int action_button[2];
	int axis[2];

	PlayerDummy(Color colorParam, int input_handleParam):
		color(colorParam),
		input_handle(input_handleParam)
	{

		if (input_handle < 0) {

			int keyboard_type = -(1 + input_handle);

			if (keyboard_type == 0) {
				UP = sf::Keyboard::Up;
				DOWN = sf::Keyboard::Down;
				LEFT = sf::Keyboard::Left;
				RIGHT = sf::Keyboard::Right;

				action_button[0] = sf::Keyboard::N;
				action_button[1] = sf::Keyboard::M;

			} else if (keyboard_type == 1) {
				UP = sf::Keyboard::W;
				DOWN = sf::Keyboard::S;
				LEFT = sf::Keyboard::A;
				RIGHT = sf::Keyboard::D;

				action_button[0] = sf::Keyboard::Z;
				action_button[1] = sf::Keyboard::X;

			} else if (keyboard_type == 2) {
				UP = sf::Keyboard::T;
				DOWN = sf::Keyboard::G;
				LEFT = sf::Keyboard::F;
				RIGHT = sf::Keyboard::H;

				action_button[0] = sf::Keyboard::V;
				action_button[1] = sf::Keyboard::B;

			} else {
				UP = sf::Keyboard::I;
				DOWN = sf::Keyboard::K;
				LEFT = sf::Keyboard::J;
				RIGHT = sf::Keyboard::L;

				action_button[0] = sf::Keyboard::O;
				action_button[1] = sf::Keyboard::P;
			}
		} else {

			axis[0] = sf::Joystick::Y;
			axis[1] = sf::Joystick::X;

			action_button[0] = 0;
			action_button[1] = 1;
		}
	}
};




CharacterSelect::CharacterSelect() :
	backgroundColor(0,0,0),
	timer(0.0f)
{

	playerDummys = new std::vector<PlayerDummy *>();

	loadPlayerList("media/player_list.txt");

	if (playerDummys->size() < 1) {
		addDummy(
				RANDOM_COLOR, 
				-1);
	}

	float radius = 0.1f;

	characterSprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
	characterSprite.setTexture(standingTex);
	characterSprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f));

	characterSprite.setScale(sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x));


	text.setFont(font);
	text.setCharacterSize(64);
	text.setScale(Vector2f(0.003,0.003));

	text.setString("Flux");


	startButton = new Button(Vector2f(0, 0.4), Vector2f(0.52, 0.1), Color(160, 60, 60, 0), "Start Game!");

}
CharacterSelect::~CharacterSelect() {

	delete startButton;

	{
		PlayerDummy *temp;
		while (!playerDummys->empty()) {
			temp = playerDummys->back();
			delete temp;
			playerDummys->pop_back();
		}
		delete playerDummys;
	}
}


EventPass *CharacterSelect::eventHandle(sf::Event event) {

	switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
				case sf::Keyboard::Q: {
					
					int input_handle = -1;

					for (unsigned int i = 0; i < playerDummys->size(); i++) {
						if (playerDummys->at(i)->input_handle == input_handle) {
							input_handle -= 1;
							i = -1;
						}
					}

					addDummy(
						RANDOM_COLOR, 
						input_handle);

				} break;
				case sf::Keyboard::W: {

					if (playerDummys->size() > 0) {
					
						PlayerDummy *temp;
						temp = playerDummys->back();
						delete temp;
						playerDummys->pop_back();
					}

				} break;
				case sf::Keyboard::E: {

					if (playerDummys->size() > 0) {
					
						PlayerDummy *temp;
						temp = playerDummys->front();
						delete temp;
						playerDummys->erase(playerDummys->begin());
					}

				} break;
				case sf::Keyboard::R: {

					{
						PlayerDummy *temp;
						while (!playerDummys->empty()) {
							temp = playerDummys->back();
							delete temp;
							playerDummys->pop_back();
						}
					}

					loadPlayerList("media/player_list.txt");

				} break;
				case sf::Keyboard::Return: {
					return createGame();
				} break;
				default: break;
			}
		} break;
		case sf::Event::MouseButtonPressed: {
			switch (event.mouseButton.button) {
				case sf::Mouse::Left: {
					Vector2f mouseClick = win->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
					if (startButton->inside(mouseClick)) {
						return createGame();
					}
				} break;
				default: break;
			}
		} break;
		default: break;
	}

	return NULL;
}

EventPass *CharacterSelect::update(float elapsedTime) {

	timer += elapsedTime;

	float a = 0.3f;

	backgroundColor.r = 128 - (int)(120.0f * cos(timer * a * 3.0f));
	backgroundColor.g = 128 - (int)(120.0f * cos(timer * a * 5.0f));
	backgroundColor.b = 128 - (int)(120.0f * cos(timer * a * 7.0f));

	return NULL;
}

void CharacterSelect::draw(RenderWindow *window) {

	win = window;

	float aspect = ((float)window->getSize().x / (float)window->getSize().y);


	view.setSize(Vector2f(aspect, 1.0f));
	view.setCenter(Vector2f(0,0));

	window->setView(view);
	window->clear(backgroundColor);


	text.setPosition(Vector2f(-text.getGlobalBounds().width / 2.0f, -0.47));
	Color textColor;
	textColor.r = 128 - (int)(120.0f * cos(timer * 1.0f));
	textColor.g = 128 - (int)(120.0f * cos(timer * 2.0f));
	textColor.b = 128 - (int)(120.0f * cos(timer * 3.0f));
	text.setColor(textColor);
	window->draw(text);

	startButton->draw(window);


	float startx = (aspect * 0.45) * (1.0 - (1.0 / playerDummys->size()));
	float spacing = ((startx * 2) / (playerDummys->size() < 2 ? 1 : playerDummys->size() - 1));

	char str[10];

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		Vector2f offset(-startx + i * spacing, 0);

		characterSprite.setRotation(sin((timer + i) * 2.0f) * 20);
		characterSprite.setPosition(offset + Vector2f(0, -0.1f));
		characterSprite.setColor(playerDummys->at(i)->color);
		window->draw(characterSprite);


		const float size = 0.06f;

		sf::RectangleShape box(Vector2f(size, size));

		Vector2f centerArrows = offset + Vector2f(0, 0.1);

		const float arrowPlacement = size;

		if (playerDummys->at(i)->input_handle < 0) {
			sprintf(str, "%d", playerDummys->at(i)->UP);
			Button(centerArrows + Vector2f(0, -arrowPlacement), Vector2f(size, size), Color(60, 60, 160), str).draw(window);
			sprintf(str, "%d", playerDummys->at(i)->DOWN);
			Button(centerArrows + Vector2f(0, arrowPlacement), Vector2f(size, size), Color(60, 160, 60), str).draw(window);
			sprintf(str, "%d", playerDummys->at(i)->LEFT);
			Button(centerArrows + Vector2f(-arrowPlacement, 0), Vector2f(size, size), Color(160, 60, 160), str).draw(window);
			sprintf(str, "%d", playerDummys->at(i)->RIGHT);
			Button(centerArrows + Vector2f(arrowPlacement, 0), Vector2f(size, size), Color(160, 160, 60), str).draw(window);
		} else {
			sprintf(str, "%d", playerDummys->at(i)->axis[0]);
			Button(centerArrows, Vector2f(size * 3, size), Color(60, 60, 160), str).draw(window);
			sprintf(str, "%d", playerDummys->at(i)->axis[1]);
			Button(centerArrows, Vector2f(size, size * 3), Color(60, 160, 60), str).draw(window);
		}

		Vector2f centerButton = centerArrows + Vector2f(0, 0.15);
		const float buttonPlacement = size;

		sprintf(str, "%d", playerDummys->at(i)->action_button[0]);
		Button(centerButton + Vector2f(-buttonPlacement, 0), Vector2f(size, size), Color(160, 60, 60), str).draw(window);
		sprintf(str, "%d", playerDummys->at(i)->action_button[1]);
		Button(centerButton + Vector2f(buttonPlacement, 0), Vector2f(size, size), Color(60, 160, 160), str).draw(window);

	}
}






void CharacterSelect::addDummy(Color color, int input_handle) {
	playerDummys->push_back(new PlayerDummy(
			color, 
			input_handle));
}

void CharacterSelect::loadPlayerList(const char* path) {

	FILE *file;

	if (!(file = fopen(path, "r"))) {
		std::cout << "Failed to open file: " << path << std::endl;
		exit(-1);
	}

	const int MAXSTR = 256;
	char buffer[MAXSTR];

	int r = 0, g = 0, b = 0;

	char input_type = 'k';
	int input_handle = 0;

	int num_back;

	while (fgets(buffer, MAXSTR, file)) {

		num_back = sscanf(buffer, 
			"color{%d,%d,%d} input{%c%d}", 
			&r, &g, &b, &input_type, &input_handle);

		if (num_back != 5) {
			continue;
		}

		if (input_type == 'k') {
			input_handle += 1;
			input_handle = -input_handle;
		}

		addDummy(
			Color(r, g, b), 
			input_handle);
	}
	fclose(file);
}
void CharacterSelect::savePlayerList(const char* path) {

	FILE *file;

	if (!(file = fopen(path, "w"))) {
		std::cout << "Failed to open file: " << path << std::endl;
		exit(-1);
	}

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		PlayerDummy *d = playerDummys->at(i);

		char input_type;
		int input_handle;

		if (d->input_handle < 0) {
			input_type = 'k';
			input_handle = (-d->input_handle) - 1;
		}

		if (fprintf(file, 
			"color{%d, %d, %d} input{%c%d}\n", 
			d->color.r, d->color.g, d->color.b, input_type, input_handle) < 1) {

			std::cout << "Error writeing to file: " << path << std::endl;
			exit(-1);
		}
	}
	fclose(file);
}

EventPass *CharacterSelect::createGame() {

	if (playerDummys->size() < 1) {
		return NULL;
		std::cout << "Can't start a game without players" << std::endl;
	}

	for (unsigned int i = 0; i < playerDummys->size(); i++) {
		for (unsigned int j = i + 1; j < playerDummys->size(); j++) {
			if (playerDummys->at(i)->input_handle == playerDummys->at(j)->input_handle) {
				std::cout << "Multiple players have the same controll settings!" << std::endl;
				return NULL;
			}
		}
	}

	savePlayerList("media/player_list.txt");

	std::vector<Player *> *players = new std::vector<Player *>();

	float startx = (2.0 / 3.0) * M_PI * (1.0 - (1.0 / playerDummys->size()));
	float spacing = ((startx * 2) / (playerDummys->size() < 2 ? 1 : playerDummys->size() - 1));

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		float t = (-startx + i * spacing) + M_PI / 2.0;

		players->push_back(new Player(
			Vector2f(cos(t), sin(t)) * 40.0f,
			playerDummys->at(i)->color, 
			playerDummys->at(i)->input_handle));
	}

	return new Game(players);
}
