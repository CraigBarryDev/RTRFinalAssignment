#include "particles.h"

Particles::Particles() {}

void vecAdd(vector<GLfloat>* v, vec3 f) {
	v->push_back(f.x);
	v->push_back(f.y);
	v->push_back(f.z);
}

void vecAdd(vector<GLfloat>* v, vec2 f) {
	v->push_back(f.x);
	v->push_back(f.y);
}

Particles::Particles(Loader* loader, int nParticles, ModelTexture* particleTexture, ParticleShader shader, vec3 srcPos) 
	:
shader(shader),
srcPos(srcPos),
texture(particleTexture)
{
	vector<GLfloat> verts;
	vecAdd(&verts, vec3(1.0f, 1.0f, 0.0f));
	vecAdd(&verts, vec3(-1.0f, 1.0f, 0.0f));
	vecAdd(&verts, vec3(-1.0f, -1.0f, 0.0f));
	vecAdd(&verts, vec3(1.0f, -1.0f, 0.0f));

	vector<GLfloat> texCoords;
	vecAdd(&texCoords, vec2(1.0f, 1.0f));
	vecAdd(&texCoords, vec2(0.0f, 1.0f));
	vecAdd(&texCoords, vec2(0.0f, 0.0f));
	vecAdd(&texCoords, vec2(1.0f, 0.0f));

	pModel = loader->loadToVAO(verts, texCoords);

	for(int i = 0; i < nParticles; i++) {
		Particle p = Particle();
		p.pos = srcPos;
		float speed = Maths::randBetweenf(MIN_PARTICLE_SPEED, MAX_PARTICLE_SPEED);
		p.veloc = speed * normalize(vec3(Maths::randBetweenf(-1.0f, 1.0f),
			Maths::randBetweenf(-1.0f, 1.0f),Maths::randBetweenf(-1.0f, 1.0f)));
		p.scale = Maths::randBetweenf(MIN_PARTICLE_SIZE, MAX_PARTICLE_SIZE);
		particles.push_back(p);
	}
}

Particles::~Particles() {}

void Particles::render() 
{
	//Bind the shader program
	shader.start();

	//Disable depth testing when drawing backdrop
	glDisable(GL_DEPTH_TEST);

	//Bind the VAO to the context
	glBindVertexArray(vao);

	//Enable the attribute lists on the VAO (our positional data)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Bind the texture to the texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

	//SET RUNNING TIME

	for(int i = 0; i < particles.size(); i++) {
		//SET PARTICLE SPEED
		//Draw the model
		glDrawArrays(GL_QUADS, 0, 4);
	}

	//Disable the attribute lists from the VAO
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//Unbind the VAO from the context
	glBindVertexArray(0);

	//Re-enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Unbind the shader program
	shader.stop();
}