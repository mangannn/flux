#include <math.h>

#include "EventPass.hpp"
#include "game/Game.hpp"

#include "Functions.hpp"
#include "Resources.hpp"

#include "game/Player.hpp"

#include "CharacterSelect.hpp"

#include <iostream>


#include "InputCodeString.hpp"



class PlayerDummy {

public:

	enum ControllType {
		Keyboard = 0,
		Joystick,
		Computer
	};

	Color color;

	ControllType type;

	int UP, DOWN, LEFT, RIGHT;

	int axisX, axisY;
	int joystickId;

	int actionButton[2];

	PlayerDummy(Color colorParam, ControllType t, int inputParam):
		color(colorParam),
		type(t)
	{
		switch (type) {
			case ControllType::Keyboard: {
				switch (inputParam) {
					case 0: {
						UP = sf::Keyboard::Up;
						DOWN = sf::Keyboard::Down;
						LEFT = sf::Keyboard::Left;
						RIGHT = sf::Keyboard::Right;

						actionButton[0] = sf::Keyboard::N;
						actionButton[1] = sf::Keyboard::M;
					} break;
					case 1: {
						UP = sf::Keyboard::W;
						DOWN = sf::Keyboard::S;
						LEFT = sf::Keyboard::A;
						RIGHT = sf::Keyboard::D;

						actionButton[0] = sf::Keyboard::Z;
						actionButton[1] = sf::Keyboard::X;
					} break;
					case 2: {
						UP = sf::Keyboard::T;
						DOWN = sf::Keyboard::G;
						LEFT = sf::Keyboard::F;
						RIGHT = sf::Keyboard::H;

						actionButton[0] = sf::Keyboard::V;
						actionButton[1] = sf::Keyboard::B;
					} break;
					case 3: {
						UP = sf::Keyboard::I;
						DOWN = sf::Keyboard::K;
						LEFT = sf::Keyboard::J;
						RIGHT = sf::Keyboard::L;

						actionButton[0] = sf::Keyboard::O;
						actionButton[1] = sf::Keyboard::P;
					} break;
					default: {
						UP = sf::Keyboard::Unknown;
						DOWN = sf::Keyboard::Unknown;
						LEFT = sf::Keyboard::Unknown;
						RIGHT = sf::Keyboard::Unknown;

						actionButton[0] = sf::Keyboard::Unknown;
						actionButton[1] = sf::Keyboard::Unknown;
					}
				}
			} break;
			case ControllType::Joystick: {

				joystickId = inputParam;

				axisX = sf::Joystick::X;
				axisY = sf::Joystick::Y;

				actionButton[0] = 0;
				actionButton[1] = 1;

			} break;
			case ControllType::Computer: {
				// Nothing need be done
			}
		}
	}
};




CharacterSelect::CharacterSelect() :
	backgroundColor(0,0,0),
	timer(0.0f),

	catchEvent((sf::Event::EventType)0),
	catchIndex(-1),

	selectMarkerPosition(0),
	selectMarkerDirection(0),
	selectMarkerIndex(0)
{

	playerDummys = new std::vector<PlayerDummy *>();

	loadPlayerList("media/player_list.txt");

	if (playerDummys->size() < 1) {
		playerDummys->push_back(new PlayerDummy(
			RANDOM_COLOR, PlayerDummy::ControllType::Keyboard, 0));
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

}
CharacterSelect::~CharacterSelect() {

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

	if (event.type == catchEvent) {

		switch (event.type) {
			case sf::Event::KeyPressed: {

				int key = event.key.code;

				PlayerDummy *pl = playerDummys->at(selectMarkerIndex);

				switch (catchIndex) {
					case 0: {
						pl->UP = key;
					} break;
					case 1: {
						pl->DOWN = key;
					} break;
					case 2: {
						pl->LEFT = key;
					} break;
					case 3: {
						pl->RIGHT = key;
					} break;
					case 4: {
						pl->actionButton[0] = key;
					} break;
					case 5: {
						pl->actionButton[1] = key;
					} break;
				}

				catchIndex += 1;
				if (catchIndex >= 6) {
					catchEvent = (sf::Event::EventType)0;
					catchIndex = -1;
				}

			} break;
			case sf::Event::JoystickButtonPressed: {
				if (catchIndex == 0) {
					playerDummys->at(selectMarkerIndex)->joystickId = event.joystickButton.joystickId;
					catchEvent = sf::Event::JoystickMoved;
					catchIndex += 1;
				} else {

					PlayerDummy *pl = playerDummys->at(selectMarkerIndex);

					if (pl->joystickId == (int)event.joystickButton.joystickId) {

						int key = event.joystickButton.button;

						switch (catchIndex) {
							case 3: {
								pl->actionButton[0] = key;
							} break;
							case 4: {
								pl->actionButton[1] = key;
							} break;
						}

						catchIndex += 1;
						if (catchIndex >= 5) {
							catchEvent = (sf::Event::EventType)0;
							catchIndex = -1;
						}
					}
				}
			} break;
			case sf::Event::JoystickMoved: {
				PlayerDummy *pl = playerDummys->at(selectMarkerIndex);

				if (pl->joystickId == (int)event.joystickMove.joystickId) {
					if (fabs(event.joystickMove.position) > 50.0f) { // catch twitch events

						int axis = event.joystickMove.axis;

						switch (catchIndex) {
							case 1: {
								pl->axisX = axis;
						
								catchIndex += 1;

							} break;
							default: {
								if (axis != pl->axisX) {
									pl->axisY = axis;
									catchEvent = sf::Event::JoystickButtonPressed;

									catchIndex += 1;
								}
							} break;
						}
					}
				}
			} break;
			default: {
				std::cout << "Not valid event to catch" << std::endl;
			} break;
		}
		
		return NULL;
	}

	switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
				case sf::Keyboard::Q: {

					playerDummys->push_back(new PlayerDummy(
						RANDOM_COLOR,
						PlayerDummy::ControllType::Keyboard, 
						playerDummys->size()));

					selectMarkerIndex = playerDummys->size() - 1;

				} break;
				case sf::Keyboard::W: {

					if (selectMarkerIndex >= 0 && selectMarkerIndex < (int)playerDummys->size()) {
				
						PlayerDummy *temp;
						temp = playerDummys->at(selectMarkerIndex);
						delete temp;
						playerDummys->erase(playerDummys->begin() + selectMarkerIndex);

						selectMarkerIndex -= 1;
						if (selectMarkerIndex < 0) {
							selectMarkerIndex = 0;
						}

						if (playerDummys->size() < 1) {
							playerDummys->push_back(new PlayerDummy(
								RANDOM_COLOR,
								PlayerDummy::ControllType::Keyboard, 
								0));
						}
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
					selectMarkerIndex = 0;

				} break;
				case sf::Keyboard::C: {
					playerDummys->at(selectMarkerIndex)->color = RANDOM_COLOR;
				} break;
				case sf::Keyboard::K: {

					catchIndex = 0;
					catchEvent = sf::Event::KeyPressed;

					playerDummys->at(selectMarkerIndex)->type = PlayerDummy::ControllType::Keyboard;

				} break;
				case sf::Keyboard::J: {

					catchIndex = 0;
					catchEvent = sf::Event::JoystickButtonPressed;

					playerDummys->at(selectMarkerIndex)->type = PlayerDummy::ControllType::Joystick;

				} break;
				case sf::Keyboard::H: {
					playerDummys->at(selectMarkerIndex)->type = PlayerDummy::ControllType::Computer;

				} break;
				case sf::Keyboard::Left: {
					selectMarkerIndex -= 1;
					if (selectMarkerIndex < 0) {
						selectMarkerIndex += playerDummys->size();
					}
				} break;
				case sf::Keyboard::Right: {
					selectMarkerIndex += 1;
					if (selectMarkerIndex >= (int)playerDummys->size()) {
						selectMarkerIndex -= playerDummys->size();
					}
				} break;

				case sf::Keyboard::Return: {
					return newGame();
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
	/*textColor.r = 255 - backgroundColor.r;
	textColor.g = 255 - backgroundColor.g;
	textColor.b = 255 - backgroundColor.b;*/
	text.setFillColor(textColor);
	window->draw(text);


	const float startx = (aspect * 0.45) * (1.0 - (1.0 / playerDummys->size()));
	const float spacing = ((startx * 2) / (playerDummys->size() < 2 ? 1 : playerDummys->size() - 1));


	// Update mark position
	const float selectMarkerPositionGoal = -startx + (selectMarkerIndex * spacing);
	const float selectMarkerDirectionGoal = sin((timer + selectMarkerIndex) * 2.0f) * 20;

	selectMarkerPosition = (selectMarkerPositionGoal - selectMarkerPosition) / 5.0f + selectMarkerPosition;
	selectMarkerDirection = (selectMarkerDirectionGoal - selectMarkerDirection) / 5.0f + selectMarkerDirection;


	const float markSize = 0.3;
	const Vector2f markV(markSize, markSize);

	sf::RectangleShape box;
	box.setSize(markV);
	box.setOrigin(markV * 0.5f);
	box.setPosition(Vector2f(selectMarkerPosition, -0.12f));
	box.setFillColor(Color::Red);
	box.setRotation(selectMarkerDirection);
	window->draw(box);

	char str[10];

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		Vector2f offset(-startx + i * spacing, 0);

		characterSprite.setRotation(sin((timer + i) * 2.0f) * 20);
		characterSprite.setPosition(offset + Vector2f(0, -0.1f));
		characterSprite.setColor(playerDummys->at(i)->color);
		window->draw(characterSprite);


		const float size = 0.06f;

		sf::RectangleShape box;
		sf::Text text;

		text.setFont(font);
		text.setCharacterSize(64);
		text.setScale(0.012f * size, 0.012f * size);


		const Vector2f centerArrows = offset + Vector2f(0, 0.1);
		const float arrowPlacement = size;

		const Vector2f centerButton = centerArrows + Vector2f(0, 0.15);
		const float buttonPlacement = size * 0.7f;

		switch (playerDummys->at(i)->type) {
			case PlayerDummy::ControllType::Keyboard: {

				drawButton(box, text, 
					centerArrows + Vector2f(0, -arrowPlacement), Vector2f(size, size), 
					Color(60, 60, 160), keyCodeToString(playerDummys->at(i)->UP),
					(selectMarkerIndex == (int)i && catchIndex == 0), window);
				drawButton(box, text, 
					centerArrows + Vector2f(0, arrowPlacement), Vector2f(size, size),
					Color(60, 160, 60), keyCodeToString(playerDummys->at(i)->DOWN),
					(selectMarkerIndex == (int)i && catchIndex == 1), window);
				drawButton(box, text, 
					centerArrows + Vector2f(-arrowPlacement, 0), Vector2f(size, size), 
					Color(160, 60, 160), keyCodeToString(playerDummys->at(i)->LEFT), 
					(selectMarkerIndex == (int)i && catchIndex == 2), window);
				drawButton(box, text, 
					centerArrows + Vector2f(arrowPlacement, 0), Vector2f(size, size), 
					Color(160, 160, 60), keyCodeToString(playerDummys->at(i)->RIGHT),
					(selectMarkerIndex == (int)i && catchIndex == 3), window);

				drawButton(box, text, 
					centerButton + Vector2f(-buttonPlacement, 0), Vector2f(size, size), 
					Color(160, 60, 60), keyCodeToString(playerDummys->at(i)->actionButton[0]),
					(selectMarkerIndex == (int)i && catchIndex == 4), window);
				drawButton(box, text, 
					centerButton + Vector2f(buttonPlacement, 0), Vector2f(size, size), 
					Color(60, 160, 160), keyCodeToString(playerDummys->at(i)->actionButton[1]),
					(selectMarkerIndex == (int)i && catchIndex == 5), window);
				
			} break;
			case PlayerDummy::ControllType::Joystick: {

				sprintf(str, "%d", playerDummys->at(i)->joystickId);
				drawButton(box, text, 
					centerArrows + Vector2f(0, arrowPlacement), Vector2f(size, size), 
					Color(60, 60, 60), str,
					(selectMarkerIndex == (int)i && catchIndex == 0), window);

				drawButton(box, text, 
					centerArrows + Vector2f(-arrowPlacement / 2.0f, 0), Vector2f(size * 2, size), 
					Color(60, 60, 160), joystickAxisToString(playerDummys->at(i)->axisX), 
					(selectMarkerIndex == (int)i && catchIndex == 1), window);
				drawButton(box, text, 
					centerArrows + Vector2f(arrowPlacement, -arrowPlacement / 2.0f), Vector2f(size, size * 2), 
					Color(60, 160, 60), joystickAxisToString(playerDummys->at(i)->axisY),
					(selectMarkerIndex == (int)i && catchIndex == 2), window);

				sprintf(str, "%d", playerDummys->at(i)->actionButton[0]);
				drawButton(box, text, 
					centerButton + Vector2f(-buttonPlacement, 0), Vector2f(size, size), 
					Color(160, 60, 60), str,
					(selectMarkerIndex == (int)i && catchIndex == 3), window);
				sprintf(str, "%d", playerDummys->at(i)->actionButton[1]);
				drawButton(box, text, 
					centerButton + Vector2f(buttonPlacement, 0), Vector2f(size, size), 
					Color(60, 160, 160), str, 
					(selectMarkerIndex == (int)i && catchIndex == 4), window);

			} break;
			case PlayerDummy::ControllType::Computer: {
				drawButton(box, text, 
					centerArrows + Vector2f(0, arrowPlacement), Vector2f(size*2, size), 
					Color(160, 0, 160), "CPU", false, window);
			} break;
		}
	}
}


void CharacterSelect::drawButton(sf::RectangleShape box, sf::Text text, sf::Vector2f pos, sf::Vector2f size, sf::Color color, sf::String str, bool active, sf::RenderWindow *window) {
	
	box.setPosition(pos);
	box.setOrigin(0.5f * size);
	box.setSize(size);


	if (active) {
		box.setFillColor(Color(120, 0, 0));
		text.setString("?");
		box.setRotation(sin((timer + selectMarkerIndex) * 16.0f) * 16);

		window->draw(box);

		box.setRotation(0);

	} else {
		box.setFillColor(color);
		text.setString(str);

		window->draw(box);
	}

	text.setPosition(pos - 0.5f * Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height * 1.3));
	window->draw(text);
}


void CharacterSelect::loadPlayerList(const char* path) {

	FILE *file;

	if (!(file = fopen(path, "r"))) {
		std::cout << "Failed to load player list (" << path << ")\n";
		return;
	}

	const int MAXSTR = 256;
	char buffer[MAXSTR];

	char str[MAXSTR];

	int cr = 0, cg = 0, cb = 0;

	int joystickId = 0;
	int k_up = 0, k_down = 0, k_left = 0, k_right = 0, a1 = 0, a2 = 0, axisX = 0, axisY = 0;

	int num_back;

	while (fgets(buffer, MAXSTR, file)) {

		// %[^}\n] saves all characters until a } or \n

		num_back = sscanf(buffer, 
			"color{%d,%d,%d} input{%[^}\n]}", 
			&cr, &cg, &cb, str);

		if (num_back != 4) {
			continue;
		}

		switch (str[0]) {
			case 'k': {

				num_back = sscanf(str, 
				"k,%d,%d,%d,%d,%d,%d", 
				&k_up, &k_down, &k_left, &k_right, &a1, &a2);

				PlayerDummy *pl = new PlayerDummy(sf::Color(cr, cg, cb), PlayerDummy::ControllType::Keyboard, 0);

				// if input settings in file is ok, use them, otherwise use preset
				if (num_back == 6) {

					pl->UP = k_up;
					pl->DOWN = k_down;
					pl->LEFT = k_left;
					pl->RIGHT = k_right;

					pl->actionButton[0] = a1;
					pl->actionButton[1] = a2;
				}

				playerDummys->push_back(pl);
			} break;
			case 'j': {
				num_back = sscanf(str, 
				"j,%d,%d,%d,%d",
				&axisX, &axisY, &a1, &a2);

				PlayerDummy *pl = new PlayerDummy(sf::Color(cr, cg, cb), PlayerDummy::ControllType::Joystick, joystickId);

				joystickId += 1;

				// if input settings in file is ok, use them, otherwise use preset
				if (num_back == 5) {

					pl->axisX = axisX;
					pl->axisY = axisY;

					pl->actionButton[0] = a1;
					pl->actionButton[1] = a2;
				}

				playerDummys->push_back(pl);
			} break;
			case 'c': {

				PlayerDummy *pl = new PlayerDummy(sf::Color(cr, cg, cb), PlayerDummy::ControllType::Computer, 0);

				playerDummys->push_back(pl);
			} break;
			default: {

			}
		}
	}
	fclose(file);
}
void CharacterSelect::savePlayerList(const char* path) {

	FILE *file;

	if (!(file = fopen(path, "w"))) {
		std::cout << "Failed to save player list (" << path << ")\n";
		return;
	}

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		PlayerDummy *pl = playerDummys->at(i);

		const int MAXSTR = 256;
		char str[MAXSTR];

		switch (pl->type) {
			case PlayerDummy::ControllType::Keyboard: {
				sprintf(str, "%c, %d, %d, %d, %d, %d, %d", 
					'k', pl->UP, pl->DOWN, pl->LEFT, pl->RIGHT, pl->actionButton[0], pl->actionButton[1]);
			} break;
			case PlayerDummy::ControllType::Joystick: {
				sprintf(str, "%c, %d, %d, %d, %d", 
					'j', pl->axisX, pl->axisY, pl->actionButton[0], pl->actionButton[1]);
			} break;
			case PlayerDummy::ControllType::Computer: {
				sprintf(str, "%c", 
					'c');
			}
		}

		if (fprintf(file, 
			"color{%d, %d, %d} input{%s}\n", 
			pl->color.r, pl->color.g, pl->color.b, str) < 1) {

			std::cout << "Error writing to file: " << path << std::endl;
			exit(-1);
		}
	}
	fclose(file);
}

EventPass *CharacterSelect::newGame() {

	if (playerDummys->size() < 1) {
		return NULL;
		std::cout << "Can't start a game without players" << std::endl;
	}

	savePlayerList("media/player_list.txt");

	std::vector<Player *> *players = new std::vector<Player *>();

	float startAngle = (2.0 / 3.0) * M_PI * (1.0 - (1.0 / playerDummys->size()));
	float spacing = ((startAngle * 2) / (playerDummys->size() < 2 ? 1 : playerDummys->size() - 1));

	const float startRadius = 40.0f;

	for (unsigned int i = 0; i < playerDummys->size(); i++) {

		float angle = (-startAngle + i * spacing) + (M_PI / 2.0);

		PlayerDummy *pl = playerDummys->at(i);
		Controls *controls;

		switch (pl->type) {
			case PlayerDummy::ControllType::Keyboard: {
				controls = new KeyboardControls(pl->UP, pl->DOWN, pl->LEFT, pl->RIGHT, pl->actionButton, 2);
			} break;
			case PlayerDummy::ControllType::Joystick: {
				controls = new JoystickControls(pl->joystickId, pl->axisX, pl->axisY, pl->actionButton, 2);
			} break;
			case PlayerDummy::ControllType::Computer: {
				controls = new CPUControls();
			}
		}

		players->push_back(new Player(
			Vector2f(cos(angle), sin(angle)) * startRadius,
			angle * (180.0f / M_PI),
			pl->color, 
			controls));
	}

	return new Game(players);
}
