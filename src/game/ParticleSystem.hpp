#ifndef _ParticleSystem_H_
#define _ParticleSystem_H_


namespace ParticleSystem {

	void init();
	void exit();

	void addParticle(Vector2f pos, Vector2f vel);

	void update(float elapsedTime);
	void draw(RenderTarget *target);
}


#endif