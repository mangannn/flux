#ifndef _RESOURCES_H_
#define _RESOURCES_H_


sf::Texture standingTex, runningTex;
sf::Texture bodyTex, eyesTex, mouthTex;
sf::Texture bollTex;

sf::Vector2i playerSpriteSize(200, 300);

sf::Vector2i bodySize(745, 745);
sf::Vector2i eyesSize(455, 35);
sf::Vector2i mouthSize(130, 50);

sf::Vector2i bollSize(125, 125);


bool load_resources() {

	std::cout << "Loading textures..." << std::endl;
	if (!standingTex.loadFromFile("media/images/player/standing.png") ||
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

	return true;
}

#endif