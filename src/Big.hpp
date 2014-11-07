#ifndef _BIG_H_
#define _BIG_H_

class Big: public Object {

public:

	sf::Texture bodyTex, eyesTex, mouthTex;
	sf::Sprite body, eyes, mouth;
	sf::Vector2i bodySize, eyesSize, mouthSize;

	int body_pos;
	float body_timer;

	bool yawning;
	int sprite_pos;
	int sprite_dir;
	float timer;

	float direction;

	Big(Vector2f pos, Color color):
		Object(pos, Vector2f(0,0), 100.0f, 20.0f),

		body_pos(0),
		body_timer(0.0f),

		yawning(false),
		sprite_pos(0),
		sprite_dir(0),
		timer(0.0f),

		direction(0.0f)
	{
		if (!bodyTex.loadFromFile("media/images/clumsy/body.png")) {/*error...*/}
		if (!eyesTex.loadFromFile("media/images/clumsy/eyes.png")) {/*error...*/}
		if (!mouthTex.loadFromFile("media/images/clumsy/mouth.png")) {/*error...*/}

		bodyTex.setSmooth(true);
		eyesTex.setSmooth(true);
		mouthTex.setSmooth(true);

		bodySize = Vector2i(745, 745);
		eyesSize = Vector2i(455, 35);
		mouthSize = Vector2i(130, 50);

		sf::Vector2f scale((radius * 2.0f) / (float)bodySize.x, (radius * 2.0f) / (float)bodySize.y);


		body.setTextureRect(sf::IntRect(0, 0, bodySize.x, bodySize.y));
		body.setTexture(bodyTex);
		body.setScale(scale);
		body.setOrigin(sf::Vector2f((float)bodySize.x / 2.0f, (float)bodySize.y / 2.0f));

		eyes.setTextureRect(sf::IntRect(0, 0, eyesSize.x, eyesSize.y));
		eyes.setTexture(eyesTex);
		eyes.setScale(scale * 1.1f);
		eyes.setOrigin(sf::Vector2f((float)eyesSize.x / 2.0f, (float)eyesSize.y / 2.0f) + Vector2f(0,80));

		mouth.setTextureRect(sf::IntRect(0, 0, mouthSize.x, mouthSize.y));
		mouth.setTexture(mouthTex);
		mouth.setScale(scale * 1.4f);
		mouth.setOrigin(sf::Vector2f((float)mouthSize.x / 2.0f, (float)mouthSize.y / 2.0f) + Vector2f(0,-150));


		body.setColor(color);
		mouth.setColor(Color(200, 100, 100));
	}

	virtual ~Big() {}

	void update(float elapsedTime) {

		body_timer += elapsedTime;
		if (body_timer > 0.2f) {
			body_timer -= 0.2f;
			body_pos++;
			if (body_pos >= 6) {
				body_pos = 0;
			}
			body.setTextureRect(sf::IntRect(bodySize.x * body_pos, 0, bodySize.x, bodySize.y));
		}



		timer += elapsedTime;
		if (yawning) {
			if (timer > 0.1f) {

				timer -= 0.1f;

				if (sprite_dir == 0) {
					sprite_pos++;
					if (sprite_pos >= 8) {
						sprite_dir = 1;
						sprite_pos -= 2;
					}
				} else {
					sprite_pos--;
					if (sprite_pos < 0) {
						sprite_dir = 0;
						sprite_pos = 0;
						yawning = false;
						timer = 0.0f;
					}
				}
				eyes.setTextureRect(sf::IntRect(eyesSize.x * (sprite_pos >= 6 ? 5 : sprite_pos), 0, eyesSize.x, eyesSize.y));
				mouth.setTextureRect(sf::IntRect(mouthSize.x * sprite_pos, 0, mouthSize.x, mouthSize.y));
			}
		} else if (timer > 1.0f) {
			timer = 0.0f;
			yawning = true;
		}
	}

	virtual void draw(RenderWindow *window) {

		body.setRotation(direction);
		body.setPosition(pos);
		window->draw(body);
		eyes.setRotation(direction);
		eyes.setPosition(pos);
		window->draw(eyes);
		mouth.setRotation(direction);
		mouth.setPosition(pos);
		window->draw(mouth);
	}
};

#endif