#include "EventPass.hpp"

class PlayerDummy {

public:

	Vector2f pos;
	Vector2f vel;
	Color color;
	int input_handle;

	PlayerDummy(Vector2f posParam, Color colorParam, int input_handleParam):
		pos(posParam),
		color(colorParam),
		input_handle(input_handleParam)
	{}

	virtual ~PlayerDummy() {}
};



class CharacterSelect : public EventPass {

private:

	void addDummy(Vector2f pos, Color color, int input_handle) {
		playerDummys->push_back(new PlayerDummy(
				pos,
				color, 
				input_handle));
	}

	void load_player_list(const char* path) {


		FILE *file;

		if (!(file = fopen(path, "r"))) {
			cout << "Failed to open file: " << path << endl;
			exit(-1);
		}

		const int MAXSTR = 256;
		char buffer[MAXSTR];

		float x = 0.0, y = 0.0;
		int r = 0, g = 0, b = 0;

		char input_type = 'k';
		int input_handle = 0;

		int num_back;

		while (fgets(buffer, MAXSTR, file)) {

			num_back = sscanf(buffer, 
				"pos: %f %f color: %d %d %d input: %c%d", 
				&x, &y, &r, &g, &b, &input_type, &input_handle);

			if (num_back != 7) {
				continue;
			}

			if (input_type == 'k') {
				input_handle += 1;
				input_handle = -input_handle;
			}

			addDummy(
				Vector2f(x, y),
				Color(r, g, b), 
				input_handle);
		}
		fclose(file);
	}

public:

	sf::View view;

	sf::Sprite characterSprite;
	sf::Text text;

	Color backgroundColor;

	float timer;

	std::vector<PlayerDummy *> *playerDummys;

	CharacterSelect() :
		backgroundColor(0,0,0),
		timer(0.0f)
	{

		playerDummys = new std::vector<PlayerDummy *>();

		load_player_list("media/player_list.txt");

		if (playerDummys->size() < 1) {
			addDummy(
					Vector2f(0, 0),
					RANDOM_COLOR, 
					-1);
		}


		/*for (int i = 0; i < 2; i++) {
			playerDummys->push_back(new PlayerDummy(Vector2f((i + 1), 0.0f), RANDOM_COLOR, -1));
			playerDummys->push_back(new PlayerDummy(Vector2f(-(i + 1), 0.0f), RANDOM_COLOR, -2));
			playerDummys->push_back(new PlayerDummy(Vector2f(0.0f, (i + 1)), RANDOM_COLOR, -3));
			playerDummys->push_back(new PlayerDummy(Vector2f(0.0f, -(i + 1)), RANDOM_COLOR, -4));
		}*/

		float radius = 0.1f;


		characterSprite.setTextureRect(sf::IntRect(0, 0, playerSpriteSize.x, playerSpriteSize.y));
		characterSprite.setTexture(standingTex);
		characterSprite.setOrigin(sf::Vector2f((float)playerSpriteSize.x / 2.0f, (float)playerSpriteSize.y / 2.0f));

		characterSprite.setScale(sf::Vector2f((radius * 2.0f) / (float)playerSpriteSize.x, (radius * 2.0f) / (float)playerSpriteSize.x));


		text.setFont(font);
		text.setCharacterSize(64);
		text.setColor(sf::Color::Blue);
		//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
		text.setScale(Vector2f(0.002,0.002));

	}
	virtual ~CharacterSelect() {
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


	virtual EventPass *event_handle(sf::Event event) {

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
							Vector2f(0, 0),
							RANDOM_COLOR, 
							input_handle);

					} break;
					case sf::Keyboard::Return: {
						return createGame();
					} break;
					default: break;
				}
			} break;
			default: break;
		}

		return NULL;
	}

	virtual EventPass *update(float elapsedTime) {

		timer += elapsedTime;

		float a = 0.3f;

		backgroundColor.r = 128 - (int)(120.0f * cos(timer * a * 3.0f));
		backgroundColor.g = 128 - (int)(120.0f * cos(timer * a * 5.0f));
		backgroundColor.b = 128 - (int)(120.0f * cos(timer * a * 7.0f));

		return NULL;
	}

	virtual void draw(RenderWindow *window) {

		float aspect = ((float)window->getSize().x / (float)window->getSize().y);


		view.setSize(Vector2f(aspect, 1.0f));
		view.setCenter(Vector2f(0,0));

		window->setView(view);
		window->clear(backgroundColor);


		float startx = 0.45 * (1.0 - (1.0 / playerDummys->size()));
		float spacing = ((startx * 2) / (playerDummys->size() < 2 ? 1 : playerDummys->size() - 1));

		char str[10];

		for (unsigned int i = 0; i < playerDummys->size(); i++) {

			characterSprite.setRotation(sin((timer + i) * 2.0f) * 20);
			characterSprite.setPosition(Vector2f(-startx + i * spacing, 0));
			characterSprite.setColor(playerDummys->at(i)->color);
			window->draw(characterSprite);

			sprintf(str, "%d", playerDummys->at(i)->input_handle);
			text.setString(str);
			text.setPosition(Vector2f(-startx + i * spacing, 0.2));
			window->draw(text);
		}
	}

	EventPass *createGame() {

		for (unsigned int i = 0; i < playerDummys->size(); i++) {
			for (unsigned int j = i + 1; j < playerDummys->size(); j++) {
				if (playerDummys->at(i)->input_handle == playerDummys->at(j)->input_handle) {
					std::cout << "Multiple players have the same controll settings!\n";
					return NULL;
				}
			}
		}

		std::vector<Player *> *players = new std::vector<Player *>();

		for (unsigned int i = 0; i < playerDummys->size(); i++) {
			players->push_back(new Player(
				playerDummys->at(i)->pos,
				Vector2f(0, 0),
				playerDummys->at(i)->color, 
				playerDummys->at(i)->input_handle));
		}

		return new Game(players);
	}
};