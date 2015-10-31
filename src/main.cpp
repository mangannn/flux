#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

using namespace sf;
using namespace std;

#include "game/Game.hpp"
#include "CharacterSelect.hpp"
#include "Startup.hpp"

#define WINDOW_TITLE ("FLUX: MÖÖP!")

sf::Texture startScreenTex;
sf::Vector2i startScreenTexSize(1900, 1500);

sf::Texture standingTex, runningTex;
sf::Texture bodyTex, eyesTex, mouthTex;
sf::Texture bollTex;

sf::Vector2i playerSpriteSize(200, 300);

sf::Vector2i bodySize(745, 745);
sf::Vector2i eyesSize(455, 35);
sf::Vector2i mouthSize(130, 50);

sf::Vector2i bollSize(125, 125);

sf::Font font;


bool load_resources() {

	std::cout << "Loading textures..." << std::endl;
	if (
		!startScreenTex.loadFromFile("media/images/FLUX.png") ||
		!standingTex.loadFromFile("media/images/player/standing.png") ||
		!runningTex.loadFromFile("media/images/player/running.png") ||
		!bodyTex.loadFromFile("media/images/clumsy/body.png") ||
		!eyesTex.loadFromFile("media/images/clumsy/eyes.png") ||
		!mouthTex.loadFromFile("media/images/clumsy/mouth.png") ||
		!bollTex.loadFromFile("media/images/boll/strip.png")) {
		
		return false;
	}
	std::cout << "Done!" << std::endl;

	standingTex.setSmooth(true);
	runningTex.setSmooth(true);
	bodyTex.setSmooth(true);
	eyesTex.setSmooth(true);
	mouthTex.setSmooth(true);
	bollTex.setSmooth(true);



	std::cout << "Loading font..." << std::endl;
	if (!font.loadFromFile("media/fonts/8bitlimo.ttf")) {
	    return false;
	}
	std::cout << "Done!" << std::endl;

	bollTex.setSmooth(false);

	return true;
}

int main() {

	if (!load_resources()) {
		return 1;
	}


	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 5;

	sf::RenderWindow *window;
	bool fullscreen = false;

	window = new RenderWindow(VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
	window->setMouseCursorVisible(true);
	window->setKeyRepeatEnabled(false);
	window->setFramerateLimit(50);



	EventPass *eventPass = new Startup(3);



	sf::Event event;
	sf::Clock clock;

	EventPass *temp_eventPass = NULL;

	while (window->isOpen()) {

		float elapsedTime = clock.restart().asSeconds();

		while (window->pollEvent(event)) {

			if ((temp_eventPass = eventPass->event_handle(event)) != NULL) {
				delete eventPass;
				eventPass = temp_eventPass;
				temp_eventPass = NULL;
			}

			switch (event.type) {
				case sf::Event::Closed: {
					window->close();
				} break;
				case sf::Event::KeyPressed: {

					switch (event.key.code) {
						case sf::Keyboard::Escape: {
							window->close();
						} break;

						case sf::Keyboard::F2:
						case sf::Keyboard::F3:
						case sf::Keyboard::F4:
						case sf::Keyboard::F5:
						case sf::Keyboard::F11:
						{

							window->close();
							delete window;

							fullscreen = !fullscreen;

							if (fullscreen) {
								window = new RenderWindow(VideoMode(), WINDOW_TITLE, sf::Style::Fullscreen | sf::Style::Close, settings);
							} else {
								window = new RenderWindow(VideoMode(800, 620), WINDOW_TITLE, sf::Style::Resize | sf::Style::Close, settings);
							}

							window->setMouseCursorVisible(!fullscreen);
							window->setKeyRepeatEnabled(false);

						} break;
						default: break;
					}
				} break;

				case sf::Event::JoystickConnected: {

					int id = event.joystickConnect.joystickId;

					cout << "Joystick connected: " << id << endl;
					cout << "Button Count: " << sf::Joystick::getButtonCount(id) << endl;
					cout << "X Axis: " << (sf::Joystick::hasAxis(id, sf::Joystick::X) ? "yes" : "no") << endl;
					cout << "Y Axis: " << (sf::Joystick::hasAxis(id, sf::Joystick::Y) ? "yes" : "no") << endl;
					cout << "Z Axis: " << (sf::Joystick::hasAxis(id, sf::Joystick::Z) ? "yes" : "no") << endl;
				} break;
				case sf::Event::JoystickDisconnected: {
					std::cout << "Joystick disconnected: " << event.joystickConnect.joystickId << std::endl;
				} break;
				default: break;
			}
		}

		if ((temp_eventPass = eventPass->update(elapsedTime)) != NULL) {
			delete eventPass;
			eventPass = temp_eventPass;
			temp_eventPass = NULL;
		}

		eventPass->draw(window);

		//Update window
		window->display();
	}

	delete eventPass;

	delete window;

	return 0;
}
