#ifndef LIT_SHADER_H
#define LIT_SHADER_H

#include "../framework/shaders.h"

class LitShader : public Shader {
public:
	LitShader();
	LitShader(string vertexShader, string fragShader);
	~LitShader();

public:
	void bindAttributes();

	void setLightAmbient(glm::vec3 amb);
	void setLightDiffuse(glm::vec3 diff);
	void setLightSpecular(glm::vec3 spec);
	void setLightDirection(glm::vec3 lightDir);

	void setMaterialAmbient(glm::vec3 amb);
	void setMaterialDiffuse(glm::vec3 diff);
	void setMaterialSpecular(glm::vec3 spec);
};

#endif