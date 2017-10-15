#include "../shaders.h"

#ifndef PARTICLE_SHADER_H
#define PARTICLE_SHADER_H

class ParticleShader : public Shader {
public:
	ParticleShader() {}
	ParticleShader(string vertexFile, string fragmentFile);

	void setRunningTime(float time);
	void setParticleVeloc(vec3 veloc);

protected:
	virtual void bindAttributes() override;
};

#endif