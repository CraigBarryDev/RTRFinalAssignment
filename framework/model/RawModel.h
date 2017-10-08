#pragma once

#include "../GLHeaders.h"

class RawModel {
public:
	RawModel(GLuint vaoID, unsigned int vertexCount, GLfloat radius);
	const GLuint getVAOID() {return vaoID;}
	const unsigned int getVertexCount() {return vertexCount;}
	const float getModelRadius() { return radius; }
private:
	GLuint vaoID;
	unsigned int vertexCount;
	float radius;
};