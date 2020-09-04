#include <deque>

#include <SFML/Graphics.hpp>
using namespace sf;

#include "../Resources.hpp"


#define PARTICLE_LIFE_LENGTH (0.6f)


namespace ParticleSystem {

	typedef struct {
		Vector2f pos, vel;
		float timer;
	} particle;

	static std::deque<particle> *particles;
	sf::Sprite partile_sprite;

	void init() {
		particles = new std::deque<particle>();


		float radius = 2.f;
		sf::Vector2f scale((radius * 2.0f) / (float)bollSize.x, (radius * 2.0f) / (float)bollSize.y);
		sf::Vector2f origin((float)bollSize.x / 2.0f, (float)bollSize.y / 2.0f);

		partile_sprite.setTexture(bollTex);
		partile_sprite.setScale(scale);
		partile_sprite.setOrigin(origin);
		partile_sprite.setColor(Color(150,100,100));
	}

	void exit() {
		delete particles;
	}

	void addParticle(Vector2f pos, Vector2f vel) {
		particle p = {pos, vel, 0.f};
		particles->push_back(p);
	}

	void update(float elapsedTime) {

		if (!particles->empty()) {
			for (std::deque<particle>::iterator it = particles->begin(); it != particles->end(); it++) {
			    it->pos += elapsedTime * it->vel;
			    it->timer += elapsedTime;
			}

			while (!particles->empty() && particles->front().timer > PARTICLE_LIFE_LENGTH) {
				particles->pop_front();
			}
		}
	}

	void draw(RenderTarget *target) {
		if (!particles->empty()) {
			for (std::deque<particle>::iterator it = particles->begin(); it != particles->end(); it++) {
				partile_sprite.setTextureRect(sf::IntRect(bollSize.x * (3 + (int)((it->timer/PARTICLE_LIFE_LENGTH)*4.f)), 2 * bollSize.y, bollSize.x, bollSize.y));
			    partile_sprite.setPosition(it->pos);
			    target->draw(partile_sprite);
			}
		}
	}
}